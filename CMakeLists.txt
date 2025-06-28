cmake_minimum_required(VERSION 3.28)
project("golxx")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")

include(FetchContent)

add_subdirectory(extern/glad)
add_subdirectory(extern/glfw)
add_subdirectory(extern/glm)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/golxx/application.cpp
        src/golxx/config_manager.cpp
        src/golxx/engine.cpp
        src/golxx/game.cpp
        src/golxx/grid_renderer.cpp
        src/golxx/input.cpp
        src/golxx/player.cpp
        src/golxx/simulator.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        include
        extern/glad/include
        extern/glad-wrap/include
        extern/glfw-wrap/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        glad
        glfw
        glm::glm
)