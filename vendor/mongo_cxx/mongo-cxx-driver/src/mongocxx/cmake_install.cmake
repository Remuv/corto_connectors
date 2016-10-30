# Install script for directory: /home/rfloresx/work/remuv/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/mongocxx

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/rfloresx/work/remuv/recorto/vendor/mongo_cxx/mongo-cxx-driver/install")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mongocxx/v_noabi" TYPE DIRECTORY FILES "/home/rfloresx/work/remuv/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/mongocxx" FILES_MATCHING REGEX "/[^/]*\\.h$" REGEX "/[^/]*\\.hpp$" REGEX ".+/mock" EXCLUDE REGEX ".+/private" EXCLUDE REGEX ".+/test" EXCLUDE)
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mongocxx/v_noabi/mongocxx/config" TYPE FILE FILES "/home/rfloresx/work/remuv/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/mongocxx/config/export.hpp")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "runtime")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/rfloresx/work/remuv/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/mongocxx/CMakeFiles/CMakeRelink.dir/libmongocxx.so.3.0.0"
    "/home/rfloresx/work/remuv/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/mongocxx/CMakeFiles/CMakeRelink.dir/libmongocxx.so._noabi"
    "/home/rfloresx/work/remuv/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/mongocxx/CMakeFiles/CMakeRelink.dir/libmongocxx.so"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/rfloresx/work/remuv/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/mongocxx/libmongocxx.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libmongocxx-3.0.0" TYPE FILE FILES
    "/home/rfloresx/work/remuv/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/mongocxx/libmongocxx-config.cmake"
    "/home/rfloresx/work/remuv/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/mongocxx/libmongocxx-config-version.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/rfloresx/work/remuv/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/mongocxx/config/cmake_install.cmake")
  include("/home/rfloresx/work/remuv/recorto/vendor/mongo_cxx/mongo-cxx-driver/src/mongocxx/test/cmake_install.cmake")

endif()

