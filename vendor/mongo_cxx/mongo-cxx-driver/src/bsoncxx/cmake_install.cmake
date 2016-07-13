# Install script for directory: /home/hendren/dev/remuv/src/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/bsoncxx

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/hendren/dev/remuv/src/recorto/vendor/mongo_cxx/mongo-cxx-driver/install")
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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/bsoncxx/v_noabi" TYPE DIRECTORY FILES "/home/hendren/dev/remuv/src/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/bsoncxx" FILES_MATCHING REGEX "/[^/]*\\.h$" REGEX "/[^/]*\\.hpp$" REGEX ".+/private" EXCLUDE REGEX ".+/test" EXCLUDE)
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/bsoncxx/v_noabi/bsoncxx/config" TYPE FILE FILES "/home/hendren/dev/remuv/src/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/bsoncxx/config/export.hpp")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "runtime")
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbsoncxx.so.3.0.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbsoncxx.so._noabi"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbsoncxx.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "/usr/local/lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/hendren/dev/remuv/src/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/bsoncxx/libbsoncxx.so.3.0.0"
    "/home/hendren/dev/remuv/src/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/bsoncxx/libbsoncxx.so._noabi"
    "/home/hendren/dev/remuv/src/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/bsoncxx/libbsoncxx.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbsoncxx.so.3.0.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbsoncxx.so._noabi"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbsoncxx.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "/usr/local/lib:"
           NEW_RPATH "/usr/local/lib")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/hendren/dev/remuv/src/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/bsoncxx/libbsoncxx.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libbsoncxx-3.0.0" TYPE FILE FILES
    "/home/hendren/dev/remuv/src/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/bsoncxx/libbsoncxx-config.cmake"
    "/home/hendren/dev/remuv/src/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/bsoncxx/libbsoncxx-config-version.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/hendren/dev/remuv/src/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/bsoncxx/third_party/cmake_install.cmake")
  include("/home/hendren/dev/remuv/src/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/bsoncxx/config/cmake_install.cmake")
  include("/home/hendren/dev/remuv/src/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/bsoncxx/test/cmake_install.cmake")

endif()

