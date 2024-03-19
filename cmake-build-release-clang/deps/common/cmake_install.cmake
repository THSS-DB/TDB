# Install script for directory: /home/ycycse/workSpace/test/TDB/deps/common

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/ycycse/workSpace/test/TDB/cmake-build-release-clang/lib/libcommon.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/conf" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/conf/ini.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/defs.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/io" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/io/io.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/io" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/io/roll_select_dir.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/io" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/io/select_dir.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/lang" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/lang/bitmap.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/lang" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/lang/comparator.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/lang" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/lang/defer.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/lang" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/lang/lower_bound.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/lang" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/lang/lru_cache.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/lang" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/lang/mutex.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/lang" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/lang/serializable.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/lang" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/lang/string.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/log" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/log/log.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/math" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/math/md5.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/math" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/math/random_generator.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/math" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/math/regex.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/metrics" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/metrics/console_reporter.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/metrics" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/metrics/histogram_snapshot.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/metrics" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/metrics/log_reporter.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/metrics" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/metrics/metric.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/metrics" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/metrics/metrics.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/metrics" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/metrics/metrics_registry.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/metrics" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/metrics/reporter.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/metrics" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/metrics/reservoir.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/metrics" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/metrics/sampler.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/metrics" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/metrics/snapshot.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/metrics" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/metrics/timer_snapshot.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/metrics" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/metrics/uniform_reservoir.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/mm" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/mm/debug_new.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/mm" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/mm/mem.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/mm" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/mm/mem_pool.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/os" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/os/os.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/os" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/os/path.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/os" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/os/pidfile.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/os" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/os/process.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/os" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/os/process_param.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/os" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/os/signal.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/time" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/time/datetime.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deps/common/time" TYPE FILE FILES "/home/ycycse/workSpace/test/TDB/deps/common/deps/common/time/timeout_info.h")
endif()

