cmake_minimum_required(VERSION 3.7)
project(tests LANGUAGES C ASM)

set(CMAKE_C_STANDARD 99)

add_subdirectory(src)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

# cmocka
find_package(CMocka CONFIG)
if (CMocka_FOUND)
    include(AddCMockaTest)
    include(AddMockedTest)
    add_subdirectory(test)
    enable_testing()
endif(CMocka_FOUND)
