//
// Created by kylemallory on 3/7/23.
//

#include <StarTrackBase.h>
#include <Eigen/Dense>

#include <gtest/gtest.h>

// #define INCLUDE_GTEST_PRINTF
#ifdef INCLUDE_GTEST_PRINTF
namespace testing
{
    namespace internal
    {
        enum GTestColor {
            COLOR_DEFAULT,
            COLOR_RED,
            COLOR_GREEN,
            COLOR_YELLOW
        };

        extern void ColoredPrintf(GTestColor color, const char* fmt, ...);
    }
}
#define PRINTF(...)  do { testing::internal::ColoredPrintf(testing::internal::COLOR_GREEN, "[          ] "); testing::internal::ColoredPrintf(testing::internal::COLOR_YELLOW, __VA_ARGS__); } while(0)
#else
#define PRINTF(...)  std::cout
#endif
#define TEST_COUT  PRINTF(...)

// Demonstrate some basic assertions.
TEST(StarTrack_Tests, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

TEST(StarTrack_Tests, LoadTruthTest) {
    std::vector<Vector3d> points;

    StarTrackBase st;

    int num = st.load_points("../test/test.truth", points);
    ASSERT_EQ(num, 5);

    std::vector<Vector3d> measured;
    measured.push_back( { 0.0, 1.0, 2.0 } );
    measured.push_back( { 3.0, 4.0, 5.0 } );
    measured.push_back( { 6.0, 7.0, 8.0 } );
    measured.push_back( { 8.25, 8.5, 8.75 } );
    measured.push_back( { 9.25, 9.5, 9.75 } );

    EXPECT_EQ(points[0], measured[0]);
    EXPECT_EQ(points[1], measured[1]);
    EXPECT_EQ(points[2], measured[2]);
    EXPECT_EQ(points[3], measured[3]);
    EXPECT_EQ(points[4], measured[4]);
}

TEST(StarTrack_Tests, NearestPoint_Tests) {
    std::vector<Vector3d> points;

    StarTrackBase st;
    int num = st.load_truth_points("../test/test.truth");
    ASSERT_EQ(num, 5);

    double dist = 0.0;
    st.add_measurement_point({ 6.0, 7.0, 8.0} );
    int p = st.find_nearest_truth( { 6.0, 7.0, 8.0}, &dist );
    EXPECT_EQ(p, 2);

    st.add_measurement_point({ 6.0, 3.0, 4.0} );
    p = st.find_nearest_truth( { 6.0, 3.0, 4.0}, &dist );
    EXPECT_EQ(p, 1);

    st.add_measurement_point({ 2.0, 9.0, 3.0} );
    p = st.find_nearest_truth( { 2.0, 9.0, 7.0}, &dist );
    EXPECT_EQ(p, 2);

}

TEST(StarTrack_Tests, InitializeLogs_Tests) {
    StarTrackBase st;
    // st.initialize();

    // check to make sure the log directory exists
    // check to make sure the symlink directory exists

    // cleanup and remove ./logs/*
}

TEST(StarTrack_Tests, Reset_Tests) {
    StarTrackBase st;
    // st.initialize();

    // check to make sure the log directory exists
    // check to make sure the symlink directory exists

    // st.reset();

    // check to make sure the new log directory exists
    // check to make sure the symlink directory points to the new log directory
    // check that points are saved
    // check that measured_is cleared
}

TEST(StarTrack_Tests, Process_Tests) {
    std::vector<Vector3d> tpoints;
    std::vector<Vector3d> mpoints;
    Vector3d point;
    int num_cal_pts = 3;
    int num_ini_pts = 4;

    StarTrackBase st(num_cal_pts, num_ini_pts);

    TEST_COUT << "Loading truth points." << std::endl;
    // Aluminum 8-lug 3-sided pattern cube
    // Side A
    point << 0.16, 0.2489, 0.0;   tpoints.push_back(point);
    point << 0.16f, 0.0711, 0.0;   tpoints.push_back(point);
    point << 0.2489, 0.16, 0.0;   tpoints.push_back(point);
    point << 0.0711, 0.16, 0.0;   tpoints.push_back(point);
    point << 0.222738, 0.222738, 0.0;   tpoints.push_back(point);
    point << 0.097262, 0.097262, 0.0;   tpoints.push_back(point);
    point << 0.222738, 0.097262, 0.0;   tpoints.push_back(point);
    point << 0.097262, 0.222738f, 0.0;   tpoints.push_back(point);
    // Side B
    point << 0.16, 0.0, 0.0711;      tpoints.push_back(point);
    point << 0.16, 0.0, 0.2489;      tpoints.push_back(point);
    point << 0.2489, 0.0, 0.16;        tpoints.push_back(point);
    point << 0.0711, 0.0, 0.16;        tpoints.push_back(point);
    point << 0.222738, 0.0, 0.097262;    tpoints.push_back(point);
    point << 0.097262, 0.0, 0.222738;    tpoints.push_back(point);
    point << 0.222738, 0.0, 0.222738;    tpoints.push_back(point);
    point << 0.097262, 0.0, 0.097262;    tpoints.push_back(point);
    // Side C
    point << 0.0, 0.16, 0.0711;      tpoints.push_back(point);
    point << 0.0, 0.16, 0.2489;      tpoints.push_back(point);
    point << 0.0, 0.0711, 0.16;        tpoints.push_back(point);
    point << 0.0, 0.2489, 0.16;        tpoints.push_back(point);
    point << 0.0, 0.097262, 0.097262;    tpoints.push_back(point);
    point << 0.0, 0.222738, 0.222738;    tpoints.push_back(point);
    point << 0.0, 0.097262, 0.222738;    tpoints.push_back(point);
    point << 0.0, 0.222738, 0.097262;    tpoints.push_back(point);

    for (int i = 0; i < tpoints.size(); i++) {
        st.add_truth_point(tpoints[i]);
    }

    TEST_COUT << "Loading measured points." << std::endl;
    // Measured points
    // First 3 are Jake's calibration points
    point << -1.798019522243272e6, -4.536312368985481e6, 4.095677423413508e6;    mpoints.push_back(point);
    point << -1.798019743950901e6, -4.536312321795086e6, 4.095677267588068e6;    mpoints.push_back(point);
    point << -1.798019558614969e6, -4.536312481261311e6, 4.095677183419921e6;    mpoints.push_back(point);
    // Actual cube sequence
    point << -1.798019517203591e6, -4.536312415089283e6, 4.095677367936790e6;    mpoints.push_back(point); // 0
    point << -1.798019685237269e6, -4.536312399602084e6, 4.095677316938862e6;    mpoints.push_back(point); // 1
    point << -1.798019613336576e6, -4.536312342770044e6, 4.095677402316733e6;    mpoints.push_back(point); // 2
    point << -1.798019589426526e6, -4.536312467577970e6, 4.095677283128023e6;    mpoints.push_back(point); // 3
    point << -1.798019552224239e6, -4.536312374336401e6, 4.095677401048793e6;    mpoints.push_back(point); // 4
    point << -1.798019650285735e6, -4.536312442918447e6, 4.095677285715976e6;    mpoints.push_back(point); // 5
    point << -1.798019663628433e6, -4.536312367143728e6, 4.095677366267579e6;    mpoints.push_back(point); // 6
    point << -1.798019545139160e6, -4.536312437969652e6, 4.095677324591906e6;    mpoints.push_back(point); // 7
    point << -1.798019731808595e6, -4.536312290106419e6, 4.095677227078727e6;    mpoints.push_back(point); // 8

    //point << -1.798019682635105e6, -4.536312172986366e6, 4.095677116288032e6;    mpoints.push_back(point); // 9
    //point << -1.798019713899294e6, -4.536312172564074e6, 4.095677227658298e6;    mpoints.push_back(point); // 10

    // TEST: flip the order of points 9 and 10
    point << -1.798019713899294e6, -4.536312172564074e6, 4.095677227658298e6;    mpoints.push_back(point); // 10
    point << -1.798019682635105e6, -4.536312172986366e6, 4.095677116288032e6;    mpoints.push_back(point); // 9


    point << -1.798019684970558e6, -4.536312300180818e6, 4.095677107640639e6;    mpoints.push_back(point); // 11
    point << -1.798019725482318e6, -4.536312239756521e6, 4.095677254010813e6;    mpoints.push_back(point); // 12
    point << -1.798019658327266e6, -4.536312242092346e6, 4.095677098753882e6;    mpoints.push_back(point); // 13
    point << -1.798019665405295e6, -4.536312165045403e6, 4.095677169059504e6;    mpoints.push_back(point); // 14
    point << -1.798019686159439e6, -4.536312336958287e6, 4.095677160899903e6;    mpoints.push_back(point); // 15
    point << -1.798019509950534e6, -4.536312453775291e6, 4.095677167013520e6;    mpoints.push_back(point); // 16
    point << -1.798019461375211e6, -4.536312330086507e6, 4.095677060349145e6;    mpoints.push_back(point); // 17
    point << -1.798019575886232e6, -4.536312381392281e6, 4.095677087361727e6;    mpoints.push_back(point); // 18
    point << -1.798019413027543e6, -4.536312400510028e6, 4.095677140208910e6;    mpoints.push_back(point); // 19
    point << -1.798019575590895e6, -4.536312421692585e6, 4.095677131866417e6;    mpoints.push_back(point); // 20
    point << -1.798019432348833e6, -4.536312351114717e6, 4.095677083674763e6;    mpoints.push_back(point); // 21
    point << -1.798019543646745e6, -4.536312323787866e6, 4.095677061641934e6;    mpoints.push_back(point); // 22
    point << -1.798019464446433e6, -4.536312428227114e6, 4.095677175492271e6;    mpoints.push_back(point); // 23

    TEST_COUT << mpoints.size() << " measured points have been added. Staring simulated sequence.." << std::endl;

    // Simulate sequence by adding each measured point
    int N;
    for (int i = 0; i < mpoints.size(); i++) {
        N = st.add_measurement_point(mpoints[i]);
        TEST_COUT << "Recorded new measured point (total: " << N << ")." << std::endl;
        if (i >= num_cal_pts) {
            st.processPoints();
        }
        // Check: desired sequence is 0,1,2,...,N
        if (i >= num_cal_pts) {
            // Desired order is 0,1,2,...,N
            EXPECT_EQ(st.get_desired_order(i - num_cal_pts), i - num_cal_pts);
            if (i == mpoints.size() - 1) {
                // Actual order should be the same as desired except for points 9 and 10
                for (int j = 0; j < N; j++) {
                    if (j != 9 && j != 10)
                        EXPECT_EQ(st.get_actual_order(j), st.get_desired_order(j));
                    else if (j == 9)
                        EXPECT_EQ(st.get_actual_order(j), 10);
                    else if (j == 10)
                        EXPECT_EQ(st.get_actual_order(j), 9);
                }
            }
        }
    }
}

// FIXME:  These tests utilize bad MEASURED DATA, resulting in their failure.  These should be updated with good data.
TEST(StarTrack_Tests, Paper_Test) {
    std::vector<Vector3d> tpoints;
    std::vector<Vector3d> mpoints;
    Vector3d point;
    int num_cal_pts = 0;
    int num_ini_pts = 0;

    StarTrackBase st(num_cal_pts, num_ini_pts);

    TEST_COUT << "Loading truth points." << std::endl;
    // Side A
    point <<  0.00,  0.00,  0.00;   tpoints.push_back(point);
    point << -0.14,  0.11,  0.00;   tpoints.push_back(point);
    point << -0.14, -0.11,  0.00;   tpoints.push_back(point);
    point <<  0.14, -0.11,  0.00;   tpoints.push_back(point);
    point <<  0.14,  0.11,  0.00;   tpoints.push_back(point);

    for (int i = 0; i < tpoints.size(); i++) {
        st.add_truth_point(tpoints[i]);
    }

    TEST_COUT << "Loading measured points." << std::endl;
    // Measured points
    point << -1804336.64, -4496337.32, 4136331.12;    mpoints.push_back(point);
    point << -1804336.56, -4496337.36, 4136331.09;    mpoints.push_back(point);
    point << -1804336.54, -4496337.31, 4136331.18;    mpoints.push_back(point);
    point << -1804336.61, -4496337.36, 4136331.09;    mpoints.push_back(point);
    point << -1804336.62, -4496337.40, 4136331.04;    mpoints.push_back(point);

    TEST_COUT << mpoints.size() << " measured points have been added. Starting simulated sequence.." << std::endl;

    // Simulate sequence by adding each measured point
    int N;
    for (int i = 0; i < mpoints.size(); i++) {
        N = st.add_measurement_point(mpoints[i]);
        TEST_COUT << "Recorded new measured point (total: " << N << ")." << std::endl;
        if (i >= num_cal_pts) {
            st.processPoints();
        }
        // Check: desired sequence is 0,1,2,...,N
        if (i >= num_cal_pts) {
            // Desired order is 0,1,2,...,N
            EXPECT_EQ(st.get_desired_order(i - num_cal_pts), i - num_cal_pts);
            if (i == mpoints.size() - 1) {
                // Actual order should be the same as desired except for points 9 and 10
                for (int j = 0; j < N; j++) {
                    EXPECT_EQ(st.get_actual_order(j), st.get_desired_order(j));
                }
            }
        }
    }
}

// FIXME:  These tests utilize bad MEASURED DATA, resulting in their failure.  These should be updated with good data.
TEST(StarTrack_Tests, Paper_Test_No_Center) {
    std::vector<Vector3d> tpoints;
    std::vector<Vector3d> mpoints;
    Vector3d point;
    int num_cal_pts = 0;
    int num_ini_pts = 0;

    StarTrackBase st(num_cal_pts, num_ini_pts);

    TEST_COUT << "Loading truth points." << std::endl;
    // Side A
    point <<  0.00,  0.00,  0.00;   tpoints.push_back(point);
    point <<  0.00, -0.22,  0.00;   tpoints.push_back(point);
    point <<  0.28, -0.22,  0.00;   tpoints.push_back(point);
    point <<  0.28,  0.00,  0.00;   tpoints.push_back(point);

    for (int i = 0; i < tpoints.size(); i++) {
        st.add_truth_point(tpoints[i]);
    }

    TEST_COUT << "Loading measured points." << std::endl;
    // Measured points
    point << -1804336.56, -4496337.36, 4136331.09;    mpoints.push_back(point);
    point << -1804336.54, -4496337.31, 4136331.18;    mpoints.push_back(point);
    point << -1804336.61, -4496337.36, 4136331.09;    mpoints.push_back(point);
    point << -1804336.62, -4496337.40, 4136331.04;    mpoints.push_back(point);

    std::cout << std::cout.width(7) << std::fixed << std::setprecision(4);

    TEST_COUT << mpoints.size() << " measured points have been added. Starting simulated sequence.." << std::endl;

    // Simulate sequence by adding each measured point
    int N;
    for (int i = 0; i < mpoints.size(); i++) {
        N = st.add_measurement_point(mpoints[i]);
        TEST_COUT << "Recorded new measured point (total: " << N << ")." << std::endl;
        if (i >= num_cal_pts) {
            auto dist = st.get_measured_distance(i, i-1);
            std::cout << "Measured distance: " << dist << std::endl;
            st.processPoints();
        }
        // Check: desired sequence is 0,1,2,...,N
        if (i >= num_cal_pts) {
            // Desired order is 0,1,2,...,N
            EXPECT_EQ(st.get_desired_order(i - num_cal_pts), i - num_cal_pts);
            if (i == mpoints.size() - 1) {
                // Actual order should be the same as desired except for points 9 and 10
                for (int j = 0; j < N; j++) {
                    EXPECT_EQ(st.get_actual_order(j), st.get_desired_order(j));
                }
            }
        }
    }
}