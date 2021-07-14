#include "logger.h"
#include <fstream>
#include <chrono>
#include <iostream>

namespace logger {
    constexpr auto debugLogLevel = "DEBUG";
    constexpr auto infoLogLevel = "INFO";
    constexpr auto errorLogLevel = "ERROR";

    constexpr auto defaultLogLevel = debugLogLevel;

    Logger& Logger::getInstance() {
        static Logger instance;
        return instance;
    }

    Logger::Logger() {
        debug_level = defaultLogLevel;
        filepath = "game.log";
    }

    void Logger::setLogLevel(const std::string& debug_level) {
        this->debug_level = debug_level;
    }

    void Logger::log(const std::string& message, const std::string& type) const {
        char mbstr[20] = {0};
        auto current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        strftime(mbstr, sizeof(mbstr), "%Y-%m-%d %X", std::localtime(&current_time));
        std::string timestamp = std::string(mbstr);
        
        std::ofstream file(filepath, std::ofstream::app);
        file << timestamp;
        file << " [" << type << "] " << message << std::endl;
        file.close();    
    }

    void Logger::logError(const std::string& text) const {
        log(text, errorLogLevel);
    }

    void Logger::logInformation(const std::string& text) const {
        if (debug_level == debugLogLevel || debug_level == infoLogLevel) {
            log(text, infoLogLevel);
        }
    }

    void Logger::logDebug(const std::string& text) const {
        if (debug_level == debugLogLevel) {
            log(text, debugLogLevel);
        }
    }

    void Logger::logNewGame(void) const {
    std::ofstream file(filepath, std::ofstream::app);
    file << '\n';
    file << "-------------------------\n";
    file << " STARTING DONKEY KONG II\n";
    file << "-------------------------" << std::endl;
    file.close();
    }
}