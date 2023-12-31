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
include lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/progress.make

# Include the compile flags for this target's objects.
include lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/flags.make

lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/ISCommunicationsExample.c.o: lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/flags.make
lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/ISCommunicationsExample.c.o: /home/ubuntu/StarTrack/lib/inertial-sense-sdk/ExampleProjects/Communications/ISCommunicationsExample.c
lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/ISCommunicationsExample.c.o: lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/StarTrack/debug-build-ubuntu23.04/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/ISCommunicationsExample.c.o"
	cd /home/ubuntu/StarTrack/debug-build-ubuntu23.04/lib/inertial-sense-sdk/ExampleProjects/Communications && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/ISCommunicationsExample.c.o -MF CMakeFiles/ISCommunicationsExample.dir/ISCommunicationsExample.c.o.d -o CMakeFiles/ISCommunicationsExample.dir/ISCommunicationsExample.c.o -c /home/ubuntu/StarTrack/lib/inertial-sense-sdk/ExampleProjects/Communications/ISCommunicationsExample.c

lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/ISCommunicationsExample.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ISCommunicationsExample.dir/ISCommunicationsExample.c.i"
	cd /home/ubuntu/StarTrack/debug-build-ubuntu23.04/lib/inertial-sense-sdk/ExampleProjects/Communications && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ubuntu/StarTrack/lib/inertial-sense-sdk/ExampleProjects/Communications/ISCommunicationsExample.c > CMakeFiles/ISCommunicationsExample.dir/ISCommunicationsExample.c.i

lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/ISCommunicationsExample.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ISCommunicationsExample.dir/ISCommunicationsExample.c.s"
	cd /home/ubuntu/StarTrack/debug-build-ubuntu23.04/lib/inertial-sense-sdk/ExampleProjects/Communications && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ubuntu/StarTrack/lib/inertial-sense-sdk/ExampleProjects/Communications/ISCommunicationsExample.c -o CMakeFiles/ISCommunicationsExample.dir/ISCommunicationsExample.c.s

# Object files for target ISCommunicationsExample
ISCommunicationsExample_OBJECTS = \
"CMakeFiles/ISCommunicationsExample.dir/ISCommunicationsExample.c.o"

# External object files for target ISCommunicationsExample
ISCommunicationsExample_EXTERNAL_OBJECTS =

lib/inertial-sense-sdk/ExampleProjects/Communications/ISCommunicationsExample: lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/ISCommunicationsExample.c.o
lib/inertial-sense-sdk/ExampleProjects/Communications/ISCommunicationsExample: lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/build.make
lib/inertial-sense-sdk/ExampleProjects/Communications/ISCommunicationsExample: lib/inertial-sense-sdk/libInertialSense.so
lib/inertial-sense-sdk/ExampleProjects/Communications/ISCommunicationsExample: /usr/lib/aarch64-linux-gnu/libusb-1.0.so
lib/inertial-sense-sdk/ExampleProjects/Communications/ISCommunicationsExample: /usr/lib/aarch64-linux-gnu/libyaml-cpp.so.0.7.0
lib/inertial-sense-sdk/ExampleProjects/Communications/ISCommunicationsExample: lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu/StarTrack/debug-build-ubuntu23.04/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ISCommunicationsExample"
	cd /home/ubuntu/StarTrack/debug-build-ubuntu23.04/lib/inertial-sense-sdk/ExampleProjects/Communications && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ISCommunicationsExample.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/build: lib/inertial-sense-sdk/ExampleProjects/Communications/ISCommunicationsExample
.PHONY : lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/build

lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/clean:
	cd /home/ubuntu/StarTrack/debug-build-ubuntu23.04/lib/inertial-sense-sdk/ExampleProjects/Communications && $(CMAKE_COMMAND) -P CMakeFiles/ISCommunicationsExample.dir/cmake_clean.cmake
.PHONY : lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/clean

lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/depend:
	cd /home/ubuntu/StarTrack/debug-build-ubuntu23.04 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/StarTrack /home/ubuntu/StarTrack/lib/inertial-sense-sdk/ExampleProjects/Communications /home/ubuntu/StarTrack/debug-build-ubuntu23.04 /home/ubuntu/StarTrack/debug-build-ubuntu23.04/lib/inertial-sense-sdk/ExampleProjects/Communications /home/ubuntu/StarTrack/debug-build-ubuntu23.04/lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/inertial-sense-sdk/ExampleProjects/Communications/CMakeFiles/ISCommunicationsExample.dir/depend

