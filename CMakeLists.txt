cmake_minimum_required(VERSION 3.10)
project(dceffplot)

if(NOT ROOT_FOUND)
    find_package(ROOT REQUIRED)
    include(${ROOT_USE_FILE})
endif()

if(NOT DEFINED ANAROOT_LIBRARIES)
    if(NOT DEFINED ENV{TARTSYS})
        message(FATAL_ERROR "ANAROOT (TARTSYS) is not defined.")
    endif()
    set(ANAROOT_INCLUDE_DIR $ENV{TARTSYS}/include)
    set(ANAROOT_LIB_DIR $ENV{TARTSYS}/lib)
    include_directories(${ANAROOT_INCLUDE_DIR})
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "..." FORCE)
endif()

add_subdirectory(sources)
