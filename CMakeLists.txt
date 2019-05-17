cmake_minimum_required(VERSION 2.8.2) # 3.10 needed for MSVC switches

project(mandelbrot)

# options #
option(BUILD_TESTS "Build tests and googletest" OFF) #off by default to not overload user

# version #
set(MAJOR_VERSION 0)
set(MINOR_VERSION 1a)

##########################
### Setting up targets ###
##########################

# parse tree
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
file(GLOB SOURCES "${SRC_DIR}/*.cpp")

## Main library ##
add_library(mandelbrot_lib ${SOURCES})
target_include_directories(mandelbrot_lib PUBLIC ${INCLUDE_DIR})

## Main ##
add_executable(mandelbrot "main.cpp")
target_link_libraries(mandelbrot mandelbrot_lib)

## Tests ##
if (BUILD_TESTS)
	include(tests/tests.cmake)
else()
	message(AUTHOR_WARNING "Not compiling tests.")
endif()

##################################
### Dependancies and C++ flags ###
##################################

## C++ standard ##
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.10" OR NOT MSVC) # msvc switches are there
	set_property(GLOBAL PROPERTY CXX_STANDARD 14)
	set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)
elseif(MSVC_VERSION GREATER_EQUAL "1800") # VS2015
	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("/std:c++14" _cpp_14_supported)
	if (_cpp_14_supported)
		add_compile_options("/std:c++14")
	else()
		message(FATAL_ERROR "Error : C++14 is required, consider updating Visual Studio")
	endif()
endif()

## Optimization level and warnings ##
if (MSVC)
	add_compile_options(/WX /W4 /O3)
else()
	add_compile_options(-Wall -Wextra -pedantic -Werror -O3)
endif()

## Boost ##
find_package(Boost REQUIRED COMPONENTS multiprecision)
target_include_directories(mandelbrot_lib SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(mandelbrot_lib ${Boost_libraries})

## SFML ##
find_package(SFML 2 REQUIRED COMPONENTS graphics window)
target_link_libraries(mandelbrot sfml-graphics)

## HPX ? ##


