//
// Created by kylemallory on 3/2/23.
//

#ifndef STARTRACK_STARTRACK_H
#define STARTRACK_STARTRACK_H

#include <vector>
#include <chrono>

#include <Eigen/Dense>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "filesystem.hpp"

#include <InertialSenseSDK.h>
#include <InertialSense.h>
#include "data_sets.h"

#include "Blinky.h"
#include "StarTrackBase.h"


using Eigen::Vector3d;
using Eigen::VectorXd;
using Eigen::VectorXi;
using Eigen::Matrix3Xd;
using Eigen::Matrix3d;
using Eigen::MatrixBase;
using Eigen::ArrayXi;

#ifndef MAX
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b)    (((a) < (b)) ? (a) : (b))
#endif


#define SET_CALLBACK(DID, __type, __cb_fun, __periodmultiple)                               \
    IS.BroadcastBinaryData((DID), (__periodmultiple),                                      \
                            [this](InertialSense *i, p_data_t *data, int pHandle)           \
                            {                                                               \
                                /* ROS_INFO("Got message %d", DID);*/                       \
                                this->__cb_fun(DID, reinterpret_cast<__type *>(data->buf)); \
                            })


class StarTrack : public StarTrackBase {
private:
    std::shared_ptr<spdlog::logger> combined_logger;
    std::filesystem::path base_dir, log_dir, log_path;
    spdlog::level::level_enum log_level = spdlog::level::info;

    static std::vector<std::string> config_locations;

    Blinky* blinky;
    double idle_rate;
    std::vector<int> idle_colors;
    double active_rate;
    std::vector<int> active_colors;
    bool is_active = false;
    bool was_active = true;
    std::chrono::steady_clock::time_point active_timeout;

    std::string imx_port;
    InertialSense IS;
    Vector3d cur_point;
    Vector3d obs_offset;
    bool is_stationary = false;
    void INS_cb(eDataIDs DID, const ins_4_t *const msg);

    int findRemovableMedia(std::vector<std::string>& mounts);

public:
    StarTrack(Blinky &b, std::string base_path = ".", int cal_points = 0, int ini_points = 0);
    ~StarTrack() = default;

    bool connectRUG3(const std::string& port);
    void configureRUG3();

    bool hasAllPoints() { return measured_points.size() == truth_points.size(); }

    /**
     * Parses the StarTrack config file and configures relevant parameters, including loading calibration and sequence points.
     * @return true if config was successfully parsed, otherwise false.
     */
    bool loadConfig();

    /**
     * Record the current INS position from the RUG-3 and adds it as a measurement point.
     * int *nearest_point if not null, will return the index of the nearest truth point to this recorded point.
     * @return the number true if a point was successfully recorded (if false, keep trying).
     */
    bool recordPoint(int *nearest_point= nullptr);

    /**
     * Called when motion is expected to resume (after the user releases the button on the POC, or when the user lifts the wrench from the bolt).
     * @return
     */
    int setStationary();

    /**
     * Called when motion is expected to stop, notifying the IMX.  Usually, as soon as the IMX reports back that it is in a stationary state,
     * the system can collect a point location (we take the last to be received).
     * @return
     */
    int setMoving();

    /**
     * Allows processing of "behind-the-scenes" tasks, like managing the LEDs and giving a chance for the InertialSense library to run
     */
    void update();

    /**
     * Performs a reset/initialization of the app:
     *   - closes open PPD log
     *   - clears any measured points
     *   - updates LOG PPD symlink to current timestamp
     *   - starts a new PPD log using new symlink
     *
     *   -- NOTE: this function DOES NOT load the truth points... that is an external step, that is usually performed after reset is performed.
     *
     *   This should be callable at any time, including at startup before any initialization has been completed. Ie, check for open logs before closing logs, etc.
     *
     *   starting new logs, reinitializes the truth data (reload from file), clears the measured points,
     * returns true if all initialization steps were successful, otherwise false.
     */
    bool initialize();

    void resetIMX();


    /**
     * Sets the LED to the specified pattern
     * @param rate
     * @param colors
     */
    void setIdleLEDPattern(double rate, const std::vector<int>& colors);
    void setActiveLEDPattern(double rate, const std::vector<int>& colors, double timeout = 1.0);
    bool isLEDActive();

};

#endif //STARTRACK_STARTRACK_H
