set(TEST_COMPILE_FLAGS ${DEFAULT_COMPILATION_FLAGS})

if(ON_WINDOWS)
  string(APPEND TEST_COMPILE_FLAGS "/wd4324")
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  string(APPEND TEST_COMPILE_FLAGS ${DEFAULT_RELEASE_COMPILATION_FLAGS})
else()
  string(APPEND TEST_COMPILE_FLAGS ${DEFAULT_DEBUT_COMPILATION_FLAGS})
endif()

#add_executable(unit_tests tests.cpp FloatUtils.cpp "../src/FSVF.h")
add_executable(unit_tests FloatUtils.cpp GenericMethods.cpp Operators.cpp Mat4Tests.cpp "../src/FSVF.h")

target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(unit_tests PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../src")

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(unit_tests)

if(ON_LINUX)
  target_compile_options(Catch2 PUBLIC ${DEFAULT_COMPILATION_FLAGS} "-w")
  target_compile_options(Catch2WithMain PUBLIC ${DEFAULT_COMPILATION_FLAGS} "-w")
elseif(ON_WINDOWS)
  target_compile_options(Catch2 PUBLIC "/w")
  target_compile_options(Catch2WithMain PUBLIC "/w")

  target_compile_options(unit_tests PUBLIC "/GL")
  target_link_options(unit_tests PUBLIC "/LTCG")
endif()

target_compile_options(unit_tests PUBLIC ${TESTS_COMPILE_FLAGS})

if(ON_LINUX)
  set_target_properties(unit_tests PROPERTIES OUTPUT_NAME "tests.bin")
elseif(ON_WINDOWS)
  set_target_properties(unit_tests PROPERTIES OUTPUT_NAME "tests.exe")
endif()
