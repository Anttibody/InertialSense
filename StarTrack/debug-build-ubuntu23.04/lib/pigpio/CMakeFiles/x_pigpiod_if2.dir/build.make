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
include lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/progress.make

# Include the compile flags for this target's objects.
include lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/flags.make

lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/x_pigpiod_if2.c.o: lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/flags.make
lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/x_pigpiod_if2.c.o: /home/ubuntu/StarTrack/lib/pigpio/x_pigpiod_if2.c
lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/x_pigpiod_if2.c.o: lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/StarTrack/debug-build-ubuntu23.04/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/x_pigpiod_if2.c.o"
	cd /home/ubuntu/StarTrack/debug-build-ubuntu23.04/lib/pigpio && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/x_pigpiod_if2.c.o -MF CMakeFiles/x_pigpiod_if2.dir/x_pigpiod_if2.c.o.d -o CMakeFiles/x_pigpiod_if2.dir/x_pigpiod_if2.c.o -c /home/ubuntu/StarTrack/lib/pigpio/x_pigpiod_if2.c

lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/x_pigpiod_if2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/x_pigpiod_if2.dir/x_pigpiod_if2.c.i"
	cd /home/ubuntu/StarTrack/debug-build-ubuntu23.04/lib/pigpio && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ubuntu/StarTrack/lib/pigpio/x_pigpiod_if2.c > CMakeFiles/x_pigpiod_if2.dir/x_pigpiod_if2.c.i

lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/x_pigpiod_if2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/x_pigpiod_if2.dir/x_pigpiod_if2.c.s"
	cd /home/ubuntu/StarTrack/debug-build-ubuntu23.04/lib/pigpio && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ubuntu/StarTrack/lib/pigpio/x_pigpiod_if2.c -o CMakeFiles/x_pigpiod_if2.dir/x_pigpiod_if2.c.s

# Object files for target x_pigpiod_if2
x_pigpiod_if2_OBJECTS = \
"CMakeFiles/x_pigpiod_if2.dir/x_pigpiod_if2.c.o"

# External object files for target x_pigpiod_if2
x_pigpiod_if2_EXTERNAL_OBJECTS =

lib/pigpio/x_pigpiod_if2: lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/x_pigpiod_if2.c.o
lib/pigpio/x_pigpiod_if2: lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/build.make
lib/pigpio/x_pigpiod_if2: lib/pigpio/libpigpiod_if2.so
lib/pigpio/x_pigpiod_if2: /usr/lib/aarch64-linux-gnu/librt.a
lib/pigpio/x_pigpiod_if2: lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu/StarTrack/debug-build-ubuntu23.04/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable x_pigpiod_if2"
	cd /home/ubuntu/StarTrack/debug-build-ubuntu23.04/lib/pigpio && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/x_pigpiod_if2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/build: lib/pigpio/x_pigpiod_if2
.PHONY : lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/build

lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/clean:
	cd /home/ubuntu/StarTrack/debug-build-ubuntu23.04/lib/pigpio && $(CMAKE_COMMAND) -P CMakeFiles/x_pigpiod_if2.dir/cmake_clean.cmake
.PHONY : lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/clean

lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/depend:
	cd /home/ubuntu/StarTrack/debug-build-ubuntu23.04 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/StarTrack /home/ubuntu/StarTrack/lib/pigpio /home/ubuntu/StarTrack/debug-build-ubuntu23.04 /home/ubuntu/StarTrack/debug-build-ubuntu23.04/lib/pigpio /home/ubuntu/StarTrack/debug-build-ubuntu23.04/lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/pigpio/CMakeFiles/x_pigpiod_if2.dir/depend
