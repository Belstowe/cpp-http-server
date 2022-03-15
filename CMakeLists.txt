# 3.13 is a minimum version due to using 'target_link_libraries' instruction
cmake_minimum_required(VERSION 3.13..3.21)

project(
	http-server
	VERSION 0.7
	LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

include_directories(src)

add_subdirectory(src)