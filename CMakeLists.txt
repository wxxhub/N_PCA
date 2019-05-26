cmake_minimum_required(VERSION 3.5)
project(n_pca)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED) 

include_directories(
    include
)

aux_source_directory(
    src DIR_LIB_SRCS
)
add_library(${PROJECT_NAME}
    ${DIR_LIB_SRCS}
)
target_link_libraries(${PROJECT_NAME}
  ${OpenCV_LIBS}
)

add_executable(demo demo/demo.cpp)
target_link_libraries(demo
    ${PROJECT_NAME}
)

add_executable(detector_demo demo/detector_demo.cpp)
target_link_libraries(detector_demo
    ${PROJECT_NAME}
)