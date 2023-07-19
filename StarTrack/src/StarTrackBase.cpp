//
// Created by kylemallory on 3/2/23.
//

#include <StarTrackBase.h>
#include <iostream>
#include <fstream>
#include <Eigen/SVD>

#ifdef SPDLOG_COMPILED_LIB
    #include <spdlog/spdlog.h>

    #define LOG_TRACE(log, ...)    spdlog::trace(log, __VA_ARGS__);
    #define LOG_INFO(log, ...)     spdlog::info(log, __VA_ARGS__);
    #define LOG_ERROR(log, ...)    spdlog::error(log, __VA_ARGS__);
    #define LOG_DEBUG(log, ...)    spdlog::debug(log, __VA_ARGS__);
    #define LOG_WARN(log, ...)     spdlog::warn(log, __VA_ARGS__);
    #define LOG_CRITICAL(log, ...) spdlog::critical(log, __VA_ARGS__);
#else
    #include "fmt/core.h"
    // Do something else??
    #define LOG_TRACE(log, ...)    fmt::print(log, __VA_ARGS__); fmt::print("\n");
    #define LOG_INFO(log, ...)     fmt::print(log, __VA_ARGS__); fmt::print("\n");
    #define LOG_ERROR(log, ...)    fmt::print(log, __VA_ARGS__); fmt::print("\n");
    #define LOG_DEBUG(log, ...)    fmt::print(log, __VA_ARGS__); fmt::print("\n");
    #define LOG_WARN(log, ...)     fmt::print(log, __VA_ARGS__); fmt::print("\n");
    #define LOG_CRITICAL(log, ...) fmt::print(log, __VA_ARGS__); fmt::print("\n");
#endif

StarTrackBase::StarTrackBase(int cal_points, int ini_points) {
    num_cal_points = cal_points;
    num_ini_points = ini_points;
}

bool StarTrackBase::initialize() {
    total_points = 0;
    truth_points.clear();
    measured_points.clear();
    weights.clear();
    desired_order.clear();
    actual_order.clear();
    return true;
}

double StarTrackBase::get_measured_distance(int a, int b) {
    Vector3d delta = measured_points[a] - measured_points[b];
    return delta.norm();
}

int StarTrackBase::add_calib_point(Vector3d point) {
    calib_points.push_back(point);
    num_cal_points = calib_points.size();
    return num_cal_points;
}

int StarTrackBase::add_truth_point(Vector3d point) {
    truth_points.push_back(point);
    // Assuming required sequence is 0,1,2,3,...,N
    desired_order.push_back(desired_order.size());
    return truth_points.size();
}

int StarTrackBase::add_measurement_point(Vector3d point) {
    total_points++;
    if (total_points <= num_cal_points) {
        LOG_INFO("Recording calibration point {} of {} at at ECEF: [{:3.6f}, {:3.6f}, {:3.6f}]", total_points, num_cal_points, point[0], point[1], point[2]);
        return total_points - (num_cal_points + 1);
    }

    if (measured_points.size() >= truth_points.size()) {
        LOG_WARN("Unable to record new measurement.  All measurement points have already been recorded.  Reset to start again. {}", "");
        return -99;
    }

    measured_points.push_back(point);
    auto ref = measured_points[0];
    LOG_INFO("Added new measurement point {} at ECEF: [{:3.6f}, {:3.6f}, {:3.6f}]", measured_points.size()-1, point[0], point[1], point[2]);

    actual_order.push_back(-1);
    if (measured_points.size() == 1)
        weights.push_back(1000.0);
    else {
        weights.push_back(1.0);
    }
    return measured_points.size();
}

int StarTrackBase::find_nearest_truth(Vector3d point, double* distance=nullptr) {
    int min_index = 0;
    double min_dist = (point - truth_points[0]).norm();
    for (int i = 1; i < truth_points.size(); i++) {
        double dist = (point - truth_points[i]).norm();
        if (dist < min_dist) {
            min_dist = dist;
            min_index = i;
            if (distance)
                *distance = dist;
        }
    }
    return min_index;
}

double StarTrackBase::processPoints()
{
    Matrix3Xd X(3, measured_points.size()), Y(3, truth_points.size());
    Matrix3Xd dX(3, measured_points.size()), dY(3, truth_points.size());
    Matrix3Xd X_hat(3, measured_points.size());
    Vector3d  ymean, xmean, T;
    Matrix3d  R, H;
    int ind_des;
    double sum = 0;
    double deviation = 0;

    Eigen::JacobiSVD<Eigen::MatrixXd> svd;

    for (int i = 0; i < truth_points.size(); i++) {
        Y.col(i) = truth_points[i];
    }
    for (int i = 0; i < measured_points.size(); i++) {
        X.col(i) = measured_points[i];
    }
    // Number of correspondencies required
    int N = measured_points.size();

    // Number of truth points
    int Nt = truth_points.size();

    // We are assuming that the first point is correct (heavily weighted) and
    // the rest of the sequence is mostly correct (only occasional mistakes)
    // Compute mean-centered data points
    ymean.setZero();
    sum = 0;
    for (int i = 0; i < MIN(N, Nt); i++) {
        ymean += Y.col(desired_order[i]) * weights[i];
        sum += weights[i];
    }
    ymean /= sum;
    dY = Y.colwise() - ymean;

    xmean.setZero();
    sum = 0;
    for (int i = 0; i < N; i++) {
        if (i < desired_order.size()) {
            xmean += X.col(desired_order[i]) * weights[i];
            sum += weights[i];
        }
    }
    xmean /= sum;
    dX = X.colwise() - xmean;

    // Initial ECEF-to-CAD transform
    R.setIdentity();
    T = ymean - R * xmean;
    //X_hat = (R * X).colwise() + T;
    X_hat = (R * dX).colwise() + ymean;

    bool done = false;
    int iter = 0;
    while (iter < 100 && !done)
    {
        // For the first iteration we use assumed/required sequence,
        // for the next iterations we use minimum distance
        //if (iter > 0 && measured_points.size() > num_ini_points) {
            //ind.setLinSpaced(0, Nt - 1);
            //for (int i = num_ini_points; i < measured_points.size(); i++) {
            //    ind(i) = find_nearest_truth(X_hat.col(i));
            //}
        //}

        // Solve for R and T using SVD
        H.setZero();
        for (int i = 0; i < measured_points.size(); i++)
        {
            ind_des = MIN(i, Nt); // can be modified if multiple point visits allowed
            H = H + dY.col(desired_order[ind_des]) * dX.col(i).transpose() * weights[i];
        }
        svd.compute(H, Eigen::NoQRPreconditioner | Eigen::ComputeThinV | Eigen::ComputeThinU);
        R = svd.matrixU() * svd.matrixV().transpose();
        T = ymean - R * xmean;

        // Are we done?
        //if ( (ind == ind_prev).all() && iter > 0 ) {
        done = true;
        //}
        //else {
        //    iter++;
        //}
        // Updated EKF output in CAD frame
        X_hat = (R * dX).colwise() + ymean;
        //X_hat = (R * X).colwise() + T;
    }

    // Find the actual sequence
    //if (measured_points.size() > num_ini_points) {
    deviation = 0;
    for (int i = 0; i < measured_points.size(); i++) {
        double dist;
        actual_order[i] = find_nearest_truth(X_hat.col(i), &dist);
        deviation += (dist * dist);
    }
    // FIXME: this isn't a true standard-deviation (it uses the difference from nearest point, rather than the mean distance of all points)  -- but it should get us in the ballpark.
    deviation = std::sqrt(deviation / (double)measured_points.size());
    //}

    int num = measured_points.size()-1;
    LOG_INFO("                            {} at CAD : [{:3.6f}, {:3.6f}, {:3.6f}]", num ,X_hat.rightCols(1)(0), X_hat.rightCols(1)(1), X_hat.rightCols(1)(2));
    LOG_INFO("                            CAD Truth : [{:3.6f}, {:3.6f}, {:3.6f}]", Y.col(num)(0), Y.col(num)(1), Y.col(num)(2));
    LOG_INFO("Distance from previous point {}: {:2.4f} meters.", measured_points.size()-2, get_measured_distance(measured_points.size()-1, measured_points.size()-2));

    double nearest_dist = 0;
    int nearest_point = find_nearest_truth(X_hat.rightCols(1), &nearest_dist);
    LOG_INFO("Nearest truth point to this location was point {} (at {:3.3f} meters, std-dev: {:3.6f}) .", nearest_point, nearest_dist, deviation);

    std::stringstream ss_desired;
    for (auto a : desired_order) { ss_desired << std::setfill(' ') << std::setw(2) << a << " "; }
    LOG_INFO("Desired sequence: {}", ss_desired.str());

    std::stringstream ss_actual;
    for (auto a: actual_order) { ss_actual << std::setfill(' ') << std::setw(2) << a << " "; }
    LOG_INFO("Actual sequence:  {}", ss_actual.str());

    //std::cout << "R: " << R << std::endl;
    //std::cout << "T: " << T.transpose() << std::endl;
    std::cout << std::endl << std::fixed;
    Eigen::IOFormat coord_fmt(5, 0, ", ", "\n", "[", "]");
    std::cout << "Y:     " << std::endl << Y.format(coord_fmt) << std::endl << std::endl;
    std::cout << "X_hat: " << std::endl << X_hat.format(coord_fmt) << std::endl << std::endl;

    return deviation;
}

bool StarTrackBase::validate_measurements() {
    for (int i = 0; i < std::min(measured_points.size(), truth_points.size()); i++) {
        if (actual_order[i] != desired_order[i])
            return false;
    }
    return true;
}

int StarTrackBase::load_truth_points(const std::string &file) {
    truth_points.clear();
    std::vector<Vector3d> points;
    int result = load_points(file, points);
    for (auto p : points)
        add_truth_point(p);
    return result;
}

int StarTrackBase::load_points(const std::string& file, std::vector<Vector3d>& points) {
    char line_buff[128]; // temp buffer to reading (this should be more than enough)
    int line_no = 0;
    double xyz[3];

    std::ifstream is(file);
    if (!is.is_open()) {
        LOG_ERROR("Unable to open points file: {}", file.c_str());
        return -1;
    }

    while (!is.eof()) {
        is.getline(line_buff, sizeof(line_buff)-1);
        line_no++;

        if (sscanf(line_buff, "%lf %lf %lf", &xyz[0], &xyz[1], &xyz[2]) != 3) {
            LOG_ERROR("Error parsing points data on line ({}) in file {}",  line_no, file.c_str());
            is.close();
            return -2;
        }

        Vector3d vec;
        vec << xyz[0], xyz[1], xyz[2];
        points.push_back(vec);
    }
    is.close();
    return points.size();
}

int StarTrackBase::save_measured_points(const std::string &file) {
    return save_points(file, measured_points);
}

int StarTrackBase::save_points(const std::string& file, std::vector<Vector3d>& points) {
    char line_buff[128]; // temp buffer to reading (this should be more than enough)
    int line_no = 0;
    double xyz[3];

    std::ofstream os(file);
    if (!os.is_open()) {
        LOG_ERROR("Error, Unable to create points file: {}", file.c_str());
        return -1;
    }

    os << std::fixed << std::setprecision(12);
    for (auto p : points) {
        os << p[0] << ", " << p[1] << ", " << p[2] << std::endl;
        os.flush();
    }
    os.close();
    return points.size();
}

