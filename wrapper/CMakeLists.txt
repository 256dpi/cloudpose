cmake_minimum_required(VERSION 3.0)

add_definitions(-std=c++11)

# helps finding GFlags and Glog on linux
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/modules")

find_package(GFlags REQUIRED)
find_package(Glog REQUIRED)
find_package(OpenCV REQUIRED)

# only find caffe on osx (installed with brew)
if (APPLE)
    find_package(Caffe REQUIRED)
endif (APPLE)

find_package(OpenPose REQUIRED)

add_executable(main main.cpp wrapper.h wrapper.cpp)

include_directories(${OpenPose_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS} ${Caffe_INCLUDE_DIRS})
target_link_libraries(main caffe ${OpenPose_LIBS} ${OpenCV_LIBS} ${Caffe_LIBRARIES})
