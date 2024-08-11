cmake_minimum_required(VERSION 3.14)

project(
	Algorithms_DataStructures_Library
	VERSION 1.0
	DESCRIPTION "C++ 20 Project to implement common algorithms and datastructures"
	LANGUAGES CXX
)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

	include(CTest)

	# Project requires C++20
	set(CMAKE_CXX_STANDARD 20)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_CXX_EXTENSIONS OFF)

	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	add_subdirectory(tests)
	add_subdirectory(manual)

endif()
