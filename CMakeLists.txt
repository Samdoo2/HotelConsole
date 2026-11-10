cmake_minimum_required(VERSION 3.10.0)
project(HotelConsole VERSION 0.1.0 LANGUAGES C CXX)

# 1. Le decimos a CMake dónde encontrar los archivos .h
include_directories(header)

# 2. Le decimos a CMake que busque TODOS los archivos .cpp dentro de la carpeta "source"
file(GLOB_RECURSE SOURCES "source/*.cpp")

# 3. Creamos el ejecutable usando el main.cpp y todos los .cpp que encontró en "source"
add_executable(HotelConsole main.cpp ${SOURCES})