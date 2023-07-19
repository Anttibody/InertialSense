# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ubuntu/StarTrack

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/StarTrack/debug-build-ubuntu23.04

# Include any dependencies generated for this target.
include lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/progress.make

# Include the compile flags for this target's objects.
include lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/flags.make

lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/main.cpp.o: lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/flags.make
lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/main.cpp.o: /home/ubuntu/StarTrack/lib/inertial-sense-sdk/cltool/main.cpp
lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/main.cpp.o: lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/StarTrack/debug-build-ubuntu23.04/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/main.cpp.o"
	cd /home/ubuntu/StarTrack/debug-build-ubuntu23.04/lib/inertial-sense-sdk/cltool && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/main.cpp.o -MF CMakeFiles/cltool.dir/main.cpp.o.d -o CMakeFiles/cltool.dir/main.cpp.o -c /home/ubuntu/StarTrack/lib/inertial-sense-sdk/cltool/main.cpp

lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cltool.dir/main.cpp.i"
	cd /home/ubuntu/StarTrack/debug-build-ubuntu23.04/lib/inertial-sense-sdk/cltool && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/StarTrack/lib/inertial-sense-sdk/cltool/main.cpp > CMakeFiles/cltool.dir/main.cpp.i

lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cltool.dir/main.cpp.s"
	cd /home/ubuntu/StarTrack/debug-build-ubuntu23.04/lib/inertial-sense-sdk/cltool && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/StarTrack/lib/inertial-sense-sdk/cltool/main.cpp -o CMakeFiles/cltool.dir/main.cpp.s

# Object files for target cltool
cltool_OBJECTS = \
"CMakeFiles/cltool.dir/main.cpp.o"

# External object files for target cltool
cltool_EXTERNAL_OBJECTS =

lib/inertial-sense-sdk/cltool/cltool: lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/main.cpp.o
lib/inertial-sense-sdk/cltool/cltool: lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/build.make
lib/inertial-sense-sdk/cltool/cltool: lib/inertial-sense-sdk/libInertialSense.so
lib/inertial-sense-sdk/cltool/cltool: /usr/lib/aarch64-linux-gnu/libusb-1.0.so
lib/inertial-sense-sdk/cltool/cltool: /usr/lib/aarch64-linux-gnu/libyaml-cpp.so.0.7.0
lib/inertial-sense-sdk/cltool/cltool: lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu/StarTrack/debug-build-ubuntu23.04/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable cltool"
	cd /home/ubuntu/StarTrack/debug-build-ubuntu23.04/lib/inertial-sense-sdk/cltool && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cltool.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/build: lib/inertial-sense-sdk/cltool/cltool
.PHONY : lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/build

lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/clean:
	cd /home/ubuntu/StarTrack/debug-build-ubuntu23.04/lib/inertial-sense-sdk/cltool && $(CMAKE_COMMAND) -P CMakeFiles/cltool.dir/cmake_clean.cmake
.PHONY : lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/clean

lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/depend:
	cd /home/ubuntu/StarTrack/debug-build-ubuntu23.04 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/StarTrack /home/ubuntu/StarTrack/lib/inertial-sense-sdk/cltool /home/ubuntu/StarTrack/debug-build-ubuntu23.04 /home/ubuntu/StarTrack/debug-build-ubuntu23.04/lib/inertial-sense-sdk/cltool /home/ubuntu/StarTrack/debug-build-ubuntu23.04/lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/inertial-sense-sdk/cltool/CMakeFiles/cltool.dir/depend
