//
// Created by kylemallory on 3/2/23.
//

#include <StarTrack.h>
#include <iostream>
#include <time.h>
#include <Eigen/Dense>
#include <Eigen/SVD>
#include <yaml-cpp/yaml.h>

std::vector<std::string> StarTrack::config_locations = { "./Cube.StarTrack.yaml", "~/Cube.StarTrack.yaml", "/etc/Cube.StarTrack.yaml" };

StarTrack::StarTrack(Blinky &b, std::string base_path, int cal_points, int ini_points) : StarTrackBase(cal_points, ini_points), blinky(&b), cur_point(FP_NAN, FP_NAN, FP_NAN) {
    base_dir = std::filesystem::path(base_path);
    std::filesystem::current_path(base_dir);

    idle_rate = 0.2;
    idle_colors = Blinky::CYCLE_RGB_OFF;

    active_rate = 1.0;
    active_colors = Blinky::SOLID_RED;

    was_active = true;
    is_active = false;
    active_timeout = std::chrono::steady_clock::now();
}

int StarTrack::findRemovableMedia(std::vector<std::string>& mounts) {
    int line_no = 0;

    std::ifstream is("/proc/mounts");
    if (!is.is_open()) {
        spdlog::error("Unable to open /proc/mounts.");
        return -1;
    }

    for (std::string line; std::getline(is, line); ) {
        line_no++;
        std::string device, mount, fstype, opts;
        int r_flags, w_flags;

        std::stringstream ss(line);
        ss >> device >> mount >> fstype >> opts >> r_flags >> w_flags;

        // handle path names with special characters.
        size_t pos = 0;
        while ((pos = mount.find("\\")) != std::string::npos) {
            auto escape_str = mount.substr(pos+1, 3);
            int octal = std::stoi(escape_str, 0, 8);
            mount.erase(pos, 4);
            mount.insert(pos, 1, octal);
        }

        if ((fstype.find("ext") == 0) || (fstype.find("fat") != std::string::npos)) {
            if (mount.rfind("/media", 0) == 0)
            {
                // don't bother parsing options unless these other criteria are met first.
                // std::vector<std::string> options;  // if we decide we want to retain the options for evaluation
                std::stringstream opts_ss(opts);
                std::string opt;
                size_t pos;
                while ((pos = opts.find(",")) != std::string::npos) {
                    opt = opts.substr(0, pos);
                    if (opt == "rw") {
                        mounts.push_back(mount);
                        break;
                    }
                    // options.push_back(opt);
                    opts.erase(0, pos + 1);
                }

            }
        }
    }
    is.close();
    return mounts.size();
}

/***
 *
 * log_dir: <path to where logs are stored>.
 * log_level: <spdlog::level>
 *
 * calibration_points:
 *  - [x, y, z]
 *  - [x, y, z]
 *  - [x, y, z]
 *
 * sequence_points:
 *  - [x, y, z]
 *  - [x, y, z]
 *  - [x, y, z]
 *  - [x, y, z]
 *
 **/

bool StarTrack::loadConfig() {
    // scan for all removeable media first, and add to 'config_locations'

    std::vector<std::string> root_paths;
    if (findRemovableMedia(root_paths) > 0) {
        // just set the current directory to the first detected 'media'
        auto root_path = std::filesystem::path(root_paths[0]);
        std::filesystem::current_path(root_path);
    }
    spdlog::info("Using directory: {}", std::filesystem::current_path().generic_string());
    base_dir = std::filesystem::current_path();

    for (auto cfg_file : config_locations) {
        if (!std::filesystem::is_regular_file(cfg_file)) {
            // spdlog::error("Unable to open configuration file: {} :: File not found.", cfg_file);
            continue;
        }

        try {
            YAML::Node config = YAML::LoadFile(cfg_file);

            if (config["gpio_host"])
                blinky->initGPIO(config["gpio_host"].as<std::string>());
            else
                blinky->initGPIO();

            log_dir = "/var/log/StarTrack";
            if (config["log_dir"])
                log_dir = config["log_dir"].as<std::string>();

            if (config["log_level"])
                log_level = spdlog::level::from_str(config["log_level"].as<std::string>());
            spdlog::set_level(log_level);

            // load the observation/imx offset
            auto offset = config["imx_offset"];
            if (offset)
                obs_offset = {offset[0].as<double>(), offset[1].as<double>(), offset[2].as<double>()};

            // load the calibration points, if any, to the calibration set.
            auto cal_points = config["calibration_points"];
            calib_points.clear();
            if (cal_points) {
                for (auto pos: cal_points) {
                    add_calib_point({pos[0].as<double>(), pos[1].as<double>(), pos[2].as<double>()});
                }
            }

            // load the sequence/truth points, if anyt.
            truth_points.clear();
            auto seq_points = config["sequence_points"];
            for(auto pos : seq_points) {
                Vector3d point = { pos[0].as<double>(), pos[1].as<double>(), pos[2].as<double>() };
                add_truth_point( point );
            }
            spdlog::info("Successfully parsed configuration from {}.", cfg_file);
            return true;
        } catch(const YAML::BadFile& e) {
            spdlog::error("Unable to open configuration file: {} :: {}", cfg_file, e.msg);
        } catch(const YAML::ParserException& e) {
            spdlog::error("Unable to parse configuration file: {} :: {}", cfg_file, e.msg);
        }
    }
    return false;
}

bool StarTrack::initialize() {
    char timestamp[32];
    std::time_t now_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm now_tm = *std::localtime(&now_c);
    strftime(timestamp, sizeof(timestamp), "%Y%m%d-%H%M%S", &now_tm);

    if (!StarTrackBase::initialize())
        return false;

    loadConfig();

    if (IS.LoggerEnabled()) {
        // flush and close any current logs
        IS.SetTimeoutFlushLoggerSeconds(0);
        IS.SetLoggerEnabled(false, log_path);
    }

    std::filesystem::path cur_path(base_dir);
    log_path = base_dir / log_dir / timestamp;

    std::filesystem::path sympath = base_dir / log_dir / "latest";

    // look for, and remove any 'current' symlink that isn't already pointing to our timestamp-log directory (very unlikely).
    auto link_status = std::filesystem::symlink_status(sympath);
    if (is_symlink(link_status)) {
        auto link_target = std::filesystem::read_symlink(sympath);
        if (link_target.compare(log_path.c_str()) != 0) {
            // the "current" symlink is pointing to the wrong target, so delete the symlink (not the target, and recreate it)
            std::filesystem::remove(sympath);
        }
    }

    // make the new log directory
    if (!std::filesystem::create_directories(log_path)) {
        spdlog::error("Unable to create path for log files. Error {}", errno);
        return false;
    }

    // finally, update the symlink...  if it already exists, then we're good; if it doesn't exist, create it
    link_status = std::filesystem::symlink_status(sympath);
    if (!is_symlink(link_status)) {
        try {
            std::filesystem::create_symlink(log_path.filename(), sympath);
        } catch (const std::filesystem::filesystem_error &err) {
            // some filesystems don't support symlinks (looking at you, FAT)
            spdlog::warn("Unable to create symlink to 'latest' log directory: {} ", err.what());
        }
    }

    auto cur_level = spdlog::default_logger()->level();

    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_st>());
    sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_path / "startrack.log"));

    combined_logger = std::make_shared<spdlog::logger>("StarTrack", begin(sinks), end(sinks));
    //spdlog::register_logger(combined_logger);
    spdlog::set_default_logger(combined_logger);
    spdlog::flush_every(std::chrono::seconds(1));
    spdlog::flush_on(spdlog::level::info);
    spdlog::set_level(cur_level);
    // spdlog::info("Using directory: {}", std::filesystem::current_path().generic_string());

    // Enable Inertial-Sense logging
    if (!IS.LoggerEnabled()) {
        // flush (there shouldn't be anything) and start logging.
        IS.SetTimeoutFlushLoggerSeconds(5);
        IS.SetLoggerEnabled(true, log_path.generic_string());
    }

    spdlog::info("Initialization complete.");
    return true;
}

bool StarTrack::connectRUG3(const std::string& port) {
    bool connected = false;

    imx_port = port;
    auto next_attempt = std::chrono::steady_clock::now();
    do {
        auto now = std::chrono::steady_clock::now();
        if (next_attempt <= now) {
            spdlog::info("Connecting to RUG-3 [ {} ]", port);
            connected = IS.Open(port.c_str(), IS_COM_BAUDRATE_DEFAULT);
            if (!connected) {
                next_attempt = std::chrono::steady_clock::now() + std::chrono::milliseconds(1000);
            }
        }
        if (blinky != nullptr)
            blinky->update();
    } while (!connected);

    return true;
}

void StarTrack::configureRUG3() {
    nvm_flash_cfg_t flash_cfg;

    if (IS.LoggerEnabled()) {
        // flush and close any current logs
        IS.SetTimeoutFlushLoggerSeconds(0);
        IS.SetLoggerEnabled(false, log_path);
    }

    // Enable Inertial-Sense logging
    if (!IS.LoggerEnabled()) {
        // flush (there shouldn't be anything) and start logging.
        IS.SetTimeoutFlushLoggerSeconds(0);
        IS.SetLoggerEnabled(true, log_path);
    }

    auto dev_info = IS.GetDeviceInfo();
    spdlog::info("IMX Firmware Version: {}.{}.{}.{}", dev_info.firmwareVer[0], dev_info.firmwareVer[1], dev_info.firmwareVer[2], dev_info.firmwareVer[3]);

    IS.GetFlashConfig(flash_cfg);
    spdlog::info("startupNavDtMs: {}", flash_cfg.startupNavDtMs);
    spdlog::info("startupImuDtMs: {}", flash_cfg.startupImuDtMs);
    spdlog::info("ioConfig: 0x{:08x}", flash_cfg.ioConfig);
    spdlog::info("insDynModel: {}", flash_cfg.insDynModel);
    spdlog::info("sysCfgBits: 0x{:08x}", flash_cfg.sysCfgBits);

    // flash_cfg.ioConfig = 0x0044;
    // flash_cfg.sysCfgBits = 0x0200;

    flash_cfg.startupNavDtMs = 16;
    flash_cfg.startupImuDtMs = 1;
    flash_cfg.insDynModel = 10; // indoor model
    flash_cfg.localPosVelObsOffset[0] = obs_offset[0];
    flash_cfg.localPosVelObsOffset[1] = obs_offset[1];
    flash_cfg.localPosVelObsOffset[2] = obs_offset[2];
    spdlog::info("Configuring local Observation<>IMX offset: [{:3.6f}, {:3.6f}, {:3.6f}]", flash_cfg.localPosVelObsOffset[0], flash_cfg.localPosVelObsOffset[1], flash_cfg.localPosVelObsOffset[2]);

    IS.SetFlashConfig(flash_cfg);
    usleep(1000);
    IS.Update();
    IS.GetFlashConfig(flash_cfg);

    if (flash_cfg.insDynModel != 10)
        resetIMX();

    // check if we need to reset first...
    SET_CALLBACK(DID_INS_4, ins_4_t, INS_cb, 10);

    IS.SetSysCmd(SYS_CMD_REF_POINT_MOVING);
}

void StarTrack::resetIMX()
{
    // send reset command
    spdlog::info("Rebooting IMX and reinitializing StarTrack...");
    system_command_t reset_command;
    reset_command.command = 99;
    reset_command.invCommand = ~reset_command.command;
    IS.SendData(DID_SYS_CMD, reinterpret_cast<uint8_t *>(&reset_command), sizeof(system_command_t), 0);
    sleep(2);
    IS.Close();
    sleep(2);

    setIdleLEDPattern(1.0, Blinky::CYCLE_RED_YELLOW);
    bool connected = connectRUG3(imx_port);
    if (connected)
        configureRUG3();
    setIdleLEDPattern(1.0, Blinky::SOLID_BLUE);
    initialize();
}


void StarTrack::INS_cb(eDataIDs DID, const ins_4_t *const msg) {
    spdlog::debug("ECEF: [{:3.6f}, {:3.6f}, {:3.6f}]  insStatus: 0x{:08x}", msg->ecef[0], msg->ecef[1], msg->ecef[2], msg->insStatus);
    if (msg->insStatus & INS_STATUS_STATIONARY_MODE) {
        is_stationary = true;
        if (cur_point[0] == cur_point[0]) {
            cur_point[0] = msg->ecef[0];
            cur_point[1] = msg->ecef[1];
            cur_point[2] = msg->ecef[2];
        }
    } else {
        is_stationary = false;
        cur_point[0] = FP_NAN;
        cur_point[1] = FP_NAN;
        cur_point[2] = FP_NAN;
    }
}

int StarTrack::setStationary() {
    IS.SetSysCmd(SYS_CMD_REF_POINT_STATIONARY);
    spdlog::info("Entering Stationary Mode.");
    return 0;
}

int StarTrack::setMoving() {
    spdlog::info("Exiting Stationary Mode.");
    IS.SetSysCmd(SYS_CMD_REF_POINT_MOVING);
    return 0;
}

bool StarTrack::recordPoint(int *np) {
    // we want to force a 3-second hold, before actually record the point.
    setActiveLEDPattern(0.2, Blinky::BLINK_YELLOW, 3.0);
    auto time_out = std::chrono::steady_clock::now() + std::chrono::milliseconds(3000);
    while (std::chrono::steady_clock::now() < time_out) {
        update();
    }

    // get the INS position
    // we need to wait for a bit, until we get a valid point
    if (!is_stationary || cur_point[0] != cur_point[0] || (cur_point[0] == 0.0 && cur_point[1] == 0.0 && cur_point[2] == 0.0))
        return false;

    int point = add_measurement_point(cur_point);
    if (point < 0) {
        setActiveLEDPattern(0.5, Blinky::BLINK_MAGENTA, 2.0);
        return true;
    }

    // Flush the log
    if (IS.LoggerEnabled()) {
        // flush the logger and close it
        IS.SetTimeoutFlushLoggerSeconds(0);
    }
    processPoints();
    save_measured_points(log_path.generic_string() + "/points.measured");

    if (validate_measurements()) {
        setActiveLEDPattern(0.5, Blinky::BLINK_GREEN, 2.0);
    } else
        setActiveLEDPattern(0.5, Blinky::BLINK_RED, 2.0);

    return true;
}

void StarTrack::setIdleLEDPattern(double rate, const std::vector<int> &colors) {
    idle_rate = rate;
    idle_colors = colors;
    blinky->setPattern(idle_rate, idle_colors);
    spdlog::debug("Setting Idle LED pattern: [{}], {}", fmt::join(colors, ", "), rate);
}

void StarTrack::setActiveLEDPattern(double rate, const std::vector<int> &colors, double timeout) {
    active_rate = rate;
    active_colors = colors;
    spdlog::debug("Setting Active LED pattern: [{}], {}", fmt::join(colors, ", "), rate);
    blinky->setPattern(active_rate, active_colors);
    if (timeout == timeout) {
        is_active = true;
        active_timeout = (std::chrono::steady_clock::now() + std::chrono::milliseconds((long)(1000 * timeout)));
    }
}

bool StarTrack::isLEDActive() { return is_active; }

void StarTrack::update() {
    if (blinky != nullptr) {
        if (is_active) {
            if (was_active != is_active) {
                blinky->setPattern(active_rate, active_colors);
            }
            if (std::chrono::steady_clock::now() > active_timeout) {
                is_active = false;
                blinky->setPattern(idle_rate, idle_colors);
            }
        } else if (was_active) {
            blinky->setPattern(idle_rate, idle_colors);
        }
        blinky->update();
    }
    was_active = is_active;
    IS.Update();
}
