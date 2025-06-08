#pragma once

#include "glm_common.h"
#include <string>

namespace golxx {
    struct GameConfig {
        // Visual settings
        glm::vec3 liveCellColor{1.0f, 1.0f, 1.0f};
        glm::vec3 backgroundColor{0.1f, 0.4f, 0.7f};

        // Camera settings
        float initialZoom = 20.0f;

        // Player movement
        float playerSpeed = 50.0f;
    };

    class ConfigManager {
    public:
        static ConfigManager& get();

        bool loadFromFile(const std::string& filename = "config.json");
        bool saveToFile(const std::string& filename = "config.json") const;

        const GameConfig& getConfig() const { return config_; }
        GameConfig& getConfig() { return config_; }

    private:
        ConfigManager() = default;
        GameConfig config_;

        void setDefaults();
        bool parseJSON(const std::string& jsonContent);
        std::string generateJSON() const;
    };
}
