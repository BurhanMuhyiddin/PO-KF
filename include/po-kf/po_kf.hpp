#ifndef PO_KF_HPP_
#define PO_KF_HPP_

#include <core/VioManager.h>
#include "sensors/sensor_data.hpp"

namespace msceqf
{
    class PO_kf
    {
    public:
        PO_kf() = default;

        PO_kf(const std::string configPath);

        void processMeasurement(const Imu& meas) { processImuMeasurement(meas); }

        void processMeasurement(Camera& meas) { processCameraMeasurement(meas); }

        bool isInit() { return vio->initialized(); }

        Vector4 getAttitude() const;

        Vector3 getPosition() const;

        std::vector<Vector3> getFeatures() const;

    private:
        void processImuMeasurement(const Imu& imu);

        void processCameraMeasurement(Camera& cam);

    private:
        std::shared_ptr<ov_msckf::VioManager> vio;
    };
}
#endif //PO_KF_HPP_