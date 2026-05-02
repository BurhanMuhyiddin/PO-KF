#include <core/VioManager.h>
#include <state/State.h>

#include "po-kf/po_kf.hpp"

namespace msceqf
{

    PO_kf::PO_kf(const std::string configPath)
    {
        const auto parser = std::make_shared<ov_core::YamlParser>(configPath);

        std::string verbosity = "DEBUG";
        parser->parse_config("verbosity", verbosity);
        ov_core::Printer::setPrintLevel(verbosity);

        ov_msckf::VioManagerOptions params;
        params.print_and_load(parser);
        // params.print_and_load_simulation(parser);

        if (!parser->successful()) {
            std::cerr << "Problem reading parameters\n";
        }

        vio = std::make_shared<ov_msckf::VioManager>(params);
    }

    void PO_kf::processImuMeasurement(const Imu& imu)
    {
        ov_core::ImuData data;
        data.timestamp = imu.timestamp_;
        data.wm = imu.ang_;
        data.am = imu.acc_;

        // std::cout << "timestamp: " << data.timestamp << "\n";
        // std::cout << "am: " << data.am.transpose() << "\n";
        // std::cout << "----------------\n";

        vio->feed_measurement_imu(data);
    }

    void PO_kf::processCameraMeasurement(Camera& cam)
    {
        ov_core::CameraData data;
        data.timestamp = cam.timestamp_;
        data.sensor_ids = {0};
        data.masks = {cv::Mat(cam.image_.size(), CV_8UC1, cv::Scalar(0))};
        data.images = {cam.image_};

        vio->feed_measurement_camera(data);
    }

    Vector4 PO_kf::getAttitude() const {
        return vio->get_state()->_imu->quat();
    }

    Vector3 PO_kf::getPosition() const {
        return vio->get_state()->_imu->pos();
    }

    std::vector<Vector3> PO_kf::getFeatures() const {
        return vio->get_features_SLAM();
    }

}