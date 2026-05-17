cmake_minimum_required(VERSION 3.10)
project(RomaOS LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

add_compile_definitions(_GNU_SOURCE)

add_library(romaos STATIC
    src/scheduler.c
    src/port.c
    src/resource.c
    src/kernel.c
)

target_include_directories(romaos PUBLIC include src)

file(GLOB TEST_SOURCES "tests/*.c")
foreach(test_file ${TEST_SOURCES})
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name} romaos)
    # По желанию можно зарегистрировать тест для CTest
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()

enable_testing()