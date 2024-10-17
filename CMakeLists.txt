cmake_minimum_required(VERSION 3.28)
project("golxx")

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(extern)
add_subdirectory(src)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm::glm glad)

set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets")
set(ASSETS_DEST_DIR "${CMAKE_BINARY_DIR}/assets")

# Create a custom target for copying assets
add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_SOURCE_DIR} ${ASSETS_DEST_DIR}
        COMMENT "Copying assets directory to build folder"
)

# Make the executable depend on the custom target
add_dependencies(${PROJECT_NAME} copy_assets)
