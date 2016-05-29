cmake_minimum_required(VERSION 2.6)
project(GRAVITY)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# --- Definitions ---

set(GRAVITY_VERSION "0.1-dev")
set(GRAVITY_MAJOR_VERSION 0)
set(GRAVITY_MINOR_VERSION 1)

# --- Compile options ---

option(GRAVITY_STATIC_VARIANT "Build static and minimal variant" OFF)
option(GRAVITY_STATIC_DEPENDENCIES "Whether the dependency libraries have been built as static or shared" OFF)
option(GRAVITY_COPY_DLLS "Whether to copy DLL files to build directory (only for development)" OFF)
option(GRAVITY_SYMLINK_DATA "Whether to symlink data dir to build directory (only for development) " OFF)

# -- Targets ---

set(GRAVITY_HEADERS gravity)
set(GRAVITY_DEV_LIBRARY dev)
set(GRAVITY_GAME_LIBRARY game)
set(GRAVITY_CGAME_LIBRARY cgame)
set(GRAVITY_CLIENT_EXECUTABLE client)
set(GRAVITY_SERVER_EXECUTABLE server)
set(GRAVITY_DATA data)
set(EXTLIBS extlibs)
set(EXTLIBS_GAME extlibs-game)
set(EXTLIBS_DEV extlibs-dev)

# --- Project files ---

file(GLOB GRAVITY_H "include/gravity/*.hpp")

file(GLOB_RECURSE GRAVITY_DEV_H "include/gravity/dev/*.hpp")
file(GLOB_RECURSE GRAVITY_DEV_SRC "src/dev/*.cpp")

file(GLOB_RECURSE GRAVITY_GAME_H "include/gravity/game/*.hpp" "include/gravity/game/*.inl")
file(GLOB_RECURSE GRAVITY_GAME_SRC "src/game/*.cpp")

file(GLOB_RECURSE GRAVITY_CGAME_H "include/gravity/cgame/*.hpp")
file(GLOB_RECURSE GRAVITY_CGAME_SRC "src/cgame/*.cpp")

file(GLOB_RECURSE GRAVITY_CLIENT_H "include/gravity/client/*.hpp")
file(GLOB_RECURSE GRAVITY_CLIENT_SRC "src/client/*.cpp")

file(GLOB_RECURSE GRAVITY_SERVER_H "include/gravity/server/*.hpp")
file(GLOB_RECURSE GRAVITY_SERVER_SRC "src/server/*.cpp")

file(GLOB_RECURSE EXTLIBS_GAME_H "extlibs/clipper/*.hpp")
file(GLOB_RECURSE EXTLIBS_GAME_SRC "extlibs/clipper/*.cpp")

file(GLOB_RECURSE EXTLIBS_DEV_H
    "extlibs/nanosvg/*.h"
    "extlibs/physfs/*.hpp"
    "extlibs/poly2tri/*.h"
    "extlibs/rapidxml/*.hpp"
	"extlibs/stb_image/stb_image.h"
)
file(GLOB_RECURSE EXTLIBS_DEV_SRC
    "extlibs/nanosvg/*.c"
    "extlibs/physfs/*.cpp"
    "extlibs/poly2tri/*.cc"
)

#file(GLOB_RECURSE EXTLIBS_H ${EXTLIBS_DEV_H} ${EXTLIBS_GAME_H})
#file(GLOB_RECURSE EXTLIBS_SRC ${EXTLIBS_DEV_SRC} ${EXTLIBS_GAME_SRC})

file(GLOB_RECURSE GRAVITY_DATA_FILES "data/*")

# --- Targets ---

#add_custom_target(${EXTLIBS} SOURCES ${EXTLIBS})
add_custom_target(${GRAVITY_HEADERS} SOURCES ${GRAVITY_H})
add_custom_target(${GRAVITY_DATA} SOURCES ${GRAVITY_DATA_FILES})
add_library(${GRAVITY_DEV_LIBRARY} STATIC
    ${GRAVITY_DEV_SRC}
    ${GRAVITY_DEV_H}
    ${EXTLIBS_DEV_SRC}
    ${EXTLIBS_DEV_H}
)
add_library(${GRAVITY_GAME_LIBRARY} STATIC
    ${GRAVITY_GAME_SRC}
    ${GRAVITY_GAME_H}
    ${EXTLIBS_GAME_SRC} 
    ${EXTLIBS_GAME_H}
)
add_library(${GRAVITY_CGAME_LIBRARY} STATIC
    ${GRAVITY_CGAME_SRC}
    ${GRAVITY_CGAME_H}
)
add_executable(${GRAVITY_CLIENT_EXECUTABLE}
    ${GRAVITY_CLIENT_SRC}
    ${GRAVITY_CLIENT_H}
)
add_executable(${GRAVITY_SERVER_EXECUTABLE}
    ${GRAVITY_SERVER_SRC}
    ${GRAVITY_SERVER_H}
)

# Workaround for multiple definition of __ftol2_sse error
if (MSVC)
	set_target_properties(${GRAVITY_CLIENT_EXECUTABLE} PROPERTIES LINK_FLAGS_MINSIZEREL /FORCE:multiple)
endif()

# --- Source groups ---

set(SRCS
    ${GRAVITY_DEV_H}
    ${GRAVITY_DEV_SRC}
    ${GRAVITY_GAME_H}
    ${GRAVITY_GAME_SRC}
    ${GRAVITY_CGAME_H}
    ${GRAVITY_CGAME_SRC}
    ${GRAVITY_CLIENT_H}
    ${GRAVITY_CLIENT_SRC}
    ${GRAVITY_SERVER_H}
    ${GRAVITY_SERVER_SRC}
    ${GRAVITY_DATA_FILES}
    ${EXTLIBS_GAME_SRC}
    ${EXTLIBS_GAME_H}
    ${EXTLIBS_DEV_SRC}
    ${EXTLIBS_DEV_H}
)
set(ERASE_PREFIXES
    ${GRAVITY_HEADERS}
    ${GRAVITY_GAME_LIBRARY}
    ${GRAVITY_CGAME_LIBRARY}
    ${GRAVITY_CLIENT_EXECUTABLE}
    ${GRAVITY_SERVER_EXECUTABLE}
    ${GRAVITY_DATA_FILES}
)

function(gravity_configure_source_group FILES ERASE_PREFIXES)
    foreach(FILE ${FILES})
        get_filename_component(PARENT_PATH "${FILE}" PATH)

        string(REGEX REPLACE "^${CMAKE_SOURCE_DIR}\\/" "" GROUP ${PARENT_PATH})

        string(REGEX REPLACE "^src\\/" "" GROUP "${GROUP}") 
        string(REGEX REPLACE "^include\\/" "" GROUP "${GROUP}") 

        #foreach(ERASE_PREFIX ${ERASE_PREFIXES})
        #    message("- ${ERASE_PREFIX}/ weg van ${GROUP}")
        #    string(REGEX REPLACE "^${ERASE_PREFIX}\\/?" "" GROUP "${GROUP}") 
        #endforeach()

        string(REPLACE "/" "\\" GROUP "${GROUP}")

        if ("${FILE}" MATCHES "(.*\\.cpp)|(.*\\.cc)|.*\\.c")
            set(GROUP "Source Files\\${GROUP}")
        elseif("${FILE}" MATCHES "(.*\\.h)|(.*\\.hpp)|(.*\\.hh)|(.*\\.inl)")
            set(GROUP "Header Files\\${GROUP}")
        else()
            set(GROUP "Data Files\\${GROUP}")
        endif()

        source_group("${GROUP}" FILES "${FILE}")

    endforeach()
endfunction()

gravity_configure_source_group("${SRCS}" "${ERASE_PREFIXES}")

# --- Include directories ---

include_directories("include")
include_directories(SYSTEM "extlibs/physfs/include")
include_directories(SYSTEM "extlibs/poly2tri")
include_directories(SYSTEM "extlibs/nanosvg")
include_directories(SYSTEM "extlibs/rapidxml")
include_directories(SYSTEM "extlibs/stb_image")

# --------------

#target_link_libraries(${GRAVITY_GAME_LIBRARY}
#    ${GRAVITY_LIB_LIBRARY})
#target_link_libraries(${GRAVITY_CLIENT_EXECUTABLE}
#    ${GRAVITY_LIB_LIBRARY} ${GRAVITY_GAME_LIBRARY} ${GRAVITY_CGAME_LIBRARY})
#target_link_libraries(${GRAVITY_SERVER_EXECUTABLE}
#    ${GRAVITY_LIB_LIBRARY} ${GRAVITY_GAME_LIBRARY})

set(GRAVITY_GAME_FLAGS -DGRAVITY_GAME)
set(GRAVITY_CGAME_FLAGS -DGRAVITY_CGAME)
set(GRAVITY_CLIENT_FLAGS -DGRAVITY_CLIENT)
set(GRAVITY_SERVER_FLAGS -DGRAVITY_SERVER)
if (NOT GRAVITY_STATIC_VARIANT)
    set(GRAVITY_GAME_FLAGS "${GRAVITY_GAME_FLAGS} -DGRAVITY_GAME_EXPORTS")
    set(GRAVITY_CGAME_FLAGS "${GRAVITY_CGAME_FLAGS} -DGRAVITY_CGAME_EXPORTS")
   # add_definitions(-DBOOST_ALL_DYN_LINK)
   # add_definitions(-DGLEW_STATIC) #temporary <?
endif()
if (GRAVITY_STATIC_DEPENDENCIES)
	add_definitions(-DGLEW_STATIC)
else()
	add_definitions(-DYAML_CPP_DLL)
endif()

set_target_properties(${GRAVITY_GAME_LIBRARY} PROPERTIES COMPILE_FLAGS ${GRAVITY_GAME_FLAGS})
set_target_properties(${GRAVITY_CGAME_LIBRARY} PROPERTIES COMPILE_FLAGS ${GRAVITY_CGAME_FLAGS})
set_target_properties(${GRAVITY_CLIENT_EXECUTABLE} PROPERTIES COMPILE_FLAGS ${GRAVITY_CLIENT_FLAGS})
set_target_properties(${GRAVITY_SERVER_EXECUTABLE} PROPERTIES COMPILE_FLAGS ${GRAVITY_SERVER_FLAGS})

#source_group("Header files" FILES ${GRAVITY_H})
#source_group("Source files" FILES ${GRAVITY_STUDIO_SRC})
#source_group("Source files" FILES ${GRAVITY_GAME_SRC})
#source_group("Source files" FILES ${GRAVITY_CGAME_SRC})
#source_group("Source files" FILES ${GRAVITY_CLIENT_SRC})
#source_group("Source files" FILES ${GRAVITY_SERVER_SRC})



foreach(FILE
#    ${GRAVITY_DEV_H}
#    ${GRAVITY_DEV_SRC}
#    ${GRAVITY_GAME_H}
#    ${GRAVITY_GAME_SRC}
#    ${GRAVITY_CGAME_H}
#    ${GRAVITY_CGAME_SRC}
#    ${GRAVITY_CLIENT_H}
#    ${GRAVITY_CLIENT_SRC}
#    ${GRAVITY_SERVER_H}
#    ${GRAVITY_SERVER_SRC}
#    ${GRAVITY_DATA_FILES}
#    ${EXTLIBS_GAME_SRC}
#    ${EXTLIBS_GAME_H}
#    ${EXTLIBS_DEV_SRC}
#    ${EXTLIBS_DEV_H}
    )

  get_filename_component(PARENT_DIR "${FILE}" PATH)

   # skip src and include dir
    string(REPLACE "${CMAKE_SOURCE_DIR}/include/" "" GROUP ${PARENT_DIR})
    string(REPLACE "${CMAKE_SOURCE_DIR}/src/" "" GROUP ${GROUP})
    string(REPLACE "${CMAKE_SOURCE_DIR}/" "" GROUP ${GROUP})

  # skip src or include and changes /'s to \\'s
  #string(REGEX REPLACE "(\\./)?(src|include)/?" "" GROUP "${PARENT_DIR}")
   # string(REGEX REPLACE "
  string(REPLACE "/" "\\" GROUP "${GROUP}")

message("GROUP ${GROUP}")

  # group into "Source Files" and "Header Files"
  if ("${FILE}" MATCHES "(.*\\.cpp)|(.*\\.cc)|.*\\.c")
    set(GROUP "Source Files\\${GROUP}")
  elseif("${FILE}" MATCHES "(.*\\.h)|(.*\\.hpp)")
    set(GROUP "Header Files\\${GROUP}")
  endif()

  source_group("${GROUP}" FILES "${FILE}")
endforeach()

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /MP /fp:fast")
    #add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(/utf-8 /wd4828 /EHsc)
	#wut??
	#if (CMAKE_BUILD_TYPE EQUAL Debug)
	#	add_definitions(/MDd)
	#endif()
else()
    set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wstrict-aliasing -pedantic -ffast-math")
endif()

add_definitions(-DGLM_FORCE_RADIANS=1)
add_definitions(-DGRAVITY_COMPILETIME_SOURCE_PATH="${CMAKE_SOURCE_DIR}")
add_definitions(-DBOOST_ALL_NO_LIB=1)

find_package(Threads REQUIRED)
#find_package(Boost COMPONENTS thread program_options system filesystem date_time)
find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
find_package(Box2D REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
if (UNIX)
set(SDL2_BUILDING_LIBRARY TRUE)
endif()
find_package(SDL2 REQUIRED)
find_package(JsonCpp REQUIRED)
find_package(PhysFS REQUIRED)
find_package(GLM REQUIRED)
find_package(CEGUI REQUIRED)
#find_package(LOGOG REQUIRED)
find_package(YAMLCPP REQUIRED)
find_package(Freetype REQUIRED)
#find_package(MyGUI REQUIRED)

#message(${Boost_LIBRARIES})
#message(${BOX2D_LIBRARY})
#message(${OPENGL_LIBRARY})
#message(${GLEW_LIBRARY})
#message(${SDL2_LIBRARY})
#message(${JSONCPP_LIBRARY})
#message(${PHYSFS_LIBRARY})
##message(${CEGUI_LIBRARY})
##message(${LOGOG_LIBRARY})
#message(${YAMLCPP_LIBRARY})

include_directories(SYSTEM ${Boost_INCLUDE_DIR})
include_directories(SYSTEM ${BOX2D_INCLUDE_DIR})
include_directories(SYSTEM ${OPENGL_INCLUDE_DIR})
include_directories(SYSTEM ${GLEW_INCLUDE_DIR})
include_directories(SYSTEM ${SDL2_INCLUDE_DIR})
include_directories(SYSTEM ${JSONCPP_INCLUDE_DIR})
include_directories(SYSTEM ${PHYSFS_INCLUDE_DIR})
include_directories(SYSTEM ${GLM_INCLUDE_DIR})
#include_directories(SYSTEM ${LOGOG_INCLUDE_DIR})
include_directories(SYSTEM ${YAMLCPP_INCLUDE_DIR})
include_directories(SYSTEM ${CEGUI_INCLUDE_DIR}/cegui-0)
#include_directories(SYSTEM ${MYGUI_INCLUDE_DIR})

#target_link_libraries(${GRAVITY_DEV_LIBRARY}
#    ${CMAKE_THREAD_LIBS_INIT}
#    ${Boost_LIBRARIES}
#)
target_link_libraries(${GRAVITY_GAME_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
	${Boost_LIBRARIES}
    ${GRAVITY_DEV_LIBRARY}
    ${PHYSFS_LIBRARY}
    ${BOX2D_LIBRARY}
    ${JSONCPP_LIBRARY}
    ${YAMLCPP_LIBRARY}
)

target_link_libraries(${GRAVITY_CGAME_LIBRARY}
    ${GRAVITY_GAME_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_LIBRARY}
    ${SDL2_LIBRARY}
	${FREETYPE_LIBRARY}
	#${MYGUI_LIBRARY}
	#${MYGUI_OPENGLSYSTEM_LIBRARY}
	${CEGUI_LIBRARIES}
)
target_link_libraries(${GRAVITY_CLIENT_EXECUTABLE}
    ${GLEW_LIBRARY} # temp?
    ${GRAVITY_CGAME_LIBRARY}
)
if (NOT UNIX)
    target_link_libraries(
        ${GRAVITY_CGAME_LIBRARY}
        ${SDL2MAIN_LIBRARY})
endif()
if (GRAVITY_STATIC_DEPENDENCIES AND WIN32)
	target_link_libraries(
		${GRAVITY_CGAME_LIBRARY}
		 opengl32 imm32 version winmm
	)
endif()

target_link_libraries(${GRAVITY_SERVER_EXECUTABLE}
    ${GRAVITY_GAME_LIBRARY}
)

if (GRAVITY_COPY_DLLS AND WIN32)
	file(GLOB GRAVITY_DEPENDENCY_DLLS
		${CMAKE_PREFIX_PATH}/lib/*.dll
        ${CMAKE_PREFIX_PATH}/bin/*.dll
	)

	foreach(GRAVITY_DEPENDENCY_DLL ${GRAVITY_DEPENDENCY_DLLS})
		add_custom_command(TARGET ${GRAVITY_CLIENT_EXECUTABLE} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${GRAVITY_DEPENDENCY_DLL}"
				$<TARGET_FILE_DIR:${GRAVITY_CLIENT_EXECUTABLE}>
		)
	endforeach()
endif()

if (GRAVITY_SYMLINK_DATA AND WIN32)
	#add_custom_command(TARGET ${GRAVITY_CLIENT_EXECUTABLE} POST_BUILD
	#	COMMAND ${CMAKE_COMMAND} -E make_directory
	#		"$<TARGET_FILE_DIR:${GRAVITY_CLIENT_EXECUTABLE}>/data"
	#)

	#add_custom_command(TARGET ${GRAVITY_CLIENT_EXECUTABLE} POST_BUILD
	#	COMMAND ${CMAKE_COMMAND} -E copy_directory
	#		"${CMAKE_SOURCE_DIR}/data"
	#		"$<TARGET_FILE_DIR:${GRAVITY_CLIENT_EXECUTABLE}>/data"
	#)
	
	add_custom_command(TARGET ${GRAVITY_CLIENT_EXECUTABLE} POST_BUILD
		COMMAND if not exist "\"$<TARGET_FILE_DIR:${GRAVITY_CLIENT_EXECUTABLE}>/data\"" mklink /J
			"\"$<TARGET_FILE_DIR:${GRAVITY_CLIENT_EXECUTABLE}>/data\""
			"\"${CMAKE_SOURCE_DIR}/data\""
	)
endif()

#set(CMAKE_C_COMPILER "/usr/bin/clang")
