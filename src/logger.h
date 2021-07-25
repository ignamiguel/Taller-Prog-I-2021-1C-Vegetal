#pragma once

#include <string>

namespace logger
{
    class Logger
    {
    private:
        Logger();
        ~Logger() = default;
        Logger(const Logger &) = delete;
        Logger &operator=(const Logger &) = delete;
        std::string filepath;
        std::string debug_level;
        void log(const std::string &, const std::string &) const;

    public:
        static Logger &getInstance();
        void setLogLevel(const std::string &);
        void logNewGame(void) const;
        void logGameOver(void) const;
        void logError(const std::string &) const;
        void logInformation(const std::string &) const;
        void logDebug(const std::string &) const;
    };
}