cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(native_library VERSION 1.0.0 LANGUAGES CXX)
add_library(native_library SHARED native.cpp native.def)
add_executable(native native.cpp)

set_target_properties(native PROPERTIES
    PUBLIC_HEADER native.h
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    OUTPUT_NAME "native"
)

set_target_properties(native_library PROPERTIES
    PUBLIC_HEADER native.h
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    OUTPUT_NAME "native"
)