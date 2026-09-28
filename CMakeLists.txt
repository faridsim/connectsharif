cmake_minimum_required(VERSION 3.16)
project(messaging LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

enable_testing()

add_executable(msg_demo main.cpp)
add_executable(msg_tests concurrency_test.cpp)

target_link_libraries(msg_demo PRIVATE Threads::Threads)
target_link_libraries(msg_tests PRIVATE Threads::Threads)

target_compile_options(msg_demo PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_compile_options(msg_tests PRIVATE -Wall -Wextra -Wpedantic -Werror)

add_test(NAME msg_tests COMMAND msg_tests)
