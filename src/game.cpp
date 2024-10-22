#include <iostream>
#include <unordered_set>
#include "game.hpp"
#include "shader_manager.hpp"
#include "mesh_manager.hpp"
#include "files.hpp"

void update_screen_size(unsigned int width, unsigned int height);

void update_projection();

void load_shaders();

void load_meshes();

void do_step();

struct Camera {
    glm::vec3 position{};
    glm::mat4 projection{};
    float zoomLevel = 500.0f;
};

float widthF, heightF, windowAspectRatio;

Shader *shader;
Mesh *mesh;
Camera camera;
glm::mat4 projection;
float cellSize = 1.0f;
std::unordered_set<glm::ivec2> liveCells;
glm::ivec2 lastCellPosition;

bool isAutoUpdate = false;
float updateIntervalS = 0.5f;
float lastUpdateS = 0.0f;

glm::vec3 liveCellColor = glm::vec3(1.0f, 1.0f, 1.0f);

Game::~Game() {
    shader = nullptr;
    mesh = nullptr;

    ShaderManager::Clear();
    MeshManager::Clear();
}

void Game::Initialize() {
    load_shaders();
    load_meshes();

    shader = &ShaderManager::Get("default");
    shader->use();
    shader->setVec3f("color", liveCellColor);

    mesh = &MeshManager::Get("quad");
}

void Game::Resize(unsigned int width, unsigned int height) {
    this->Width = width;
    this->Height = height;

    update_screen_size(width, height);
}

void Game::Update(float deltaTime) {
    if (DownKeys[glfwxx::KeyCode::Escape])
        this->IsExit = true;


    if (PressedButtons[glfwxx::ButtonCode::Left]) {
        if (CursorPosition.x < widthF &&
            CursorPosition.y < heightF &&
            CursorPosition.x > 0.0f &&
            CursorPosition.y > 0.0f) {

            float ndcX = (2.0f * CursorPosition.x) / widthF - 1.0f;
            float ndcY = 1.0f - (2.0f * CursorPosition.y) / heightF; // Inverted Y-axis for NDC
            glm::vec4 ndcPos(ndcX, ndcY, 0.0f, 1.0f);

            glm::mat4 inverseProjView = glm::inverse(
                    camera.projection * glm::translate(glm::identity<glm::mat4>(), -camera.position));
            glm::vec4 worldPos = inverseProjView * ndcPos;

            worldPos /= worldPos.w;

            auto column = static_cast<int>(std::floor(worldPos.x));
            auto row = static_cast<int>(std::floor(worldPos.y));
            auto cellPosition = glm::ivec2(column, row);

            if (cellPosition != lastCellPosition) {
                lastCellPosition = cellPosition;
                if (liveCells.find(cellPosition) != liveCells.end()) {
                    liveCells.erase(cellPosition);
                } else {
                    liveCells.insert(cellPosition);
                }
            }
        }
    } else {
        lastCellPosition = glm::ivec2(-1);
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
        liveCells.clear();
    }

    float baseSpeed = 1.0f;
    float speedFactor = baseSpeed * camera.zoomLevel * deltaTime;  // Adjust camera speed based on zoom level
    if (PressedKeys[glfwxx::KeyCode::W]) {
        camera.position.y += speedFactor;
    }
    if (PressedKeys[glfwxx::KeyCode::S]) {
        camera.position.y -= speedFactor;
    }
    if (PressedKeys[glfwxx::KeyCode::D]) {
        camera.position.x += speedFactor;
    }
    if (PressedKeys[glfwxx::KeyCode::A]) {
        camera.position.x -= speedFactor;
    }

    if (PressedKeys[glfwxx::KeyCode::Z]) {
        camera.zoomLevel -= 0.1f;  // Zoom in
        if (camera.zoomLevel < 0.1f) camera.zoomLevel = 0.1f;  // Prevent excessive zoom in
        update_projection();
    }
    if (PressedKeys[glfwxx::KeyCode::X]) {
        camera.zoomLevel += 0.1f;  // Zoom out
        update_projection();
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
    shader->setMatrix4fv("projection", camera.projection);
    shader->setMatrix4fv("view", glm::translate(glm::identity<glm::mat4>(), -camera.position));


    shader->setVec3f("color", liveCellColor);
    for (const auto &liveCell: liveCells) {
        auto cellPosition = glm::vec2(static_cast<float>(liveCell.x) * cellSize,
                                      static_cast<float>(liveCell.y) * cellSize);

        auto model = glm::identity<glm::mat4>();
        model = glm::translate(model, glm::vec3(cellPosition, 0.0f));
        model = glm::scale(model, glm::vec3(cellSize, cellSize, 1.0f));
        model = glm::translate(model, glm::vec3(0.5f));
        shader->setMatrix4fv("model", model);

        mesh->draw();
    }
}

size_t Game::GetLiveCellsCount() const {
    return liveCells.size();
}

void update_screen_size(unsigned int width, unsigned int height) {
    widthF = static_cast<float>(width);
    heightF = static_cast<float>(height);

    windowAspectRatio = widthF / heightF;

    update_projection();
}

void update_projection() {
    camera.projection = glm::ortho(
            -1.0f * windowAspectRatio * camera.zoomLevel, 1.0f * windowAspectRatio * camera.zoomLevel,
            -camera.zoomLevel, camera.zoomLevel
    );
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
    std::unordered_set<glm::ivec2> nextLiveCell;
    std::unordered_set<glm::ivec2> activeCells; // Includes live cells and their neighbors

    // Neighbor offsets: [x, y] positions relative to the current cell
    std::vector<glm::ivec2> neighborOffsets = {
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
    for (const auto &liveCell: liveCells) {
        // Add the live cell itself
        activeCells.insert(liveCell);

        // Add its neighbors
        for (const auto &offset: neighborOffsets) {
            auto neighbourCell = liveCell + offset;
            activeCells.insert(neighbourCell);
        }
    }

    // Process only the active cells
    for (const auto &cellIndex: activeCells) {
        int liveNeighbors = 0;

        // Check all neighbors
        for (const auto &offset: neighborOffsets) {
            auto neighbourCell = cellIndex + offset;

            if (liveCells.find(neighbourCell) != liveCells.end()) {
                liveNeighbors++;
            }
        }

        // Apply Game of Life rules
        if (liveCells.find(cellIndex) != liveCells.end()) { // Cell is alive
            if (liveNeighbors < 2 || liveNeighbors > 3) {
                nextLiveCell.erase(cellIndex); // Cell dies
            } else {
                nextLiveCell.insert(cellIndex); // Cell lives
            }
        } else { // Cell is dead
            if (liveNeighbors == 3) {
                nextLiveCell.insert(cellIndex); // Cell becomes alive
            }
        }
    }

    // Update the live cell index set and states
    liveCells = nextLiveCell;
}
