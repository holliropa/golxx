message(STATUS "Fetching GLFW...")

#GLFW library
FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.9)
FetchContent_MakeAvailable(glfw)

message(STATUS "GLFW Fetched")
