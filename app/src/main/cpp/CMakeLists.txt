cmake_minimum_required(VERSION 3.4.1)
file(GLOB SOURCE *.cpp)
include_directories(include)
set(my_lib_path ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${my_lib_path}")
add_library(
        native-lib
        SHARED
        ${SOURCE})


target_link_libraries(
        native-lib
        avcodec avfilter avformat avutil swresample swscale
        ${log-lib})