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
CMAKE_SOURCE_DIR = /home/startrack/StarTrack

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/startrack/StarTrack/debug-build-startrack

# Include any dependencies generated for this target.
include lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/progress.make

# Include the compile flags for this target's objects.
include lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/flags.make

lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/ISBootloaderExample.cpp.o: lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/flags.make
lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/ISBootloaderExample.cpp.o: /home/startrack/StarTrack/lib/inertial-sense-sdk/ExampleProjects/Bootloader/ISBootloaderExample.cpp
lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/ISBootloaderExample.cpp.o: lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/startrack/StarTrack/debug-build-startrack/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/ISBootloaderExample.cpp.o"
	cd /home/startrack/StarTrack/debug-build-startrack/lib/inertial-sense-sdk/ExampleProjects/Bootloader && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/ISBootloaderExample.cpp.o -MF CMakeFiles/ISBootloaderExample.dir/ISBootloaderExample.cpp.o.d -o CMakeFiles/ISBootloaderExample.dir/ISBootloaderExample.cpp.o -c /home/startrack/StarTrack/lib/inertial-sense-sdk/ExampleProjects/Bootloader/ISBootloaderExample.cpp

lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/ISBootloaderExample.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ISBootloaderExample.dir/ISBootloaderExample.cpp.i"
	cd /home/startrack/StarTrack/debug-build-startrack/lib/inertial-sense-sdk/ExampleProjects/Bootloader && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/startrack/StarTrack/lib/inertial-sense-sdk/ExampleProjects/Bootloader/ISBootloaderExample.cpp > CMakeFiles/ISBootloaderExample.dir/ISBootloaderExample.cpp.i

lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/ISBootloaderExample.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ISBootloaderExample.dir/ISBootloaderExample.cpp.s"
	cd /home/startrack/StarTrack/debug-build-startrack/lib/inertial-sense-sdk/ExampleProjects/Bootloader && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/startrack/StarTrack/lib/inertial-sense-sdk/ExampleProjects/Bootloader/ISBootloaderExample.cpp -o CMakeFiles/ISBootloaderExample.dir/ISBootloaderExample.cpp.s

# Object files for target ISBootloaderExample
ISBootloaderExample_OBJECTS = \
"CMakeFiles/ISBootloaderExample.dir/ISBootloaderExample.cpp.o"

# External object files for target ISBootloaderExample
ISBootloaderExample_EXTERNAL_OBJECTS =

lib/inertial-sense-sdk/ExampleProjects/Bootloader/ISBootloaderExample: lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/ISBootloaderExample.cpp.o
lib/inertial-sense-sdk/ExampleProjects/Bootloader/ISBootloaderExample: lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/build.make
lib/inertial-sense-sdk/ExampleProjects/Bootloader/ISBootloaderExample: lib/inertial-sense-sdk/libInertialSense.so
lib/inertial-sense-sdk/ExampleProjects/Bootloader/ISBootloaderExample: /usr/lib/aarch64-linux-gnu/libusb-1.0.so
lib/inertial-sense-sdk/ExampleProjects/Bootloader/ISBootloaderExample: /usr/lib/aarch64-linux-gnu/libyaml-cpp.so.0.7.0
lib/inertial-sense-sdk/ExampleProjects/Bootloader/ISBootloaderExample: lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/startrack/StarTrack/debug-build-startrack/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ISBootloaderExample"
	cd /home/startrack/StarTrack/debug-build-startrack/lib/inertial-sense-sdk/ExampleProjects/Bootloader && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ISBootloaderExample.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/build: lib/inertial-sense-sdk/ExampleProjects/Bootloader/ISBootloaderExample
.PHONY : lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/build

lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/clean:
	cd /home/startrack/StarTrack/debug-build-startrack/lib/inertial-sense-sdk/ExampleProjects/Bootloader && $(CMAKE_COMMAND) -P CMakeFiles/ISBootloaderExample.dir/cmake_clean.cmake
.PHONY : lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/clean

lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/depend:
	cd /home/startrack/StarTrack/debug-build-startrack && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/startrack/StarTrack /home/startrack/StarTrack/lib/inertial-sense-sdk/ExampleProjects/Bootloader /home/startrack/StarTrack/debug-build-startrack /home/startrack/StarTrack/debug-build-startrack/lib/inertial-sense-sdk/ExampleProjects/Bootloader /home/startrack/StarTrack/debug-build-startrack/lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/inertial-sense-sdk/ExampleProjects/Bootloader/CMakeFiles/ISBootloaderExample.dir/depend

