#pragma once

#include <string>
#include <vector>
#include <jsoncpp/json/json.h>

namespace configuration
{
    class Enemy
    {
        public:
            Enemy() = default;
            Enemy(const std::string& t, const unsigned int q) : type(t), quantity(q) {};
            ~Enemy() = default;;

            const std::string getType() const { return type; };
            unsigned int getQuantity() const { return quantity; };

        private:
            std::string type;
            unsigned int quantity;
    };
    
    class Stage
    {
        public:
            Stage() = default;
            Stage(const std::vector<std::string> bgs) : backgrounds(bgs) {};

            const std::vector<std::string> getBackgrounds() const { return backgrounds; };

        private:
            std::vector<std::string> backgrounds;
    };

    class GameConfiguration
    {
        public:
            GameConfiguration(const std::string& json_filename);
            ~GameConfiguration() = default;;

            const std::string getLogLevel() const { return logLevel; };
            const std::vector<configuration::Enemy> getEnemies() const { return enemies; };
            const std::vector<configuration::Stage> getStages() const { return stages; };
            int getMaxPlayers() const { return maxPlayers; };
            bool getDefaultConfigFlag();

        private:
            std::string logLevel;
            std::vector<Enemy> enemies;
            std::vector<Stage> stages;
            int maxPlayers;
            bool useDefaultConfig;

            static const Json::Value getJsonValue(const Json::Value& root, const std::string& name);
            bool loadFromFile(std::string configFileName);
    };
}
