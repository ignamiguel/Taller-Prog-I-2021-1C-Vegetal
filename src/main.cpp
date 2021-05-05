#include <stdio.h>
#include <time.h>
#include <string>
#include "logger.h"


void asign_logger_path (logger_t *logger, char * file_path){
    logger->path = file_path;
}

int log (char *path, char* text) {
    FILE * logFile;
    logFile = fopen (path, "a");
    if (logFile == NULL) {
        fprintf(stderr, "Error al abrir el archivo de log");
        return 1;
    }
    fprintf (logFile, text);
    fclose(logFile);
    return 0;
}

tm* timestamp() {
    time_t rawtime;
    struct tm * timeinfo;
    

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    return timeinfo;
}

void start_new_line (logger_t* logger) {
    tm *time = timestamp(); 
    char buffer[] = "YYYY-MM-dd HH:mm:ss";
    strftime (buffer,80,"%F %T",time);
    
    log (logger->path, "\n");
    log(logger->path, buffer);
}

void log_information (logger_t* logger, char* text) {
    start_new_line (logger);
    log (logger->path, " [INFO ] ");
    log (logger->path, text);
}

void log_error (logger_t* logger, char* text) {
    start_new_line (logger);
    log (logger->path, " [ERROR] ");
    log (logger->path, text);
}

void log_debug (logger_t* logger, char* text) {
    start_new_line (logger);
    log (logger->path, " [DEBUG] ");
    log (logger->path, text);
}

void log_game_start (logger_t* logger) {
    log (logger->path, "\n ---------------- ");
    log (logger->path, "\n  DONKEY KONG II  ");
    log (logger->path, "\n ---------------- ");
}