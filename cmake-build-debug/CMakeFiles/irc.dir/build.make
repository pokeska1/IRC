# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/jmarian/Desktop/21/5_lvl/IRC_git

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jmarian/Desktop/21/5_lvl/IRC_git/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/irc.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/irc.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/irc.dir/flags.make

CMakeFiles/irc.dir/main.cpp.o: CMakeFiles/irc.dir/flags.make
CMakeFiles/irc.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jmarian/Desktop/21/5_lvl/IRC_git/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/irc.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/irc.dir/main.cpp.o -c /Users/jmarian/Desktop/21/5_lvl/IRC_git/main.cpp

CMakeFiles/irc.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/irc.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jmarian/Desktop/21/5_lvl/IRC_git/main.cpp > CMakeFiles/irc.dir/main.cpp.i

CMakeFiles/irc.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/irc.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jmarian/Desktop/21/5_lvl/IRC_git/main.cpp -o CMakeFiles/irc.dir/main.cpp.s

CMakeFiles/irc.dir/Client.cpp.o: CMakeFiles/irc.dir/flags.make
CMakeFiles/irc.dir/Client.cpp.o: ../Client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jmarian/Desktop/21/5_lvl/IRC_git/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/irc.dir/Client.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/irc.dir/Client.cpp.o -c /Users/jmarian/Desktop/21/5_lvl/IRC_git/Client.cpp

CMakeFiles/irc.dir/Client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/irc.dir/Client.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jmarian/Desktop/21/5_lvl/IRC_git/Client.cpp > CMakeFiles/irc.dir/Client.cpp.i

CMakeFiles/irc.dir/Client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/irc.dir/Client.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jmarian/Desktop/21/5_lvl/IRC_git/Client.cpp -o CMakeFiles/irc.dir/Client.cpp.s

CMakeFiles/irc.dir/Server.cpp.o: CMakeFiles/irc.dir/flags.make
CMakeFiles/irc.dir/Server.cpp.o: ../Server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jmarian/Desktop/21/5_lvl/IRC_git/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/irc.dir/Server.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/irc.dir/Server.cpp.o -c /Users/jmarian/Desktop/21/5_lvl/IRC_git/Server.cpp

CMakeFiles/irc.dir/Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/irc.dir/Server.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jmarian/Desktop/21/5_lvl/IRC_git/Server.cpp > CMakeFiles/irc.dir/Server.cpp.i

CMakeFiles/irc.dir/Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/irc.dir/Server.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jmarian/Desktop/21/5_lvl/IRC_git/Server.cpp -o CMakeFiles/irc.dir/Server.cpp.s

# Object files for target irc
irc_OBJECTS = \
"CMakeFiles/irc.dir/main.cpp.o" \
"CMakeFiles/irc.dir/Client.cpp.o" \
"CMakeFiles/irc.dir/Server.cpp.o"

# External object files for target irc
irc_EXTERNAL_OBJECTS =

irc: CMakeFiles/irc.dir/main.cpp.o
irc: CMakeFiles/irc.dir/Client.cpp.o
irc: CMakeFiles/irc.dir/Server.cpp.o
irc: CMakeFiles/irc.dir/build.make
irc: CMakeFiles/irc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/jmarian/Desktop/21/5_lvl/IRC_git/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable irc"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/irc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/irc.dir/build: irc
.PHONY : CMakeFiles/irc.dir/build

CMakeFiles/irc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/irc.dir/cmake_clean.cmake
.PHONY : CMakeFiles/irc.dir/clean

CMakeFiles/irc.dir/depend:
	cd /Users/jmarian/Desktop/21/5_lvl/IRC_git/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jmarian/Desktop/21/5_lvl/IRC_git /Users/jmarian/Desktop/21/5_lvl/IRC_git /Users/jmarian/Desktop/21/5_lvl/IRC_git/cmake-build-debug /Users/jmarian/Desktop/21/5_lvl/IRC_git/cmake-build-debug /Users/jmarian/Desktop/21/5_lvl/IRC_git/cmake-build-debug/CMakeFiles/irc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/irc.dir/depend

