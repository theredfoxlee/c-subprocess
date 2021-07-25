cmake_minimum_required(VERSION 3.9)


project(
    subprocess
    LANGUAGES C
    VERSION 0.1.0
    DESCRIPTION "Yet another subprocess interface for C."
)

add_library(
    subprocess
    SHARED
    src/subprocess.c
    src/subprocess_attr.c
)

target_include_directories(subprocess PUBLIC inc)
target_include_directories(subprocess PRIVATE src)

add_executable(
    main
    test/main.c
)

target_link_libraries(
    main
    subprocess
)
