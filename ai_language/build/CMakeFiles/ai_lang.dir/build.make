# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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
CMAKE_COMMAND = /nix/store/q1nssraba326p2kp6627hldd2bhg254c-cmake-3.29.2/bin/cmake

# The command to remove a file.
RM = /nix/store/q1nssraba326p2kp6627hldd2bhg254c-cmake-3.29.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/runner/workspace/ai_language

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/runner/workspace/ai_language/build

# Include any dependencies generated for this target.
include CMakeFiles/ai_lang.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ai_lang.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ai_lang.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ai_lang.dir/flags.make

CMakeFiles/ai_lang.dir/src/ai_lang.cpp.o: CMakeFiles/ai_lang.dir/flags.make
CMakeFiles/ai_lang.dir/src/ai_lang.cpp.o: /home/runner/workspace/ai_language/src/ai_lang.cpp
CMakeFiles/ai_lang.dir/src/ai_lang.cpp.o: CMakeFiles/ai_lang.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/runner/workspace/ai_language/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ai_lang.dir/src/ai_lang.cpp.o"
	/nix/store/9bv7dcvmfcjnmg5mnqwqlq2wxfn8d7yi-gcc-wrapper-13.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ai_lang.dir/src/ai_lang.cpp.o -MF CMakeFiles/ai_lang.dir/src/ai_lang.cpp.o.d -o CMakeFiles/ai_lang.dir/src/ai_lang.cpp.o -c /home/runner/workspace/ai_language/src/ai_lang.cpp

CMakeFiles/ai_lang.dir/src/ai_lang.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ai_lang.dir/src/ai_lang.cpp.i"
	/nix/store/9bv7dcvmfcjnmg5mnqwqlq2wxfn8d7yi-gcc-wrapper-13.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/workspace/ai_language/src/ai_lang.cpp > CMakeFiles/ai_lang.dir/src/ai_lang.cpp.i

CMakeFiles/ai_lang.dir/src/ai_lang.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ai_lang.dir/src/ai_lang.cpp.s"
	/nix/store/9bv7dcvmfcjnmg5mnqwqlq2wxfn8d7yi-gcc-wrapper-13.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/workspace/ai_language/src/ai_lang.cpp -o CMakeFiles/ai_lang.dir/src/ai_lang.cpp.s

# Object files for target ai_lang
ai_lang_OBJECTS = \
"CMakeFiles/ai_lang.dir/src/ai_lang.cpp.o"

# External object files for target ai_lang
ai_lang_EXTERNAL_OBJECTS =

/home/runner/workspace/ai_language/ai_lang: CMakeFiles/ai_lang.dir/src/ai_lang.cpp.o
/home/runner/workspace/ai_language/ai_lang: CMakeFiles/ai_lang.dir/build.make
/home/runner/workspace/ai_language/ai_lang: libai_language_lib.a
/home/runner/workspace/ai_language/ai_lang: CMakeFiles/ai_lang.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/runner/workspace/ai_language/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/runner/workspace/ai_language/ai_lang"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ai_lang.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ai_lang.dir/build: /home/runner/workspace/ai_language/ai_lang
.PHONY : CMakeFiles/ai_lang.dir/build

CMakeFiles/ai_lang.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ai_lang.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ai_lang.dir/clean

CMakeFiles/ai_lang.dir/depend:
	cd /home/runner/workspace/ai_language/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/runner/workspace/ai_language /home/runner/workspace/ai_language /home/runner/workspace/ai_language/build /home/runner/workspace/ai_language/build /home/runner/workspace/ai_language/build/CMakeFiles/ai_lang.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/ai_lang.dir/depend

