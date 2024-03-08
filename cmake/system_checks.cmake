if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed.")
endif()

# supported platforms: Linux w/ GCC or Clang, Windows w/ MSVC
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(ON_LINUX TRUE CACHE INTERNAL "")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(ON_GCC TRUE)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(ON_CLANG TRUE)
    else()
        message(FATAL_ERROR "Clang and GCC are the only compilers supported on Linux.
            You can still try to make the program compile and run, but you're on your own.")
    endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(ON_WINDOWS TRUE CACHE INTERNAL "")
    else()
      message(FATAL_ERROR "MSVC is the only compiler supported on Windows.
          You can still try to make the program compile and run, but you're on your own.")
    endif()
else()
    message(FATAL_ERROR "${CMAKE_SYSTEM_NAME} is not supported.
        You can still try to make the program compile and run, but you're on your own.")
endif()

cmake_host_system_information(RESULT SYSTEM_LCORES QUERY NUMBER_OF_LOGICAL_CORES)
cmake_host_system_information(RESULT SYSTEM_PCORES QUERY NUMBER_OF_PHYSICAL_CORES)

if(ON_LINUX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
endif()
find_package(Threads REQUIRED)