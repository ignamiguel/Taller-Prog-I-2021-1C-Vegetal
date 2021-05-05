
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "logger.h"


int main () {
    char *path = "log.txt";
    logger_t logger;
    asign_logger_path (&logger, path);
    log_game_start (&logger);
    char *text1 = "Inicia programa de log";
    log_information(&logger, text1);
    
    char *text2 = "No hay juego";
    log_error(&logger, text2);
    
    char *text3 = "Aca un movimiento para debuggear";
    log_debug(&logger, text3);
    
    return 0;
}



