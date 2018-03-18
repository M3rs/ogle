project(ogle) 

# put at top, for some reason, see: 
# https://stackoverflow.com/questions/571292/how-do-i-fix-an-apparently-corrupted-cmake-build

cmake_minimum_required(VERSION 3.1)

IF (APPLE)
	# INCLUDE_DIRECTORIES(/System/Library/Frameworks)
	# FIND_PACKAGE(OpenGL_LIBRARY OpenGL)
	# MARK_AS_ADVANCED(OpenGL_LIBRARY)
	FIND_PACKAGE(OpenGL REQUIRED)
	SET(GLLIB ${OPENGL_LIBRARY})
ENDIF (APPLE)

find_library(OAL OpenAL)    

# "FORCE" clears the cache (which has been causing a lot of issues
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -g" CACHE STRING "compile flags" FORCE)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add_definitions(-DDEBUG_LOG)

set(BREW /usr/local/Cellar)
set(SDL ${BREW}/sdl2/2.0.5/)
set(AIMP ~/Documents/src/clones/assimp)
set(FT ${BREW}/freetype/2.8.1/)
#set(IMG ${BREW}/sdl2_image/2.0.1_2)
#set(LUA ${BREW}/lua/5.2.4_4)

include_directories(
	"${SDL}/include"
	"${SDL}/include/SDL2"
	#"${IMG}/include"
	#"${LUA}/include"
	"${AIMP}/include"
	"~/Documents/src/ogle/include"
	"${FT}/include/freetype2"
	"~/Documents/src/ogle/include/imgui"
)

link_directories(
"${SDL}/lib"
"${AIMP}/lib"
#"${IMG}/lib"
#"${LUA}/lib"
"${FT}/lib"
)

# GLOB_RECURSE will search all sub directories in src! (super cool)
file (GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")

add_executable(ogle ${SOURCES})

target_link_libraries(ogle sdl2 ${GLLIB} assimp lua freetype ${OAL}) # sdl2_image lua)
