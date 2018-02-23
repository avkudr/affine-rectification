cmake_minimum_required(VERSION 2.8)
project( affine-rectification )

set(CMAKE_BUILD_TYPE Debug)

find_package(OpenCV 3.0 QUIET)

include_directories(
    SYSTEM
    ${OpenCV_INCLUDE_DIRS}
)

set(PROJECT_SOURCES
    ${SOURCE}
    ./src/main_rectification.cpp
)

add_executable( main_rectification ${PROJECT_SOURCES} )
target_link_libraries( main_rectification ${OpenCV_LIBS} )
