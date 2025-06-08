#include "golxx/config_manager.h"
#include <fstream>
#include <sstream>

namespace golxx {
    ConfigManager& ConfigManager::get() {
        static ConfigManager instance;
        return instance;
    }

    bool ConfigManager::loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            setDefaults();
            saveToFile(filename);
            return false;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        if (!parseJSON(buffer.str())) {
            setDefaults();
            return false;
        }

        return true;
    }

    bool ConfigManager::saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }

        file << generateJSON();
        file.close();

        return true;
    }

    void ConfigManager::setDefaults() {
        config_ = GameConfig{}; // Use default values from struct
    }

    // Simple JSON parser (for basic needs - you could use a library like nlohmann/json for more complex cases)
    bool ConfigManager::parseJSON(const std::string& jsonContent) {
        // This is a simplified parser. For production, consider using nlohmann/json or similar
        try {
            setDefaults(); // Start with defaults

            // Parse live cell color
            if (auto pos = jsonContent.find("\"liveCellColor\""); pos != std::string::npos) {
                auto start = jsonContent.find('[', pos);
                auto end = jsonContent.find(']', start);
                if (start != std::string::npos && end != std::string::npos) {
                    std::string colorStr = jsonContent.substr(start + 1, end - start - 1);
                    std::istringstream iss(colorStr);
                    std::string token;
                    int i = 0;
                    while (std::getline(iss, token, ',') && i < 3) {
                        config_.liveCellColor[i] = std::stof(token);
                        i++;
                    }
                }
            }

            // Parse background color
            if (auto pos = jsonContent.find("\"backgroundColor\""); pos != std::string::npos) {
                auto start = jsonContent.find('[', pos);
                auto end = jsonContent.find(']', start);
                if (start != std::string::npos && end != std::string::npos) {
                    std::string colorStr = jsonContent.substr(start + 1, end - start - 1);
                    std::istringstream iss(colorStr);
                    std::string token;
                    int i = 0;
                    while (std::getline(iss, token, ',') && i < 3) {
                        config_.backgroundColor[i] = std::stof(token);
                        i++;
                    }
                }
            }

            // Parse simple float values
            auto parseFloat = [&](const std::string& key, float& value) {
                if (auto pos = jsonContent.find("\"" + key + "\""); pos != std::string::npos) {
                    auto colonPos = jsonContent.find(':', pos);
                    auto commaPos = jsonContent.find(',', colonPos);
                    auto bracePos = jsonContent.find('}', colonPos);
                    auto endPos = std::min(commaPos, bracePos);

                    if (colonPos != std::string::npos && endPos != std::string::npos) {
                        std::string valueStr = jsonContent.substr(colonPos + 1, endPos - colonPos - 1);
                        // Remove whitespace and quotes
                        valueStr.erase(0, valueStr.find_first_not_of(" \t\n\r\""));
                        valueStr.erase(valueStr.find_last_not_of(" \t\n\r\"") + 1);
                        value = std::stof(valueStr);
                    }
                }
            };

            // Parse simple int values
            auto parseInt = [&](const std::string& key, int& value) {
                if (auto pos = jsonContent.find("\"" + key + "\""); pos != std::string::npos) {
                    auto colonPos = jsonContent.find(':', pos);
                    auto commaPos = jsonContent.find(',', colonPos);
                    auto bracePos = jsonContent.find('}', colonPos);
                    auto endPos = std::min(commaPos, bracePos);

                    if (colonPos != std::string::npos && endPos != std::string::npos) {
                        std::string valueStr = jsonContent.substr(colonPos + 1, endPos - colonPos - 1);
                        valueStr.erase(0, valueStr.find_first_not_of(" \t\n\r\""));
                        valueStr.erase(valueStr.find_last_not_of(" \t\n\r\"") + 1);
                        value = std::stoi(valueStr);
                    }
                }
            };

            // Parse configuration values
            parseFloat("initialZoom", config_.initialZoom);
            parseFloat("playerSpeed", config_.playerSpeed);

            return true;
        } catch (const std::exception& e) {
            return false;
        }
    }

    std::string ConfigManager::generateJSON() const {
        std::ostringstream json;
        json << "{\n";
        json << "  \"visual\": {\n";
        json << "    \"liveCellColor\": [" << config_.liveCellColor.r << ", " << config_.liveCellColor.g << ", " <<
            config_.liveCellColor.b << "],\n";
        json << "    \"backgroundColor\": [" << config_.backgroundColor.r << ", " << config_.backgroundColor.g << ", "
            << config_.backgroundColor.b << "]\n";
        json << "  },\n";
        json << "  \"camera\": {\n";
        json << "    \"initialZoom\": " << config_.initialZoom << "\n";
        json << "  },\n";
        json << "  \"player\": {\n";
        json << "    \"playerSpeed\": " << config_.playerSpeed << "\n";
        json << "  }\n";
        json << "}\n";
        return json.str();
    }
}
