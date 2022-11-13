cmake_minimum_required(VERSION 3.22)
project(Animation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_LIBRARIES ${CMAKE_CXX_STANDARD_LIBRARIES} -ljpeg)

find_package(OpenCV REQUIRED)
add_executable(Animation main.cpp Util.cpp Util.h CImg.h)

target_link_libraries(Animation ${OpenCV_LIBS})