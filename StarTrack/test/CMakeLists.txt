
enable_testing()

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

set(TEST_FILES test/StarTrack_tests.cpp src/StarTrackBase.cpp)

include_directories(include lib/inertial-sense-sdk/src lib/pigpio /usr/include/libusb-1.0 /usr/include/eigen3)
add_executable( StarTrack_tests ${TEST_FILES} )
target_link_libraries( StarTrack_tests GTest::gtest_main fmt Eigen3::Eigen rt pthread )

include(GoogleTest)
gtest_discover_tests(StarTrack_tests)
