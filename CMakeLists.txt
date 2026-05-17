cmake_minimum_required(VERSION 3.10)
project(RomaOS LANGUAGES C)

# Используем C99 с расширениями GNU (нужно для ucontext, typeof и т.д.)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Включаем _GNU_SOURCE для signal.h, siginfo_t и пр.
add_compile_definitions(_GNU_SOURCE)

# Статическая библиотека ядра
add_library(romaos STATIC
    src/scheduler.c
    src/port.c
    src/resource.c
)

# Где искать заголовки (include/ и src/, т.к. там лежат task.h, list.h и др.)
target_include_directories(romaos PUBLIC include src)

# --- Тесты ---
# Чтобы не перечислять каждый файл вручную, соберём все .c из tests/
file(GLOB TEST_SOURCES "tests/*.c")
foreach(test_file ${TEST_SOURCES})
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name} romaos)
    # По желанию можно зарегистрировать тест для CTest
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()

# Включаем тестирование (чтобы работал ctest)
enable_testing()