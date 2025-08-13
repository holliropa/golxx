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
        src/golxx/background.cpp
        src/golxx/config_manager.cpp
        src/golxx/cube.cpp
        src/golxx/golxx_game.cpp
        src/golxx/grid_renderer.cpp
        src/golxx/player.cpp
        src/golxx/simulator.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        include
#        extern/glad/include
#        extern/glad-wrap/include
#        extern/glfw-wrap/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        bw::engine
)
