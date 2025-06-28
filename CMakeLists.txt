cmake_minimum_required(VERSION 3.28)
project("golxx")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")

add_subdirectory(libs/bw-engine)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/game.cpp
        src/grid_renderer.cpp
        src/player.cpp
        src/simulator.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        bw-engine/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        bw::engine
)