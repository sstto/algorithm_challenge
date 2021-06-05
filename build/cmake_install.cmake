<<<<<<< HEAD
# Install script for directory: C:/Users/kyi88/git/algo_hw2
=======
# Install script for directory: /Users/sim-yeong-in/Google 드라이브/2021-1/알고리즘/hw2
>>>>>>> 7bf45b749dc989912942e89677dc52d472b77cbe

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/subgraph_matching")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
<<<<<<< HEAD
  include("C:/Users/kyi88/git/algo_hw2/build/main/cmake_install.cmake")
=======
  include("/Users/sim-yeong-in/Google 드라이브/2021-1/알고리즘/hw2/build/main/cmake_install.cmake")
>>>>>>> 7bf45b749dc989912942e89677dc52d472b77cbe

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
<<<<<<< HEAD
file(WRITE "C:/Users/kyi88/git/algo_hw2/build/${CMAKE_INSTALL_MANIFEST}"
=======
file(WRITE "/Users/sim-yeong-in/Google 드라이브/2021-1/알고리즘/hw2/build/${CMAKE_INSTALL_MANIFEST}"
>>>>>>> 7bf45b749dc989912942e89677dc52d472b77cbe
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
