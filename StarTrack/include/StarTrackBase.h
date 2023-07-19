//
// Created by kylemallory on 3/2/23.
//

#ifndef STARTRACKBASE_STARTRACK_H
#define STARTRACKBASE_STARTRACK_H

#include <filesystem>
#include <vector>
#include <Eigen/Dense>

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

class StarTrackBase {
protected:
    std::vector<Vector3d> calib_points;
    std::vector<Vector3d> truth_points;
    std::vector<Vector3d> measured_points;
    std::vector<double>   weights;
    std::vector<int> desired_order, actual_order;
    int total_points = 0;
    int num_cal_points = 0;
    int num_ini_points = 0;

public:
    StarTrackBase(int cal_points = 0, int ini_points = 0);
    ~StarTrackBase() = default;

    int get_desired_order(int index) { return desired_order[index]; }
    int get_actual_order(int index) { return actual_order[index]; }
    int add_calib_point(Vector3d point); // returns the number of calibration points in vector
    int add_truth_point(Vector3d point); // returns number of points in vector
    int add_measurement_point(Vector3d point); // returns number of points in vector
    double get_measured_distance(int a, int b); // returns the distance in meters between two measured points (specified by index to the measured points).
    double processPoints(); // Solve for sequence

    int find_nearest_truth(Vector3d point, double* distance); // returns the index of truth point which is nearest to the specified point
    Vector3d get_avg_deviation(); // returns the average deviation for each axis across all measurements vs all truths

    /**
     * Performs a simple match of indices of desired_points and actual_points.
     * @return  true if they match, otherwise false.
     */
    bool validate_measurements(); // returns true if the algorythm detects that all measurements match all truths

    /*
     * These are helper/utility functions that aren't required for the core functionality, but which may be useful in reporting, etc.
     */
    int load_points(const std::string& file, std::vector<Vector3d>& points);
    int load_truth_points(const std::string& file);

    int save_measured_points(const std::string& file);
    int save_points(const std::string& file, std::vector<Vector3d>& points);

    /**
     * Clears all measured points, and resets the state of the system
     *   -- NOTE: a reset DOES NOT load the truth points... that is an external step, that is usually performed after reset is performed.
     * returns true if all initialization steps were successful, otherwise false.
     */
    bool initialize();
};

#endif //STARTRACKBASE_STARTRACK_H
