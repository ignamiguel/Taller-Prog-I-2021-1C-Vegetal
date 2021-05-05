#ifndef LOGGER_H
#define LOGGER_H

typedef struct {
    char *path;
}logger_t;

void asign_logger_path (logger_t *logger, char * file_path);

void log_information (logger_t* logger, char* text);

void log_error (logger_t* logger, char* text);

void log_debug (logger_t* logger, char* text);

void log_game_start (logger_t* logger);

#endif