#include <iostream>
#include <set>
#include "game.hpp"
#include "shader_manager.hpp"
#include "mesh_manager.hpp"
#include "files.hpp"

void load_shaders();

void load_meshes();

void do_step();

Shader *shader;
Mesh *mesh;
glm::mat4 projection;
unsigned int size;
float cellSize;
std::map<unsigned int, bool> states;
unsigned int lastCellIndex = -1;
std::set<unsigned int> liveCellIndexes;
glm::vec3 liveCellColor = glm::vec3(1.0f, 1.0f, 1.0f);
float widthF, heightF;
bool isAutoUpdate = false;
float updateIntervalS = 0.5f;
float lastUpdateS = 0.0f;

Game::Game(unsigned int width, unsigned int height)
        : Width(width),
          Height(height) {
    projection = glm::ortho(
            0.0f, 1.0f,  // Left, Right
            1.0f, 0.0f,  // Bottom, Top
            -1.0f, 1.0f   // Near, Far
    );

    widthF = static_cast<float>(width);
    heightF = static_cast<float>(height);
}

Game::~Game() {
    shader = nullptr;
    mesh = nullptr;

    ShaderManager::Clear();
    MeshManager::Clear();
}

void Game::Initialize() {
    load_shaders();
    load_meshes();

    auto defaultShader = &ShaderManager::Get("default");
    defaultShader->use();
    defaultShader->setVec3f("color", glm::vec3(1.0f, 0.0f, 1.0f));
    defaultShader->setMatrix4fv("projection", projection);
    shader = defaultShader;

    mesh = &MeshManager::Get("quad");

    size = 100;
    cellSize = 1.0f / static_cast<float>(size);
}

void Game::Resize(unsigned int width, unsigned int height) {
    this->Width = width;
    this->Height = height;

    widthF = static_cast<float>(width);
    heightF = static_cast<float>(height);
}

void Game::Update(float deltaTime) {
    if (DownKeys[glfwxx::KeyCode::Escape])
        this->IsExit = true;

    if (PressedButtons[glfwxx::ButtonCode::Left]) {
        if (CursorPosition.x < widthF &&
            CursorPosition.y < heightF &&
            CursorPosition.x > 0.0f &&
            CursorPosition.y > 0.0f) {

            auto pos = glm::vec2(CursorPosition.x / widthF, CursorPosition.y / heightF);
            auto column = static_cast<unsigned int>(pos.x / cellSize);
            auto row = static_cast<unsigned int>(pos.y / cellSize);
            unsigned int cellIndex = row * size + column;

            if (lastCellIndex != cellIndex) {
                lastCellIndex = cellIndex;
                auto cellState = !states[cellIndex];
                states[cellIndex] = cellState;

                if (cellState) {
                    liveCellIndexes.insert(cellIndex);
                } else {
                    liveCellIndexes.erase(cellIndex);
                }
            }
        }
    } else {
        lastCellIndex = -1;
    }

    if (DownKeys[glfwxx::KeyCode::LeftShift]) {
        do_step();
    }

    if (PressedKeys[glfwxx::KeyCode::Tab]) {
        do_step();
    }

    if (DownKeys[glfwxx::KeyCode::Space]) {
        isAutoUpdate = !isAutoUpdate;
    }

    if (DownKeys[glfwxx::KeyCode::C]) {
        states.clear();
        liveCellIndexes.clear();
    }

    if (isAutoUpdate) {
        if (lastUpdateS > updateIntervalS) {
            do_step();
            lastUpdateS = 0.0f;
        }

        lastUpdateS += deltaTime;
    }
}

void Game::Render() {
    shader->use();
    for (const auto &liveCellIndex: liveCellIndexes) {
        unsigned int row = liveCellIndex / size;
        unsigned int column = liveCellIndex % size;
        auto cellPosition = glm::vec2(static_cast<float>(column) * cellSize,
                                      static_cast<float>(row) * cellSize);

        auto model = glm::identity<glm::mat4>();
        model = glm::translate(model, glm::vec3(glm::vec3(cellPosition, 0.0f)));
        model = glm::scale(model, glm::vec3(cellSize, cellSize, 1.0f));
        model = glm::translate(model, glm::vec3(0.5f));
        shader->setMatrix4fv("model", model);

        shader->setVec3f("color", liveCellColor);

        mesh->draw();
    }
}

void load_shaders() {
    auto default_vsFileContent = utils::readFile("assets/shaders/default.vert");
    auto default_fsFileContent = utils::readFile("assets/shaders/default.frag");

    ShaderManager::Load(default_vsFileContent.c_str(),
                        default_fsFileContent.c_str(),
                        "default");
}

void load_meshes() {
    const std::vector<float> quadVertices = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f
    };

    const std::vector<float> quadNormals = {
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f
    };

    const std::vector<float> quadUVs = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f
    };

    const std::vector<unsigned int> quadTriangles = {
            0, 1, 2,
            0, 2, 3
    };

    MeshManager::Load(quadVertices,
                      quadNormals,
                      quadUVs,
                      quadTriangles,
                      "quad");
}

void do_step() {
    std::set<unsigned int> nextLiveCellIndexes;
    std::set<unsigned int> activeCells; // Includes live cells and their neighbors

    // Neighbor offsets: [x, y] positions relative to the current cell
    std::vector<std::pair<int, int>> neighborOffsets = {
            {-1, -1},
            {-1, 0},
            {-1, 1},
            {0,  -1},
            {0,  1},
            {1,  -1},
            {1,  0},
            {1,  1}
    };

    // Add live cells and their neighbors to the active cell set
    for (const auto &liveCellIndex: liveCellIndexes) {
        unsigned int row = liveCellIndex / size;
        unsigned int col = liveCellIndex % size;

        // Add the live cell itself
        activeCells.insert(liveCellIndex);

        // Add its neighbors
        for (const auto &offset: neighborOffsets) {
            int neighborRow = row + offset.first;
            int neighborCol = col + offset.second;

            if (neighborRow >= 0 && neighborRow < size &&
                neighborCol >= 0 && neighborCol < size) {
                activeCells.insert(neighborRow * size + neighborCol);
            }
        }
    }

    // Process only the active cells
    for (const auto &cellIndex: activeCells) {
        unsigned int row = cellIndex / size;
        unsigned int col = cellIndex % size;
        int liveNeighbors = 0;

        // Check all neighbors
        for (const auto &offset: neighborOffsets) {
            int neighborRow = row + offset.first;
            int neighborCol = col + offset.second;

            if (neighborRow >= 0 && neighborRow < size &&
                neighborCol >= 0 && neighborCol < size &&
                states[neighborRow * size + neighborCol]) {
                liveNeighbors++;
            }
        }

        // Apply Game of Life rules
        if (states[cellIndex]) { // Cell is alive
            if (liveNeighbors < 2 || liveNeighbors > 3) {
                nextLiveCellIndexes.erase(cellIndex); // Cell dies
            } else {
                nextLiveCellIndexes.insert(cellIndex); // Cell lives
            }
        } else { // Cell is dead
            if (liveNeighbors == 3) {
                nextLiveCellIndexes.insert(cellIndex); // Cell becomes alive
            }
        }
    }

    // Update the live cell index set and states
    liveCellIndexes = nextLiveCellIndexes;
    states.clear();
    for (const auto &cellIndex: liveCellIndexes) {
        states[cellIndex] = true;
    }
}
//void do_step() {
//    std::vector<bool> nextStates(size * size, false); // Create a new state array for the next step
//
//    // Neighbor offsets: [x, y] positions relative to the current cell
//    std::vector<std::pair<int, int>> neighborOffsets = {
//            {-1, -1},   // Top-left
//            {-1, 0},    // Top
//            {-1, 1},    // Top-right
//            {0,  -1},   // Left
//            {0,  1},    // Right
//            {1,  -1},   // Bottom-left
//            {1,  0},    // Bottom
//            {1,  1}     // Bottom-right
//    };
//
//    for (unsigned int row = 0; row < size; ++row) {
//        for (unsigned int col = 0; col < size; ++col) {
//            unsigned int cellIndex = row * size + col;
//            int liveNeighbors = 0;
//
//            // Check all neighbors
//            for (const auto &offset: neighborOffsets) {
//                int neighborRow = row + offset.first;
//                int neighborCol = col + offset.second;
//
//                // Ensure the neighbor indices are within bounds
//                if (neighborRow >= 0 && neighborRow < size &&
//                    neighborCol >= 0 && neighborCol < size) {
//                    // Count live neighbors
//                    if (states[neighborRow * size + neighborCol]) {
//                        liveNeighbors++;
//                    }
//                }
//            }
//
//            // Apply Game of Life rules
//            if (states[cellIndex]) { // Cell is alive
//                if (liveNeighbors < 2 || liveNeighbors > 3) {
//                    nextStates[cellIndex] = false; // Cell dies
//                    liveCellIndexes.erase(cellIndex);
//                } else {
//                    nextStates[cellIndex] = true; // Cell lives
//                    liveCellIndexes.insert(cellIndex);
//                }
//            } else { // Cell is dead
//                if (liveNeighbors == 3) {
//                    nextStates[cellIndex] = true; // Cell becomes alive
//                    liveCellIndexes.insert(cellIndex);
//                }
//            }
//        }
//    }
//
//    // Update states to the new calculated states
//    states = nextStates;
//}
