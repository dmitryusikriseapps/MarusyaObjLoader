cmake_minimum_required(VERSION 3.6.0)

add_library(obj_loader_jni SHARED
            obj_loader_jni.cpp)

# Include libraries needed for obj-loader-jni lib
target_link_libraries(obj_loader_jni
                      android
                      log)