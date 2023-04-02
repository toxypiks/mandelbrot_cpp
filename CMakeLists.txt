cmake_minimum_required(VERSION 3.1)
project(mandelbrot_cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SRC_FILES
  src/main.cpp)

set(JPEG_SRC_FILES
  src/toojpeg.cpp)

add_library(toojpeg
  ${JPEG_SRC_FILES}
  )

add_executable(mandelbrot_cpp
  ${SRC_FILES}
  )

target_link_libraries(mandelbrot_cpp
  toojpeg
  )
