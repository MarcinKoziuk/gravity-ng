cmake_minimum_required(VERSION 2.6)
project(GRAVITY)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

option(GRAVITY_STATIC_VARIANT "Build static and minimal variant" OFF)

set(GRAVITY_HEADERS gravity)
set(GRAVITY_GAME_LIBRARY game)
set(GRAVITY_CGAME_LIBRARY cgame)
set(GRAVITY_CLIENT_EXECUTABLE client)
set(GRAVITY_SERVER_EXECUTABLE server)

set(GRAVITY_VERSION "0.1-dev")

file(GLOB_RECURSE GRAVITY_H "include/gravity/*.hpp" "include/gravity/*.inl")
file(GLOB_RECURSE GRAVITY_GAME_SRC "src/game/*.cpp" "data/*")
file(GLOB_RECURSE GRAVITY_CGAME_SRC "src/cgame/*.cpp")
file(GLOB_RECURSE GRAVITY_CLIENT_SRC "src/client/*.cpp")
file(GLOB_RECURSE GRAVITY_SERVER_SRC "src/server/*.cpp")

file(GLOB_RECURSE EXTLIBS_H "extlibs/*.hpp" "extlibs/*.h")
file(GLOB_RECURSE EXTLIBS_SRC "extlibs/*.cpp" "extlibs/*.cc")

include_directories("include")
include_directories("extlibs/physfs/include" SYSTEM)
include_directories("extlibs/poly2tri" SYSTEM)

add_custom_target(${GRAVITY_HEADERS} SOURCES ${GRAVITY_H} ${EXTLIBS_H})
add_library(${GRAVITY_GAME_LIBRARY} SHARED ${GRAVITY_GAME_SRC} ${EXTLIBS_SRC})
add_library(${GRAVITY_CGAME_LIBRARY} SHARED ${GRAVITY_CGAME_SRC})
add_executable(${GRAVITY_CLIENT_EXECUTABLE} ${GRAVITY_CLIENT_SRC})
add_executable(${GRAVITY_SERVER_EXECUTABLE} ${GRAVITY_SERVER_SRC})

target_link_libraries(${GRAVITY_CLIENT_EXECUTABLE}
    ${GRAVITY_GAME_LIBRARY} ${GRAVITY_CGAME_LIBRARY})
target_link_libraries(${GRAVITY_SERVER_EXECUTABLE}
    ${GRAVITY_GAME_LIBRARY} ${GRAVITY_CGAME_LIBRARY})

set(GRAVITY_GAME_FLAGS -DGRAVITY_GAME)
set(GRAVITY_CGAME_FLAGS -DGRAVITY_CGAME)
set(GRAVITY_CLIENT_FLAGS -DGRAVITY_CLIENT)
set(GRAVITY_SERVER_FLAGS -DGRAVITY_SERVER)
if (NOT GRAVITY_STATIC_VARIANT)
    set(GRAVITY_GAME_FLAGS "${GRAVITY_GAME_FLAGS} -DGRAVITY_GAME_EXPORTS")
    set(GRAVITY_GAME_FLAGS "${GRAVITY_CGAME_FLAGS} -DGRAVITY_CGAME_EXPORTS")
    add_definitions(-DBOOST_ALL_DYN_LINK)
	add_definitions(-DGLEW_STATIC) #temporary
endif()

set_target_properties(${GRAVITY_GAME_LIBRARY} PROPERTIES COMPILE_FLAGS ${GRAVITY_GAME_FLAGS})
set_target_properties(${GRAVITY_CGAME_LIBRARY} PROPERTIES COMPILE_FLAGS ${GRAVITY_CGAME_FLAGS})
set_target_properties(${GRAVITY_CLIENT_EXECUTABLE} PROPERTIES COMPILE_FLAGS ${GRAVITY_CLIENT_FLAGS})
set_target_properties(${GRAVITY_SERVER_EXECUTABLE} PROPERTIES COMPILE_FLAGS ${GRAVITY_SERVER_FLAGS})

source_group("Header files" FILES ${GRAVITY_H})
source_group("Source files" FILES ${GRAVITY_GAME_SRC})
source_group("Source files" FILES ${GRAVITY_CGAME_SRC})
source_group("Source files" FILES ${GRAVITY_CLIENT_SRC})
source_group("Source files" FILES ${GRAVITY_SERVER_SRC})

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /MP /fp:fast")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    set(CMAKE_CXX_FLAGS "-std=c++98 -Wall -Wextra -Wstrict-aliasing -pedantic")
endif()
add_definitions(-DGLM_FORCE_RADIANS=1)

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS program_options thread filesystem date_time)
find_package(Box2D REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
set(SDL2_BUILDING_LIBRARY TRUE)
find_package(SDL2 REQUIRED)
find_package(JsonCpp REQUIRED)
find_package(PhysFS REQUIRED)
find_package(GLM REQUIRED)
#find_package(CEGUI REQUIRED)

message(${Boost_LIBRARIES})
message(${BOX2D_LIBRARY})
message(${OPENGL_LIBRARY})
message(${GLEW_LIBRARY})
message(${SDL2_LIBRARY})
message(${JSONCPP_LIBRARY})
message(${PHYSFS_LIBRARY})
#message(${CEGUI_LIBRARY})

include_directories(SYSTEM ${Boost_INCLUDE_DIR})
include_directories(SYSTEM ${BOX2D_INCLUDE_DIR})
include_directories(SYSTEM ${OPENGL_INCLUDE_DIR})
include_directories(SYSTEM ${GLEW_INCLUDE_DIR})
include_directories(SYSTEM ${SDL2_INCLUDE_DIR})
include_directories(SYSTEM ${JSONCPP_INCLUDE_DIR})
include_directories(SYSTEM ${PHYSFS_INCLUDE_DIR})
include_directories(SYSTEM ${GLM_INCLUDE_DIR})

target_link_libraries(${GRAVITY_GAME_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${BOX2D_LIBRARY}
    ${JSONCPP_LIBRARY}
    ${PHYSFS_LIBRARY}
    )
target_link_libraries(${GRAVITY_CGAME_LIBRARY}
    ${GRAVITY_GAME_LIBRARY}
    ${GLEW_LIBRARY}
	${OPENGL_LIBRARY}
    ${SDL2_LIBRARY}
    #${CEGUI_LIBRARY}
    )
target_link_libraries(${GRAVITY_CLIENT_EXECUTABLE}
    ${GRAVITY_CGAME_LIBRARY}
    )
target_link_libraries(${GRAVITY_SERVER_EXECUTABLE}
    ${GRAVITY_GAME_LIBRARY}
    )

set(CMAKE_C_COMPILER "/usr/bin/clang")
