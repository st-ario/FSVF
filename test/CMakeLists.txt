if(ON_WINDOWS)
    set(TESTS_COMPILE_FLAGS ${DEFAULT_COMPILATION_FLAGS} "/wd4324")
endif()

set(TESTS_COMPILE_RELEASE_FLAGS ${TESTS_COMPILE_FLAGS} ${DEFAULT_RELEASE_COMPILATION_FLAGS})
set(TESTS_COMPILE_DEBUG_FLAGS   ${TESTS_COMPILE_FLAGS} ${DEFAULT_DEBUG_COMPILATION_FLAGS})

add_executable(unit_tests tests.cpp "../src/FSVF.h")

target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(unit_tests PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../src")

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(unit_tests)

if(ON_LINUX)
    target_compile_options(Catch2 PUBLIC "-w")
    target_compile_options(Catch2WithMain PUBLIC "-w")
elseif(ON_WINDOWS)
    target_compile_options(Catch2 PUBLIC "/w")
    target_compile_options(Catch2WithMain PUBLIC "/w")

    target_compile_options(unit_tests PUBLIC "/GL")
    target_link_options(unit_tests PUBLIC "/LTCG")
endif()

target_compile_options(unit_tests PUBLIC $<$<CONFIG:DEBUG>:${TESTS_COMPILE_DEBUG_FLAGS} >
    $<$<CONFIG:RELEASE>:${TESTS_COMPILE_RELEASE_FLAGS} >)

if(ON_LINUX)
    set_target_properties(unit_tests PROPERTIES OUTPUT_NAME "tests.bin")
elseif(ON_WINDOWS)
    set_target_properties(unit_tests PROPERTIES OUTPUT_NAME "tests.exe")
endif()
