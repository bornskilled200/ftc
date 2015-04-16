cmake_minimum_required(VERSION 2.8.4)
project(ftc C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -DWIN32_LEAN_AND_MEAN -D_WIN32_WINNT=0x0600")

set(SOURCE_FILES main.c
                libuv/src/fs-poll.c
                libuv/src/heap-inl.h
                libuv/src/inet.c
                libuv/src/queue.h
                libuv/src/threadpool.c
                libuv/src/uv-common.c
                libuv/src/uv-common.h
                libuv/src/version.c)


if (WIN32)
    set(NATIVE_FILES libuv/src/win/async.c
                     libuv/src/win/atomicops-inl.h
                     libuv/src/win/core.c
                     libuv/src/win/dl.c
                     libuv/src/win/error.c
                     libuv/src/win/fs.c
                     libuv/src/win/fs-event.c
                     libuv/src/win/getaddrinfo.c
                     libuv/src/win/getnameinfo.c
                     libuv/src/win/handle.c
                     libuv/src/win/handle-inl.h
                     libuv/src/win/internal.h
                     libuv/src/win/loop-watcher.c
                     libuv/src/win/pipe.c
                     libuv/src/win/poll.c
                     libuv/src/win/process.c
                     libuv/src/win/process-stdio.c
                     libuv/src/win/req.c
                     libuv/src/win/req-inl.h
                     libuv/src/win/signal.c
                     libuv/src/win/stream.c
                     libuv/src/win/stream-inl.h
                     libuv/src/win/tcp.c
                     libuv/src/win/thread.c
                     libuv/src/win/timer.c
                     libuv/src/win/tty.c
                     libuv/src/win/udp.c
                     libuv/src/win/util.c
                     libuv/src/win/winapi.c
                     libuv/src/win/winapi.h
                     libuv/src/win/winsock.c
                     libuv/src/win/winsock.h)
else(WIN32)
    set(NATIVE_FILES libuv/src/unix/aix.c
                     libuv/src/unix/android-ifaddrs.c
                     libuv/src/unix/async.c
                     libuv/src/unix/atomic-ops.h
                     libuv/src/unix/core.c
                     libuv/src/unix/darwin.c
                     libuv/src/unix/darwin-proctitle.c
                     libuv/src/unix/dl.c
                     libuv/src/unix/freebsd.c
                     libuv/src/unix/fs.c
                     libuv/src/unix/fsevents.c
                     libuv/src/unix/getaddrinfo.c
                     libuv/src/unix/getnameinfo.c
                     libuv/src/unix/internal.h
                     libuv/src/unix/kqueue.c
                     libuv/src/unix/linux-core.c
                     libuv/src/unix/linux-inotify.c
                     libuv/src/unix/linux-syscalls.c
                     libuv/src/unix/linux-syscalls.h
                     libuv/src/unix/loop.c
                     libuv/src/unix/loop-watcher.c
                     libuv/src/unix/netbsd.c
                     libuv/src/unix/openbsd.c
                     libuv/src/unix/pipe.c
                     libuv/src/unix/poll.c
                     libuv/src/unix/process.c
                     libuv/src/unix/proctitle.c
                     libuv/src/unix/pthread-fixes.c
                     libuv/src/unix/signal.c
                     libuv/src/unix/spinlock.h
                     libuv/src/unix/stream.c
                     libuv/src/unix/sunos.c
                     libuv/src/unix/tcp.c
                     libuv/src/unix/thread.c
                     libuv/src/unix/timer.c
                     libuv/src/unix/tty.c
                     libuv/src/unix/udp.c)
endif(WIN32)

add_executable(ftc ${SOURCE_FILES} ${NATIVE_FILES})
include_directories(ftc libuv/include)

if(WIN32)
    target_link_libraries(ftc libws2_32.a libpsapi.a libiphlpapi.a)
endif(WIN32)



