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
include lib/pigpio/CMakeFiles/pigpio.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lib/pigpio/CMakeFiles/pigpio.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/pigpio/CMakeFiles/pigpio.dir/progress.make

# Include the compile flags for this target's objects.
include lib/pigpio/CMakeFiles/pigpio.dir/flags.make

lib/pigpio/CMakeFiles/pigpio.dir/pigpio.c.o: lib/pigpio/CMakeFiles/pigpio.dir/flags.make
lib/pigpio/CMakeFiles/pigpio.dir/pigpio.c.o: /home/startrack/StarTrack/lib/pigpio/pigpio.c
lib/pigpio/CMakeFiles/pigpio.dir/pigpio.c.o: lib/pigpio/CMakeFiles/pigpio.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/startrack/StarTrack/debug-build-startrack/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object lib/pigpio/CMakeFiles/pigpio.dir/pigpio.c.o"
	cd /home/startrack/StarTrack/debug-build-startrack/lib/pigpio && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT lib/pigpio/CMakeFiles/pigpio.dir/pigpio.c.o -MF CMakeFiles/pigpio.dir/pigpio.c.o.d -o CMakeFiles/pigpio.dir/pigpio.c.o -c /home/startrack/StarTrack/lib/pigpio/pigpio.c

lib/pigpio/CMakeFiles/pigpio.dir/pigpio.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pigpio.dir/pigpio.c.i"
	cd /home/startrack/StarTrack/debug-build-startrack/lib/pigpio && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/startrack/StarTrack/lib/pigpio/pigpio.c > CMakeFiles/pigpio.dir/pigpio.c.i

lib/pigpio/CMakeFiles/pigpio.dir/pigpio.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pigpio.dir/pigpio.c.s"
	cd /home/startrack/StarTrack/debug-build-startrack/lib/pigpio && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/startrack/StarTrack/lib/pigpio/pigpio.c -o CMakeFiles/pigpio.dir/pigpio.c.s

lib/pigpio/CMakeFiles/pigpio.dir/command.c.o: lib/pigpio/CMakeFiles/pigpio.dir/flags.make
lib/pigpio/CMakeFiles/pigpio.dir/command.c.o: /home/startrack/StarTrack/lib/pigpio/command.c
lib/pigpio/CMakeFiles/pigpio.dir/command.c.o: lib/pigpio/CMakeFiles/pigpio.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/startrack/StarTrack/debug-build-startrack/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object lib/pigpio/CMakeFiles/pigpio.dir/command.c.o"
	cd /home/startrack/StarTrack/debug-build-startrack/lib/pigpio && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT lib/pigpio/CMakeFiles/pigpio.dir/command.c.o -MF CMakeFiles/pigpio.dir/command.c.o.d -o CMakeFiles/pigpio.dir/command.c.o -c /home/startrack/StarTrack/lib/pigpio/command.c

lib/pigpio/CMakeFiles/pigpio.dir/command.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pigpio.dir/command.c.i"
	cd /home/startrack/StarTrack/debug-build-startrack/lib/pigpio && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/startrack/StarTrack/lib/pigpio/command.c > CMakeFiles/pigpio.dir/command.c.i

lib/pigpio/CMakeFiles/pigpio.dir/command.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pigpio.dir/command.c.s"
	cd /home/startrack/StarTrack/debug-build-startrack/lib/pigpio && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/startrack/StarTrack/lib/pigpio/command.c -o CMakeFiles/pigpio.dir/command.c.s

# Object files for target pigpio
pigpio_OBJECTS = \
"CMakeFiles/pigpio.dir/pigpio.c.o" \
"CMakeFiles/pigpio.dir/command.c.o"

# External object files for target pigpio
pigpio_EXTERNAL_OBJECTS =

lib/pigpio/libpigpio.so: lib/pigpio/CMakeFiles/pigpio.dir/pigpio.c.o
lib/pigpio/libpigpio.so: lib/pigpio/CMakeFiles/pigpio.dir/command.c.o
lib/pigpio/libpigpio.so: lib/pigpio/CMakeFiles/pigpio.dir/build.make
lib/pigpio/libpigpio.so: lib/pigpio/CMakeFiles/pigpio.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/startrack/StarTrack/debug-build-startrack/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C shared library libpigpio.so"
	cd /home/startrack/StarTrack/debug-build-startrack/lib/pigpio && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pigpio.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/pigpio/CMakeFiles/pigpio.dir/build: lib/pigpio/libpigpio.so
.PHONY : lib/pigpio/CMakeFiles/pigpio.dir/build

lib/pigpio/CMakeFiles/pigpio.dir/clean:
	cd /home/startrack/StarTrack/debug-build-startrack/lib/pigpio && $(CMAKE_COMMAND) -P CMakeFiles/pigpio.dir/cmake_clean.cmake
.PHONY : lib/pigpio/CMakeFiles/pigpio.dir/clean

lib/pigpio/CMakeFiles/pigpio.dir/depend:
	cd /home/startrack/StarTrack/debug-build-startrack && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/startrack/StarTrack /home/startrack/StarTrack/lib/pigpio /home/startrack/StarTrack/debug-build-startrack /home/startrack/StarTrack/debug-build-startrack/lib/pigpio /home/startrack/StarTrack/debug-build-startrack/lib/pigpio/CMakeFiles/pigpio.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/pigpio/CMakeFiles/pigpio.dir/depend

