cmake_minimum_required(VERSION 3.13)

project(
    po-kf
    VERSION 1.0
    DESCRIPTION "PO-KF"
    LANGUAGES CXX
)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OPENVINS_INCLUDE_DIR "/usr/local/include/open_vins")
set(OPENVINS_MSCKF_LIB "/usr/local/lib/libov_msckf_lib.so")

include(FetchContent)

find_package(Eigen3 REQUIRED)
find_package(Ceres REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(OpenCV 4 REQUIRED)

FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG master
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(yaml-cpp)

message(STATUS "Eigen VER: ${EIGEN3_VERSION}")
message(STATUS "Boost VER: ${Boost_VERSION}")
message(STATUS "OpenCV VER: ${OpenCV_VERSION}")

set(PROJECT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(LIB_SOURCES
    src/po-kf/po_kf.cpp
)

add_library(po_kf_lib STATIC
    ${LIB_SOURCES}
)

target_include_directories(po_kf_lib PUBLIC
    ${PROJECT_INCLUDE_DIR}
    ${OPENVINS_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(po_kf_lib PUBLIC
    Eigen3::Eigen
    Ceres::ceres
    yaml-cpp::yaml-cpp
    Boost::filesystem
    Boost::system
    ${OpenCV_LIBRARIES}
    ${OPENVINS_MSCKF_LIB}
    pthread
)

add_executable(po_kf
    main.cpp
)

target_link_libraries(po_kf PRIVATE
    po_kf_lib
)