# CMakeList.txt: проект CMake для CMakeProject1; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project ("Srv")

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

# Добавьте источник в исполняемый файл этого проекта.
# add_executable (CMakeProject1 "CMakeProject1.cpp" "CMakeProject1.h")
add_executable (Srv "server.c")
# add_executable (Srv "server2.c")

target_link_libraries(Srv Threads::Threads)
find_package(Threads REQUIRED)



