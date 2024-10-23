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

struct ViewBounds {
    float left;
    float right;
    float bottom;
    float top;
};

float widthF, heightF, windowAspectRatio;

Shader *shader;
Mesh *mesh;
Camera camera;
glm::mat4 projection;
float cellSize = 1.0f;
std::unordered_set<glm::ivec2> liveCells;
glm::ivec2 lastCellPosition;
unsigned int iteration;

bool isAutoUpdate = false;
float updateIntervalS = 0.5f;
float lastUpdateS = 0.0f;

glm::vec3 liveCellColor = glm::vec3(1.0f, 1.0f, 1.0f);

ViewBounds get_camera_bounds() {
    float halfWidth = windowAspectRatio * camera.zoomLevel;
    float halfHeight = camera.zoomLevel;

    ViewBounds bounds{};
    bounds.left = camera.position.x - halfWidth;
    bounds.right = camera.position.x + halfWidth;
    bounds.bottom = camera.position.y - halfHeight;
    bounds.top = camera.position.y + halfHeight;

    return bounds;
}

Game::~Game() {
    shader = nullptr;
    mesh = nullptr;

    ShaderManager::Clear();
    MeshManager::Clear();
}

GLuint bufferVBO;

void Game::Initialize() {
    load_shaders();
    load_meshes();

    shader = &ShaderManager::Get("default");
    shader->use();
    shader->setVec3f("color", liveCellColor);

    mesh = &MeshManager::Get("quad");

    std::vector<AttributeLayout> layouts = {
            {3, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0, 1}
    };

    bufferVBO = mesh->GenerateInstanceBuffer();
    mesh->SetInstanceBufferAttributes(bufferVBO, layouts);
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
        iteration = 0;
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

    // Get the camera's visible bounds
    ViewBounds bounds = get_camera_bounds();

    int count = 0;
    std::vector<glm::vec2> cells{};
    for (const auto &liveCell: liveCells) {
        // Get the cell's world position
        float cellX = static_cast<float>(liveCell.x) * cellSize;
        float cellY = static_cast<float>(liveCell.y) * cellSize;

        // Check if the cell is within the visible bounds of the camera
        if (cellX + cellSize < bounds.left || cellX > bounds.right ||
            cellY + cellSize < bounds.bottom || cellY > bounds.top) {
            continue; // Skip rendering this cell, it's outside the view
        }

        cells.emplace_back(cellX, cellY);
    }

    mesh->PutInstanceBufferData(bufferVBO,
                                sizeof(float) * 2 * cells.size(),
                                cells.data(),
                                glfwxx::BufferUsage::DynamicDraw);


    shader->use();
    shader->setMatrix4fv("projection", camera.projection);
    shader->setMatrix4fv("view", glm::translate(glm::identity<glm::mat4>(), -camera.position));

    shader->setVec3f("color", liveCellColor);
    auto model = glm::identity<glm::mat4>();
    model = glm::scale(model, glm::vec3(cellSize, cellSize, 1.0f));
    model = glm::translate(model, glm::vec3(0.5f));

    shader->setMatrix4fv("model", model);
    mesh->DrawInstanced(cells.size());
}

size_t Game::GetLiveCellsCount() const {
    return liveCells.size();
}

unsigned int Game::GetIteration() const {
    return iteration;
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
    ++iteration;
}
