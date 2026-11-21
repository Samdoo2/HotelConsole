cmake_minimum_required(VERSION 3.12)
project(HotelConsole VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "source/*.cpp")
add_executable(HotelConsole main.cpp ${SOURCES})
target_include_directories(HotelConsole PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/header")

target_compile_definitions(HotelConsole PRIVATE
    HOTEL_PROJECT_ROOT="${CMAKE_CURRENT_SOURCE_DIR}")
