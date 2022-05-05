# CMakeList.txt: проект CMake для CMakeProject1; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project ("CcCvStore")

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread -lsqlite3 -lpthread -ldl")
include_guard(GLOBAL)


add_definitions(-DSQLITE_ENABLE_RTREE)
add_definitions(-DSQLITE_ENABLE_FTS4)
add_definitions(-DSQLITE_ENABLE_FTS5)
add_definitions(-DSQLITE_ENABLE_JSON1)
add_definitions(-DSQLITE_ENABLE_RBU)
add_definitions(-DSQLITE_ENABLE_STAT4)


# Добавьте источник в исполняемый файл этого проекта.
ADD_LIBRARY(sqlite3 STATIC sqlite3.h sqlite3.c sqlite3ext.h)

ADD_LIBRARY(LibsModule 
  ini.h
  ht.h
  global.h
  config.h
  disc_data/disc_types.h
  disc_data/disc_data.c
  router.h
  store.h


  config.c
  sts_queue.c
  add_lib.c 
  ini.c
  task.c
  msg_parser.c
  router.c
  store.c
)

find_package(Threads REQUIRED)

#target_link_libraries(LibsModule -lpthread)
#target_link_libraries(LibsModule Threads::Threads)

#add_executable (CcCvStore "test_rw_file.c")
# add_executable (CcCvStore "test.c")
#add_executable (CcCvStore "test_add_lib.c")
# add_executable (CcCvStore "test_task.c")
add_executable (CcCvStore "test_libuv.c")
#add_executable (CcCvStore "test_parce.c")
#add_executable (CcCvStore "server.c")
#add_executable (CcCvStore "test_mutex.c")
#add_executable (CcCvStore "test_ini.c")
# add_executable (CcCvStore "test_sql.c")
#add_executable (CcCvStore "test_server.c")
#add_executable (CcCvStore "test_str.c")

target_link_libraries(LibsModule -lpthread)
target_link_libraries(LibsModule Threads::Threads)

target_link_libraries(sqlite3 -lpthread -ldl -lm)

find_library(LIBUV_LIBRARIES NAMES uv libuv HINTS $ENV{LIBUV_ROOT})
target_link_libraries(CcCvStore PUBLIC ${LIBUV_LIBRARIES})

target_link_libraries(CcCvStore PUBLIC LibsModule)
target_link_libraries(CcCvStore PUBLIC sqlite3)



