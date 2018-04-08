cmake_minimum_required(VERSION 2.8)

project(Cifar10Converter)

find_package(Caffe REQUIRED)
find_package(OpenCV REQUIRED)
find_package(GTest)

file(GLOB_RECURSE SOURCES Sources/*.cpp)
list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Sources/main.cpp)
add_library(core STATIC ${SOURCES})

target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/Sources)
target_include_directories(core PUBLIC ${Caffe_INCLUDE_DIRS})
target_compile_options(core PUBLIC -std=c++11 -O2 -g3)
target_link_libraries(core ${OpenCV_LIBRARIES} ${Caffe_LIBRARIES})

add_executable(Converter Sources/main.cpp)
target_link_libraries(Converter core)
set_target_properties(Converter PROPERTIES OUTPUT_NAME Converter.e)

if (${GTEST_FOUND})
	add_executable(ResizeTest Tests/ResizeTest.cpp)
	target_link_libraries(ResizeTest core GTest::GTest GTest::Main)
endif ()

