message(STATUS "Fetching GLM...")

FetchContent_Declare(glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.0)
FetchContent_MakeAvailable(glm)

message(STATUS "GLM Fetched")
