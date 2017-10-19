cmake_minimum_required(VERSION 3.0)
project(OS_shell)

if (CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "-std=c++11 -g3 ${CMAKE_CXX_FLAGS}")
endif()

add_executable(
OS_shell shell.cpp
)
