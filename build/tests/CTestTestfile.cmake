# CMake generated Testfile for 
# Source directory: /home/runner/workspace/tests
# Build directory: /home/runner/workspace/build/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[test_data_manager]=] "/home/runner/workspace/build/bin/tests/test_data_manager")
set_tests_properties([=[test_data_manager]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/runner/workspace/tests/CMakeLists.txt;36;add_test;/home/runner/workspace/tests/CMakeLists.txt;0;")
add_test([=[test_data_processor]=] "/home/runner/workspace/build/bin/tests/test_data_processor")
set_tests_properties([=[test_data_processor]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/runner/workspace/tests/CMakeLists.txt;36;add_test;/home/runner/workspace/tests/CMakeLists.txt;0;")
add_test([=[test_model]=] "/home/runner/workspace/build/bin/tests/test_model")
set_tests_properties([=[test_model]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/runner/workspace/tests/CMakeLists.txt;36;add_test;/home/runner/workspace/tests/CMakeLists.txt;0;")
add_test([=[test_utils]=] "/home/runner/workspace/build/bin/tests/test_utils")
set_tests_properties([=[test_utils]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/runner/workspace/tests/CMakeLists.txt;36;add_test;/home/runner/workspace/tests/CMakeLists.txt;0;")
subdirs("../_deps/googletest-build")
