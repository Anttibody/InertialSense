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
include CMakeFiles/StarTrack.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/StarTrack.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/StarTrack.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/StarTrack.dir/flags.make

CMakeFiles/StarTrack.dir/src/main.cpp.o: CMakeFiles/StarTrack.dir/flags.make
CMakeFiles/StarTrack.dir/src/main.cpp.o: /home/startrack/StarTrack/src/main.cpp
CMakeFiles/StarTrack.dir/src/main.cpp.o: CMakeFiles/StarTrack.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/startrack/StarTrack/debug-build-startrack/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/StarTrack.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/StarTrack.dir/src/main.cpp.o -MF CMakeFiles/StarTrack.dir/src/main.cpp.o.d -o CMakeFiles/StarTrack.dir/src/main.cpp.o -c /home/startrack/StarTrack/src/main.cpp

CMakeFiles/StarTrack.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/StarTrack.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/startrack/StarTrack/src/main.cpp > CMakeFiles/StarTrack.dir/src/main.cpp.i

CMakeFiles/StarTrack.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/StarTrack.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/startrack/StarTrack/src/main.cpp -o CMakeFiles/StarTrack.dir/src/main.cpp.s

CMakeFiles/StarTrack.dir/src/StarTrack.cpp.o: CMakeFiles/StarTrack.dir/flags.make
CMakeFiles/StarTrack.dir/src/StarTrack.cpp.o: /home/startrack/StarTrack/src/StarTrack.cpp
CMakeFiles/StarTrack.dir/src/StarTrack.cpp.o: CMakeFiles/StarTrack.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/startrack/StarTrack/debug-build-startrack/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/StarTrack.dir/src/StarTrack.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/StarTrack.dir/src/StarTrack.cpp.o -MF CMakeFiles/StarTrack.dir/src/StarTrack.cpp.o.d -o CMakeFiles/StarTrack.dir/src/StarTrack.cpp.o -c /home/startrack/StarTrack/src/StarTrack.cpp

CMakeFiles/StarTrack.dir/src/StarTrack.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/StarTrack.dir/src/StarTrack.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/startrack/StarTrack/src/StarTrack.cpp > CMakeFiles/StarTrack.dir/src/StarTrack.cpp.i

CMakeFiles/StarTrack.dir/src/StarTrack.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/StarTrack.dir/src/StarTrack.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/startrack/StarTrack/src/StarTrack.cpp -o CMakeFiles/StarTrack.dir/src/StarTrack.cpp.s

CMakeFiles/StarTrack.dir/src/Blinky.cpp.o: CMakeFiles/StarTrack.dir/flags.make
CMakeFiles/StarTrack.dir/src/Blinky.cpp.o: /home/startrack/StarTrack/src/Blinky.cpp
CMakeFiles/StarTrack.dir/src/Blinky.cpp.o: CMakeFiles/StarTrack.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/startrack/StarTrack/debug-build-startrack/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/StarTrack.dir/src/Blinky.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/StarTrack.dir/src/Blinky.cpp.o -MF CMakeFiles/StarTrack.dir/src/Blinky.cpp.o.d -o CMakeFiles/StarTrack.dir/src/Blinky.cpp.o -c /home/startrack/StarTrack/src/Blinky.cpp

CMakeFiles/StarTrack.dir/src/Blinky.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/StarTrack.dir/src/Blinky.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/startrack/StarTrack/src/Blinky.cpp > CMakeFiles/StarTrack.dir/src/Blinky.cpp.i

CMakeFiles/StarTrack.dir/src/Blinky.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/StarTrack.dir/src/Blinky.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/startrack/StarTrack/src/Blinky.cpp -o CMakeFiles/StarTrack.dir/src/Blinky.cpp.s

CMakeFiles/StarTrack.dir/src/StarTrackBase.cpp.o: CMakeFiles/StarTrack.dir/flags.make
CMakeFiles/StarTrack.dir/src/StarTrackBase.cpp.o: /home/startrack/StarTrack/src/StarTrackBase.cpp
CMakeFiles/StarTrack.dir/src/StarTrackBase.cpp.o: CMakeFiles/StarTrack.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/startrack/StarTrack/debug-build-startrack/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/StarTrack.dir/src/StarTrackBase.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/StarTrack.dir/src/StarTrackBase.cpp.o -MF CMakeFiles/StarTrack.dir/src/StarTrackBase.cpp.o.d -o CMakeFiles/StarTrack.dir/src/StarTrackBase.cpp.o -c /home/startrack/StarTrack/src/StarTrackBase.cpp

CMakeFiles/StarTrack.dir/src/StarTrackBase.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/StarTrack.dir/src/StarTrackBase.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/startrack/StarTrack/src/StarTrackBase.cpp > CMakeFiles/StarTrack.dir/src/StarTrackBase.cpp.i

CMakeFiles/StarTrack.dir/src/StarTrackBase.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/StarTrack.dir/src/StarTrackBase.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/startrack/StarTrack/src/StarTrackBase.cpp -o CMakeFiles/StarTrack.dir/src/StarTrackBase.cpp.s

# Object files for target StarTrack
StarTrack_OBJECTS = \
"CMakeFiles/StarTrack.dir/src/main.cpp.o" \
"CMakeFiles/StarTrack.dir/src/StarTrack.cpp.o" \
"CMakeFiles/StarTrack.dir/src/Blinky.cpp.o" \
"CMakeFiles/StarTrack.dir/src/StarTrackBase.cpp.o"

# External object files for target StarTrack
StarTrack_EXTERNAL_OBJECTS =

StarTrack: CMakeFiles/StarTrack.dir/src/main.cpp.o
StarTrack: CMakeFiles/StarTrack.dir/src/StarTrack.cpp.o
StarTrack: CMakeFiles/StarTrack.dir/src/Blinky.cpp.o
StarTrack: CMakeFiles/StarTrack.dir/src/StarTrackBase.cpp.o
StarTrack: CMakeFiles/StarTrack.dir/build.make
StarTrack: lib/inertial-sense-sdk/libInertialSense.so
StarTrack: lib/pigpio/libpigpiod_if2.so
StarTrack: /usr/lib/aarch64-linux-gnu/libspdlog.so.1.10.0
StarTrack: /usr/lib/aarch64-linux-gnu/librt.a
StarTrack: /usr/lib/aarch64-linux-gnu/libm.so
StarTrack: /usr/lib/aarch64-linux-gnu/libusb-1.0.so
StarTrack: /usr/lib/aarch64-linux-gnu/libyaml-cpp.so.0.7.0
StarTrack: /usr/lib/aarch64-linux-gnu/libfmt.so.9.1.0
StarTrack: CMakeFiles/StarTrack.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/startrack/StarTrack/debug-build-startrack/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable StarTrack"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/StarTrack.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/StarTrack.dir/build: StarTrack
.PHONY : CMakeFiles/StarTrack.dir/build

CMakeFiles/StarTrack.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/StarTrack.dir/cmake_clean.cmake
.PHONY : CMakeFiles/StarTrack.dir/clean

CMakeFiles/StarTrack.dir/depend:
	cd /home/startrack/StarTrack/debug-build-startrack && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/startrack/StarTrack /home/startrack/StarTrack /home/startrack/StarTrack/debug-build-startrack /home/startrack/StarTrack/debug-build-startrack /home/startrack/StarTrack/debug-build-startrack/CMakeFiles/StarTrack.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/StarTrack.dir/depend
