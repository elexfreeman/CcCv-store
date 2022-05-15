# CMakeList.txt: проект CMake для CMakeProject1; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project ("CcCvStore")

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread -lpthread -ldl")
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
#add_executable (CcCvStore "test_parce.c")
add_executable (CcCvStore "server.c")
#add_executable (CcCvStore "test_mutex.c")
#add_executable (CcCvStore "test_ini.c")
# add_executable (CcCvStore "test_sql.c")
#add_executable (CcCvStore "test_server.c")
#add_executable (CcCvStore "test_str.c")

# LIBUV
include(CheckTypeSize)

cmake_minimum_required(VERSION 2.8.9)

set(LIBUVDIR ${CMAKE_CURRENT_LIST_DIR}/libuv)

include_directories(
  ${LIBUVDIR}/src
  ${LIBUVDIR}/include
)

set(SOURCES
  ${LIBUVDIR}/include/uv.h
  ${LIBUVDIR}/include/uv/tree.h
  ${LIBUVDIR}/include/uv/errno.h
  ${LIBUVDIR}/include/uv/threadpool.h
  ${LIBUVDIR}/include/uv/version.h
  ${LIBUVDIR}/src/fs-poll.c
  ${LIBUVDIR}/src/heap-inl.h
  ${LIBUVDIR}/src/inet.c
  ${LIBUVDIR}/src/queue.h
  ${LIBUVDIR}/src/threadpool.c
  ${LIBUVDIR}/src/uv-common.c
  ${LIBUVDIR}/src/uv-common.h
  ${LIBUVDIR}/src/version.c
  ${LIBUVDIR}/src/timer.c
  ${LIBUVDIR}/src/strscpy.c
  ${LIBUVDIR}/src/strtok.c
)

if(WIN32)
  add_definitions(
    -D_WIN32_WINNT=0x0600
    -D_CRT_SECURE_NO_WARNINGS
    -D_GNU_SOURCE
  )
  set(SOURCES ${SOURCES}
    ${LIBUVDIR}/include/uv-win.h
    ${LIBUVDIR}/src/win/async.c
    ${LIBUVDIR}/src/win/atomicops-inl.h
    ${LIBUVDIR}/src/win/core.c
    ${LIBUVDIR}/src/win/detect-wakeup.c
    ${LIBUVDIR}/src/win/dl.c
    ${LIBUVDIR}/src/win/error.c
    ${LIBUVDIR}/src/win/fs-event.c
    ${LIBUVDIR}/src/win/fs.c
    ${LIBUVDIR}/src/win/getaddrinfo.c
    ${LIBUVDIR}/src/win/getnameinfo.c
    ${LIBUVDIR}/src/win/handle-inl.h
    ${LIBUVDIR}/src/win/handle.c
    ${LIBUVDIR}/src/win/internal.h
    ${LIBUVDIR}/src/win/loop-watcher.c
    ${LIBUVDIR}/src/win/pipe.c
    ${LIBUVDIR}/src/win/poll.c
    ${LIBUVDIR}/src/win/process-stdio.c
    ${LIBUVDIR}/src/win/process.c
    ${LIBUVDIR}/src/win/req-inl.h
    ${LIBUVDIR}/src/win/req.c
    ${LIBUVDIR}/src/win/signal.c
    ${LIBUVDIR}/src/win/snprintf.c
    ${LIBUVDIR}/src/win/stream-inl.h
    ${LIBUVDIR}/src/win/stream.c
    ${LIBUVDIR}/src/win/tcp.c
    ${LIBUVDIR}/src/win/thread.c
    ${LIBUVDIR}/src/win/timer.c
    ${LIBUVDIR}/src/win/tty.c
    ${LIBUVDIR}/src/win/udp.c
    ${LIBUVDIR}/src/win/util.c
    ${LIBUVDIR}/src/win/winapi.c
    ${LIBUVDIR}/src/win/winapi.h
    ${LIBUVDIR}/src/win/winsock.c
    ${LIBUVDIR}/src/win/winsock.h
  )
else()
  include_directories(${LIBUVDIR}/src/unix)
  set(SOURCES ${SOURCES}
    ${LIBUVDIR}/include/uv/unix.h
    ${LIBUVDIR}/include/uv/linux.h
    ${LIBUVDIR}/include/uv/sunos.h
    ${LIBUVDIR}/include/uv/bsd.h
    ${LIBUVDIR}/include/uv/aix.h
    ${LIBUVDIR}/src/unix/async.c
    ${LIBUVDIR}/src/unix/atomic-ops.h
    ${LIBUVDIR}/src/unix/core.c
    ${LIBUVDIR}/src/unix/dl.c
    ${LIBUVDIR}/src/unix/fs.c
    ${LIBUVDIR}/src/unix/getaddrinfo.c
    ${LIBUVDIR}/src/unix/getnameinfo.c
    ${LIBUVDIR}/src/unix/internal.h
    ${LIBUVDIR}/src/unix/loop-watcher.c
    ${LIBUVDIR}/src/unix/loop.c
    ${LIBUVDIR}/src/unix/pipe.c
    ${LIBUVDIR}/src/unix/poll.c
    ${LIBUVDIR}/src/unix/process.c
    ${LIBUVDIR}/src/unix/proctitle.c
    ${LIBUVDIR}/src/unix/signal.c
    ${LIBUVDIR}/src/unix/spinlock.h
    ${LIBUVDIR}/src/unix/stream.c
    ${LIBUVDIR}/src/unix/tcp.c
    ${LIBUVDIR}/src/unix/thread.c
    ${LIBUVDIR}/src/unix/tty.c
    ${LIBUVDIR}/src/unix/udp.c
    ${LIBUVDIR}/src/unix/epoll.c
  )
endif()

check_type_size("void*" SIZEOF_VOID_P)
if(SIZEOF_VOID_P EQUAL 8)
  add_definitions(-D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE)
endif()

## Freebsd
if("${CMAKE_SYSTEM_NAME}" MATCHES "FreeBSD")
  set(SOURCES ${SOURCES}
    ${LIBUVDIR}/src/unix/kqueue.c
    ${LIBUVDIR}/src/unix/freebsd.c
  )
endif()

## OpenBSD
if("${CMAKE_SYSTEM_NAME}" MATCHES "OpenBSD")
  set(SOURCES ${SOURCES}
    ${LIBUVDIR}/src/unix/kqueue.c
    ${LIBUVDIR}/src/unix/openbsd.c
  )
endif()

## Linux
if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
  add_definitions(
    -D_GNU_SOURCE
  )
  set(SOURCES ${SOURCES}
    ${LIBUVDIR}/src/unix/proctitle.c
    ${LIBUVDIR}/src/unix/linux-core.c
    ${LIBUVDIR}/src/unix/linux-inotify.c
    ${LIBUVDIR}/src/unix/linux-syscalls.c
    ${LIBUVDIR}/src/unix/linux-syscalls.h
    ${LIBUVDIR}/src/unix/procfs-exepath.c
    ${LIBUVDIR}/src/unix/sysinfo-loadavg.c
    ${LIBUVDIR}/src/unix/sysinfo-memory.c
  )
endif()

## SunOS
if("${CMAKE_SYSTEM_NAME}" MATCHES "SunOS")
  add_definitions(
    -D__EXTENSIONS__
    -D_XOPEN_SOURCE=500
  )
  set(SOURCES ${SOURCES}
    ${LIBUVDIR}/src/unix/sunos.c
  )
endif()

## Darwin
if(APPLE)
  add_definitions(
    -D=_DARWIN_USE_64_BIT_INODE
  )
  set(SOURCES ${SOURCES}
    ${LIBUVDIR}/include/uv-darwin.h
    ${LIBUVDIR}/src/unix/bsd-ifaddrs.c
    ${LIBUVDIR}/src/unix/darwin.c
    ${LIBUVDIR}/src/unix/darwin-proctitle.c
    ${LIBUVDIR}/src/unix/fsevents.c
    ${LIBUVDIR}/src/unix/kqueue.c
  )
endif()

add_library(uv STATIC ${SOURCES})
set_property(TARGET uv PROPERTY POSITION_INDEPENDENT_CODE ON)

if("${CMAKE_SYSTEM_NAME}" MATCHES "FreeBSD")
  target_link_libraries(uv
    pthread
    kvm
  )
endif()

if("${CMAKE_SYSTEM_NAME}" MATCHES "OpenBSD")
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  target_link_libraries(uv Threads::Threads)
endif()

if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
  target_link_libraries(uv
    pthread
  )
endif()

if(WIN32)
  target_link_libraries(uv
    ws2_32.lib
    shell32.lib
    psapi.lib
    iphlpapi.lib
    advapi32.lib
    userenv.lib
  )
endif()

if("${CMAKE_SYSTEM_NAME}" MATCHES "SunOS")
  target_link_libraries(uv
    kstat
    socket
    sendfile
  )
endif()

if(APPLE)
  find_library(FOUNDATION_LIBRARY Foundation)
  find_library(CORESERVICES_LIBRARY CoreServices)
  find_library(APPLICATION_SERVICES_LIBRARY ApplicationServices)
  target_link_libraries(uv
    ${FOUNDATION_LIBRARY}
    ${CORESERVICES_LIBRARY}
    ${APPLICATION_SERVICES_LIBRARY}
  )
endif()

# /LIBUV

target_link_libraries(LibsModule -lpthread)
target_link_libraries(LibsModule Threads::Threads)

target_link_libraries(sqlite3 -lpthread -ldl -lm)

#find_library(LIBUV_LIBRARIES NAMES uv libuv HINTS $ENV{LIBUV_ROOT})
#target_link_libraries(CcCvStore PUBLIC ${LIBUV_LIBRARIES})

target_link_libraries(CcCvStore PUBLIC uv)

target_link_libraries(CcCvStore PUBLIC LibsModule)
target_link_libraries(CcCvStore PUBLIC sqlite3)



