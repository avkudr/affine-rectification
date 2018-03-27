cmake_minimum_required(VERSION 2.8)
project( affine-rectification )

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE Debug)

find_package(OpenCV 3.0 QUIET)

set(ROBEST_DIR ${CMAKE_SOURCE_DIR}/3rdparty/robest/src)

set(PROJECT_SOURCES
    ${SOURCE}
    ${ROBEST_DIR}/robust_estim.hpp
    ./src/main_rectification.cpp
    ./src/FundMatFitting.cpp
    ./src/FundMatFitting.hpp
    ./src/RectifierAffine.hpp
    ./src/RectifierAffine.cpp

)

add_executable( main_rectification ${PROJECT_SOURCES} )
target_include_directories(main_rectification PUBLIC SYSTEM ${OpenCV_INCLUDE_DIRS} ${ROBEST_DIR})
target_link_libraries( main_rectification ${OpenCV_LIBS} )
