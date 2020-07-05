cmake_minimum_required(VERSION 3.4.1)
file(GLOB source *.cpp)
include_directories(include)
set(my_lib_path ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${my_lib_path}")
add_library(
        native-lib
        SHARED
        ${source})


target_link_libraries(
        native-lib
        -Wl,--start-group
        avcodec avfilter avformat avutil swresample swscale
        -Wl,--end-group
        ${log-lib}
        log
        z)