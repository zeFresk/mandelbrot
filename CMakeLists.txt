cmake_minimum_required(VERSION 3.9) # 3.9 needed for MSVC switches and CUDA
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CUDA_VERBOSE_BUILD ON)

project(mandelbrot)

# options #
option(EXTENSIVE_DEBUG "Activate advanced debug features in debug mode, much slower" OFF)
option(PRECISION "Use MPFR or GMP to compute floating-point numbers" OFF)
set(PRECISION_DIGITS "50" CACHE STRING "Floating point precision with PRECISION option")
option(DOUBLE_FP "Override other precision floating-point settings and set precision to double" OFF)
option(OPENMP "Use OpenMP Backend if possible" ON)
option(CUDA "Use CUDA Backend if possible" ON)

# modules #
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/third_party/cmake_modules/")

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
		message(FATAL_ERROR "Error: C++14 is required, consider updating Visual Studio")
	endif()
endif()

## Optimization level and warnings ##
if (MSVC)
	#target_compile_options(mandelbrot_lib PUBLIC /W4)
else()
	target_compile_options(mandelbrot_lib PUBLIC -Wall -Wextra -pedantic -O3)
endif()

## Debug features
if (EXTENSIVE_DEBUG)
	add_compile_definitions(EXTENSIVE_DEBUG)
endif()

## Boost ##
#set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED)
target_include_directories(mandelbrot_lib SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})

## Floating point backend
if (PRECISION)
	find_package(MPFR 2.0.0)
	if (MPFR_FOUND) # search for MPFR first
		set(MPFR_FP ON)
		target_link_libraries(mandelbrot_lib ${MPFR_LIBRARIES})
	else()
		find_package(GMP 5.0.0)
		if (GMP_FOUND)
			set(GMP_FP ON)
			target_link_libraries(mandelbrot_lib ${GMP_LIBRARIES})
		else()
			message(WARNING "No floating-point backend found. Boost will be used, decreasing performance")
			set(BOOST_FP ON)
		endif()
	endif()
	if (NOT ${PRECISION_DIGITS} MATCHES "[0-9]+") # only digits
		message(FATAL_ERROR "Incorrect floating-point precision : ${PRECISION_DIGITS}")
	endif()
endif()

## SFML ##
find_package(SFML 2 REQUIRED COMPONENTS graphics window main)
target_link_libraries(mandelbrot sfml-graphics)
target_include_directories(mandelbrot SYSTEM PUBLIC ${SFML_INCLUDE_PATH})

## OpenMP backend
if (OPENMP)
	find_package(OpenMP)
	if(OpenMP_CXX_FOUND)
		set(OPENMP_BACKEND ON)
		target_link_libraries(mandelbrot_lib OpenMP::OpenMP_CXX)
	endif()
endif()

## CUDA / Thrust ##
if (CUDA)
	include(CheckLanguage)
	check_language(CUDA)
	if (CMAKE_CUDA_COMPILER)
		# Work-around for cuda-tests to work with MSVC
		if (MSVC)
			set(CMAKE_CUDA_COMPILER_WORKS TRUE)
		endif()

		enable_language(CUDA)

		# Warn user if PRECISION is set as it is incompatible with CUDA
		if (PRECISION)
			message(WARNING "Multi-precision floating-point is incompatible with CUDA backend")
		endif()

		set(DOUBLE_FP ON) # because it's so fast

		# strange bug which cause include and links not to be set on Windows
		if (NOT CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES)
			message(WARNING "CUDA_TOOLKIT_INCLUDE_DIRECTORIES is empty trying to fix it.\n\
			If this is not working please provide CUDA_TOOLKIT_INCLUDE_DIRECTORIES and CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES.")
			string(REGEX REPLACE "/[a-z]*/[a-z]*.exe" "" CUDA_ROOT ${CMAKE_CUDA_COMPILER})
			set(CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES "${CUDA_ROOT}/include")
		endif()
		if (NOT CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES AND CUDA_ROOT)
			set(CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES "${CUDA_ROOT}/lib")
			if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8") # x64
				set(CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES "${CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES}/x64")
			else()
				set(CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES "${CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES}/Win32")
			endif()
		endif()

		set(CUDA_BACKEND ON)
		#message(AUTHOR_WARNING "${CMAKE_CUDA_COMPILER} ${CUDA_ROOT} ${CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES} ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}")

		# glob .cu files
		file(GLOB CUDA_FILES "${SRC_DIR}/*.cu")

		# generate cuda library
		add_library(cuda_backend OBJECT ${CUDA_FILES})
		target_include_directories(cuda_backend SYSTEM PUBLIC ${Boost_INCLUDE_DIRS} PUBLIC ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
		target_include_directories(cuda_backend PUBLIC ${INCLUDE_DIR})
		set_target_properties(cuda_backend PROPERTIES CUDA_SEPERABLE_COMPILATION ON)

		# C++ standard
		if(NOT DEFINED CMAKE_CUDA_STANDARD)
			set(CMAKE_CUDA_STANDARD 14)
			set(CMAKE_CUDA_STANDARD_REQUIRED ON)
		endif()

		# link cuda backend and includes
		target_include_directories(mandelbrot_lib SYSTEM PUBLIC ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
		target_link_libraries(mandelbrot_lib cuda_backend cudart)

		# Add multithread to main library
		set(CompilerFlags
			CMAKE_CXX_FLAGS
			CMAKE_CXX_FLAGS_DEBUG
			CMAKE_CXX_FLAGS_RELEASE
			CMAKE_C_FLAGS
			CMAKE_C_FLAGS_DEBUG
			CMAKE_C_FLAGS_RELEASE
			)

		if (MSVC)
			set(FLAG_SYMBOL "/")
		else()
			set(FLAG_SYMBOL "-")
		endif()
		foreach(CompilerFlag ${CompilerFlags})
			string(REPLACE "${FLAG_SYMBOL}MD" "${FLAG_SYMBOL}MT" ${CompilerFlag} "${${CompilerFlag}}")
		endforeach()
	endif()
endif()

##################################
###### Configuration files #######
##################################

# MPFR, GMP, OpenMP, CUDA
configure_file("${SRC_DIR}/configuration.h.in" "${INCLUDE_DIR}/configuration.h")
