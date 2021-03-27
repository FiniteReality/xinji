cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(Xinji
    VERSION 0.1.0
    DESCRIPTION "Schema definition language"
    HOMEPAGE_URL "https://github.com/FiniteReality/Xinji"
    LANGUAGES C
)

add_subdirectory(src)

add_subdirectory(test)
