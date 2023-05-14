cmake_minimum_required(VERSION 3.1)
project(mandelbrot_cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

include_directories(include)

set(SRC_FILES
  src/main.cpp)

set(JPEG_SRC_FILES
  src/toojpeg.cpp)

set(QT_MAIN_WINDOW_SRC
  src/main_window.cpp
  include/main_window.hpp)

set(MAIN_APP_SRC
  src/main_app.cpp)

QT5_WRAP_CPP(QT_MAIN_WINDOW_MOCrcs
  ${QT_MAIN_WINDOW_SRC}
)

add_library(toojpeg
  ${JPEG_SRC_FILES}
  )

add_executable(mandelbrot_cpp
  ${SRC_FILES}
  )

target_link_libraries(mandelbrot_cpp
  toojpeg
  )

add_library(main_window
  ${QT_MAIN_WINDOW_MOCrcs}
  ${QT_MAIN_WINDOW_SRC}
  )
target_link_libraries(main_window
  Qt5::Core
  Qt5::Widgets
  )

add_executable(main_app
  ${MAIN_APP_SRC}
  )
target_link_libraries(main_app
  main_window
  Qt5::Core
  Qt5::Widgets
  )
