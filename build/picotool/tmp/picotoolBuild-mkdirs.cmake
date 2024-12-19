# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/ahmed/pico-projects/web_server/build/_deps/picotool-src"
  "/home/ahmed/pico-projects/web_server/build/_deps/picotool-build"
  "/home/ahmed/pico-projects/web_server/build/_deps"
  "/home/ahmed/pico-projects/web_server/build/picotool/tmp"
  "/home/ahmed/pico-projects/web_server/build/picotool/src/picotoolBuild-stamp"
  "/home/ahmed/pico-projects/web_server/build/picotool/src"
  "/home/ahmed/pico-projects/web_server/build/picotool/src/picotoolBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/ahmed/pico-projects/web_server/build/picotool/src/picotoolBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/ahmed/pico-projects/web_server/build/picotool/src/picotoolBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
