project(sfx)

cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -g" CACHE STRING "compile flags" FORCE)

find_library(OAL OpenAL)

set (OGG /usr/local/Cellar/libvorbis/1.3.5)
include_directories(
  "${OGG}/include"
  )
link_directories(
  "${OGG}/lib"
  )

add_executable(sfx main.cpp wav.cpp sff.cpp source.cpp audio.cpp sfx.cpp ogg.cpp)

target_link_libraries(sfx ${OAL} vorbis vorbisfile)
