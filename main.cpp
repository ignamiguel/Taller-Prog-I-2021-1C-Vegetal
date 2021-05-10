
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "logger.h"


int main () {
    Logger logger("log.txt", 1);
    logger.logNewGame();
    logger.logError("log error con nivel 1");
    logger.logInformation("log info con nivel 1");
    logger.logDebug("log debug con nivel 1");
    
    logger = Logger("log.txt", 2);
    logger.logNewGame();
    logger.logError("log error con nivel 2");
    logger.logInformation("log info con nivel 2");
    logger.logDebug("log debug con nivel 2");
    
    logger = Logger("log.txt", 3);
    logger.logNewGame();
    logger.logError("log error con nivel 3");
    logger.logInformation("log info con nivel 3");
    logger.logDebug("log debug con nivel 3");
    
    return 0;
}