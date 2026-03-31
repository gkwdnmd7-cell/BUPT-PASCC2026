# CMake generated Testfile for 
# Source directory: C:/Users/16978/Desktop/大三下/编译原理课设/BUPT-PASCC2026
# Build directory: C:/Users/16978/Desktop/大三下/编译原理课设/BUPT-PASCC2026/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(lexer_tests "C:/Users/16978/Desktop/大三下/编译原理课设/BUPT-PASCC2026/build/Debug/lexer_tests.exe")
  set_tests_properties(lexer_tests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/16978/Desktop/大三下/编译原理课设/BUPT-PASCC2026/CMakeLists.txt;25;add_test;C:/Users/16978/Desktop/大三下/编译原理课设/BUPT-PASCC2026/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(lexer_tests "C:/Users/16978/Desktop/大三下/编译原理课设/BUPT-PASCC2026/build/Release/lexer_tests.exe")
  set_tests_properties(lexer_tests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/16978/Desktop/大三下/编译原理课设/BUPT-PASCC2026/CMakeLists.txt;25;add_test;C:/Users/16978/Desktop/大三下/编译原理课设/BUPT-PASCC2026/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(lexer_tests "C:/Users/16978/Desktop/大三下/编译原理课设/BUPT-PASCC2026/build/MinSizeRel/lexer_tests.exe")
  set_tests_properties(lexer_tests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/16978/Desktop/大三下/编译原理课设/BUPT-PASCC2026/CMakeLists.txt;25;add_test;C:/Users/16978/Desktop/大三下/编译原理课设/BUPT-PASCC2026/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(lexer_tests "C:/Users/16978/Desktop/大三下/编译原理课设/BUPT-PASCC2026/build/RelWithDebInfo/lexer_tests.exe")
  set_tests_properties(lexer_tests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/16978/Desktop/大三下/编译原理课设/BUPT-PASCC2026/CMakeLists.txt;25;add_test;C:/Users/16978/Desktop/大三下/编译原理课设/BUPT-PASCC2026/CMakeLists.txt;0;")
else()
  add_test(lexer_tests NOT_AVAILABLE)
endif()
