#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "logger.h"
#include <chrono>
#include <iostream>

#define ERROR " [ERROR] "
#define INFO " [INFO] "
#define DEBUG " [DEBUG] "


Logger::Logger(string filePath, int logLevel) {
    path = filePath;
    level = logLevel;
}

void Logger::log(string message, string type) {
    char mbstr[20] = {0};
    auto current_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
    strftime(mbstr, sizeof(mbstr), "%Y-%m-%d %X", localtime(&current_time));
    string timestamp = string(mbstr);
    
    ofstream file(path, ofstream::app);
    file << timestamp;
    file << type << message << endl;
    file.close();    
}

void Logger::logError(string text) {
    log(text, ERROR);
}

void Logger::logInformation(string text) {
    if (level > 1) {
        log(text, INFO);
    }
}

void Logger::logDebug(string text) {
    if (level > 2) {
        log(text, DEBUG);
    }
}

void Logger::logNewGame () {
    ofstream file(path, ofstream::app);
    file << endl;
    file << " ---------------- " << endl;
    file << "  DONKEY KONG II  " << endl;
    file << " ---------------- " << endl;
    file.close();
}