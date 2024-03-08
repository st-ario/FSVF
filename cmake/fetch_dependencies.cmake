# Download git submodules
find_package(Git QUIET)

if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    message(STATUS "Updating submodules")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                    RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/Catch2/CMakeLists.txt")
    message(FATAL_ERROR "The Catch2 submodule was not downloaded! GIT_SUBMODULE was turned off or failed; please update submodules and try again.")
endif()

add_subdirectory(extern/Catch2)
