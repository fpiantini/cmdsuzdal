# ***************************************************************
# CMakeLists.txt file for the Commander Suzdal Library
#
cmake_minimum_required(VERSION 3.8)

# ***************************************************************
set(CTGT cmdsuzdal)
project(lib${CTGT} VERSION 1.0.0)
# ***************************************************************

option(ENABLE_COVERAGE_TEST "Enable test coverage computation" ON)
# Disable by default the installation of the Google test headers and libraries
option(INSTALL_GTEST "Enable installation of Gtest" OFF)

# This is necessary to use cmake_addons. It is assumed that
# cmake_addons component is placed in the base directory of the
# project inside the cmake_addons directory (e.g. using a
# git submodule)
# Find cmake_addons at: https://github.com/fpiantini/cmake_addons
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake_addons)

# ---------------------------------------------------------------
set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${CTGT}/bbdefines.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${CTGT}/bitboard.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${CTGT}/chessdefines.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${CTGT}/chessmove.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${CTGT}/army.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${CTGT}/fenrecord.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${CTGT}/chessboard.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${CTGT}/chessgame.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${CTGT}/chessengine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${CTGT}/randomengine.h
)

# ---------------------------------------------------------------
# Current Target library target definition and related settings
# A library can be defined STATIC or SHARED, but, as suggested by CS
# 4.6, we avoid to use the keyword here. The type of library built
# can be specified at configuration time using the BUILD_SHARED_LIBS
# variable (e.g. -DBUILD_SHARED_LIBS=YES)
add_library(${CTGT})
target_sources(${CTGT} PRIVATE
    src/bbdefines.cpp
    src/chessdefines.cpp
    src/bitboard.cpp
    src/chessmove.cpp
    src/army.cpp
    src/fenrecord.cpp
    src/chessboard.cpp
    src/chessgame.cpp
    src/randomengine.cpp
)

# ------------------------------------------------------------------
target_include_directories(${CTGT}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    # added to take into account private headers in src directory
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(${CTGT} PRIVATE -Werror)
target_compile_features(${CTGT} PRIVATE cxx_std_17)

# ------------------------------------------------------------------
# Installation
install(FILES ${HEADERS}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${CTGT})

install(TARGETS ${CTGT}
        EXPORT ${PROJECT_NAME}_export
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

install(EXPORT ${PROJECT_NAME}_export
        FILE ${CTGT}-config.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

# ------------------------------------------------------------------
if (ENABLE_COVERAGE_TEST)
    if(CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_BUILD_TYPE Debug)
        message(STATUS "Forcing CMAKE_BUILD_TYPE to debug (test coverage computation is enabled)")
        include(CodeCoverage)
        append_coverage_compiler_flags()
        if (COVERAGE_USE_GCOVR)
            setup_target_for_coverage_gcovr_html(
                NAME ${PROJECT_NAME}_coverage
                EXECUTABLE ctest
                EXCLUDE /usr/include
                    ${PROJECT_SOURCE_DIR}/test/*
                    ${CMAKE_CURRENT_BINARY_DIR}/*
                    ${CMAKE_CURRENT_BINARY_DIR}/googletest-src
            )
        else()
            setup_target_for_coverage_fastcov(
                NAME ${PROJECT_NAME}_coverage
                EXECUTABLE ctest
                EXCLUDE /usr/include
                    ${PROJECT_SOURCE_DIR}/test/*
                    ${CMAKE_CURRENT_BINARY_DIR}/*
                    ${CMAKE_CURRENT_BINARY_DIR}/googletest-src
            )
        endif()
    endif()
endif()

enable_testing()
add_subdirectory(test)
