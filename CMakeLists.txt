cmake_minimum_required(VERSION 3.12.0)
project(HotelConsole VERSION 0.1.0 LANGUAGES C CXX)

# 1. Rutas de cabeceras
include_directories(header)

# 2. Buscar dinámicamente detectando cambios en disco
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "source/*.cpp")

# 3. Crear ejecutable
add_executable(HotelConsole main.cpp ${SOURCES})