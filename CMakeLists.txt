cmake_minimum_required(VERSION 3.22)

# fix how cmake handles compiler checks
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(FSVF)

# available build types on multi-config generators
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# default build type on single-config generators
set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# optimized globally
include("${CMAKE_CURRENT_LIST_DIR}/cmake/system_checks.cmake")
set(CMAKE_CXX_FLAGS "${ARCHITECTURE_FLAGS}")

# useful for clangd and other tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "" FORCE)

# folder structure in some IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# compilation flags I expect to use on almost all targets (not set globally,
# in order to make per-target configurations easier)
if(ON_GCC)
    set(DEFAULT_COMPILATION_FLAGS -fdiagnostics-show-template-tree
        -fno-elide-type #-msse4.2
        -march=native
        -Wno-unknown-pragmas
        # https://gcc.gnu.org/wiki/FloatingPointMath
        # https://kristerw.github.io/2021/10/19/fast-math/
        )
    set(DEFAULT_RELEASE_COMPILATION_FLAGS -O3 -DNDEBUG -W)
    set(DEFAULT_DEBUG_COMPILATION_FLAGS -ggdb -Og -Werror -Wextra -Wall
        -Wpedantic -Wconversion -Warith-conversion -Wwrite-strings
        -Warray-bounds=2 -Wcast-align=strict -Wcast-qual -Wdouble-promotion
        -Wduplicated-branches -Wduplicated-cond -Wformat-overflow=2
        -Wnull-dereference -Wimplicit-fallthrough=5 -Wshift-negative-value
        -Wswitch-default -Wswitch-enum -Wuninitialized -Wfloat-equal
        -Wfloat-conversion -Wshadow -Wlogical-op -Wvla -Wextra-semi
        -Wsuggest-override)
elseif(ON_CLANG)
    set(DEFAULT_COMPILATION_FLAGS -fdiagnostics-show-template-tree
        -fno-elide-type #-msse4.2
        -march=native
        #-ffp-model=fast
        #-fdenormal-fp-math=positive-zero
        -ffp-model=precise -ffp-contract=off
        # https://clang.llvm.org/docs/UsersManual.html#controlling-floating-point-behavior
        )
    set(DEFAULT_RELEASE_COMPILATION_FLAGS -O3 -DNDEBUG -W)
    set(DEFAULT_DEBUG_COMPILATION_FLAGS -g -O0 -Werror -Wextra -Wall -Wpedantic
        -Wconversion -Wwrite-strings -Warray-bounds -Wcast-align -Wcast-qual
        -Wdouble-promotion -Wnull-dereference -Wimplicit-fallthrough
        -Wshift-negative-value -Wswitch-default -Wswitch-enum -Wuninitialized
        -Wfloat-equal -Wfloat-conversion -Wshadow -Wvla -Wextra-semi
        -Wsuggest-override)
elseif(ON_WINDOWS)
    # enable standard C++ exception handling
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:/EHsc>)

    set(DEFAULT_COMPILATION_FLAGS /nologo /MP /sdl- /GS- /GT /Oi /Oy
        /permissive-
        /fp:precise
        # https://learn.microsoft.com/en-us/cpp/build/reference/fp-specify-floating-point-behavior
        /wd4068 # unknown pragma
        /wd4514 # unreferenced inline function has been removed
        /wd4710 # function not inlined
        /wd4868 # compiler may not enforce left-to-right evaluation order in braced initializer list
    )
    set(DEFAULT_RELEASE_COMPILATION_FLAGS /W0 /O2 /MD)
    set(DEFAULT_DEBUG_COMPILATION_FLAGS /Wall /WX /MDd /diagnostics:caret /Oy- /Zi)
endif()

option("${CMAKE_PROJECT_NAME}_UNIT_TESTS" "" ON)

if("${${CMAKE_PROJECT_NAME}_UNIT_TESTS}")
    include("${CMAKE_CURRENT_LIST_DIR}/cmake/fetch_dependencies.cmake")
    enable_testing()
    add_subdirectory(test)
endif()
