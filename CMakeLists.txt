cmake_minimum_required(VERSION 3.10)

project(cluk LANGUAGES C)

add_executable(cluk 
    main.c
    common.h
    chunk.h
    memory.h
    memory.c
    debug.h
    debug.c)

target_include_directories(cluk PRIVATE ${PROJECT_SOURCE_DIR})