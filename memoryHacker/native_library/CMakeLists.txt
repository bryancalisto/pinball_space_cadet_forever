cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(native_library VERSION 1.0.0 LANGUAGES C)
add_library(native_library SHARED native.c native.def)
add_executable(native native.c)

set_target_properties(native_library PROPERTIES
    PUBLIC_HEADER native.h
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    OUTPUT_NAME "native"
)