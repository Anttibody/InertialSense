cmake_minimum_required(VERSION 3.15)

project(StarTrack)

set(CMAKE_CXX_STANDARD 17)

if (UNIX AND NOT APPLE)
    find_library(RT_LIBRARY rt)
    mark_as_advanced(RT_LIBRARY)
    if (RT_LIBRARY)
        list(APPEND StarTrack_LIBRARIES "${RT_LIBRARY}")
        list(APPEND StarTrack_PKG_LIBS "-lrt")
    endif()
    find_library(MATH_LIBRARY m)
    mark_as_advanced(MATH_LIBRARY)
    if (MATH_LIBRARY)
        list(APPEND StarTrack_LIBRARIES "${MATH_LIBRARY}")
        list(APPEND StarTrack_LIBS "-lm")
    endif()
    find_library(FS_LIBRARY stdc++fs)
    mark_as_advanced(FS_LIBRARY)
    if (FS_LIBRARY)
        list(APPEND StarTrack_LIBRARIES "${FS_LIBRARY}")
        list(APPEND StarTrack_PKG_LIBS "-lstdc++fs")
    endif()
endif()

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

find_package(spdlog QUIET)
if(NOT TARGET spdlog::spdlog)
    add_subdirectory(lib/spdlog)
    list(APPEND StarTrack_LIBRARIES "spdlog")

    find_package(fmt QUIET)
    if(NOT TARGET fmt::fmt)
        add_subdirectory(lib/fmt)
        set_target_properties(fmt PROPERTIES fmt fmt)
        list(APPEND StarTrack_LIBRARIES "fmt")
    endif()
endif()

add_subdirectory(lib/inertial-sense-sdk)
add_subdirectory(lib/pigpio)

include_directories(include lib/inertial-sense-sdk/src lib/pigpio)
set(SOURCE_FILES src/main.cpp src/StarTrack.cpp src/Blinky.cpp src/StarTrackBase.cpp )
add_executable(StarTrack ${SOURCE_FILES})
target_link_libraries (StarTrack PRIVATE $<$<BOOL:${MINGW}>:ws2_32> InertialSense pigpiod_if2 fmt spdlog::spdlog Eigen3::Eigen Threads::Threads ${StarTrack_LIBRARIES} )

include(test/CMakeLists.txt)

install(TARGETS StarTrack RUNTIME)
install(FILES StarTrack.yaml DESTINATION /etc)
install(DIRECTORY DESTINATION /var/log/StarTrack DIRECTORY_PERMISSIONS
        OWNER_WRITE OWNER_READ OWNER_EXECUTE
        GROUP_WRITE GROUP_READ GROUP_EXECUTE
        WORLD_WRITE WORLD_READ WORLD_EXECUTE)