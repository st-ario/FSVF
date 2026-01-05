set(TEST_COMPILE_FLAGS "${DEFAULT_COMPILATION_FLAGS}")

if(ON_WINDOWS)
  list(APPEND TEST_COMPILE_FLAGS "/wd4324")
endif()

if (CMAKE_BUILD_TYPE MATCHES "Release")
  list(APPEND TEST_COMPILE_FLAGS "${DEFAULT_RELEASE_COMPILATION_FLAGS}")
elseif(CMAKE_BUILD_TYPE MATCHES "Debug")
  list(APPEND TEST_COMPILE_FLAGS "${DEFAULT_DEBUG_COMPILATION_FLAGS}")
endif()

add_subdirectory(extern/eigen)

# TODO
#add_executable(matrix_gen
#  Mat4Aux.cpp
#  "../src/FSVF.h")
#target_include_directories(matrix_gen PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../src")
#target_include_directories(matrix_gen PUBLIC "${CMAKE_CURRENT_LIST_DIR}/extern/eigen")
#target_compile_options(matrix_gen PRIVATE "${TEST_COMPILE_FLAGS}")

add_subdirectory(extern/nanobench)
set_property(TARGET nanobench PROPERTY CXX_STANDARD 17)
target_include_directories(nanobench PUBLIC ${PROJECT_SOURCE_DIR}/test/nanobench/src/include)

add_executable(benchmarks
  Benchmark.cpp
  "../src/FSVF.h")
target_include_directories(benchmarks PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../src")
target_include_directories(benchmarks PUBLIC "${CMAKE_CURRENT_LIST_DIR}/extern/eigen")
target_compile_options(benchmarks PRIVATE "${TEST_COMPILE_FLAGS}")
target_link_libraries(benchmarks PRIVATE nanobench)

add_executable(unit_tests
  TestShuffles.cpp
  FloatUtils.cpp
  GenericMethods.cpp
  Operators.cpp
  Mat4Tests.cpp
  "../src/FSVF.h")

target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(unit_tests PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../src")
target_include_directories(unit_tests PUBLIC "${CMAKE_CURRENT_LIST_DIR}/extern/eigen")

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(unit_tests)

if(ON_LINUX)
  target_compile_options(Catch2 PRIVATE "${DEFAULT_COMPILATION_FLAGS};-w;-masm=att")
  target_compile_options(Catch2WithMain PRIVATE "${DEFAULT_COMPILATION_FLAGS};-w;-masm=att")
elseif(ON_WINDOWS)
  target_compile_options(Catch2 PRIVATE "/w")
  target_compile_options(Catch2WithMain PRIVATE "/w")

  target_compile_options(unit_tests PRIVATE "/GL")
  target_link_options(unit_tests PRIVATE "/LTCG")
endif()

target_compile_options(unit_tests PRIVATE "${TEST_COMPILE_FLAGS}")

if(ON_LINUX)
  set_target_properties(unit_tests PROPERTIES OUTPUT_NAME "tests.bin")
elseif(ON_WINDOWS)
  set_target_properties(unit_tests PROPERTIES OUTPUT_NAME "tests.exe")
endif()
