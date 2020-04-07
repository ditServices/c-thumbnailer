cmake_minimum_required(VERSION 3.15)
project(tmbnl C)

set(CMAKE_C_STANDARD 99)

find_package(PkgConfig REQUIRED)
pkg_check_modules(MAGICKCORE REQUIRED IMPORTED_TARGET MagickCore)

add_executable(tmbnl main.c thumbnailer.h thumbnailer.c)
target_link_libraries(tmbnl PUBLIC PkgConfig::MAGICKCORE)