cmake_minimum_required(VERSION 3.28)
project("golxx")

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")

include(FetchContent)

add_subdirectory(libs/bw-engine)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/game-of-life/node_factory/factory.cpp
        src/game-of-life/node_factory/hash.cpp
        src/game-of-life/node_factory/key.cpp
        src/game-of-life/hashlife.cpp
        src/game-of-life/node.cpp
        src/background.cpp
        src/cube.cpp
        src/config_manager.cpp
        src/golxx_game.cpp
        src/grid_renderer.cpp
        src/player.cpp
        src/simulator.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        bw::engine
)
