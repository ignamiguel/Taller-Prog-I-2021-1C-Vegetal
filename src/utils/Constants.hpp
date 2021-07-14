#pragma once

#include <string>

#define MIN_PLAYERS 1
#define MAX_PLAYERS 4

//estadoNivel
#define MAX_PLATFORMS 12
#define MAX_ENEMIES 3
#define MAX_BARRELS 10
#define DEFAULT_MAX_PLAYERS 2 //en caso de que la cant max de jugadores sea negativa en el json


// Dimensiones
#define ANCHO_NIVEL 224
#define ALTO_NIVEL 256

#define ANCHO_TILE 16
#define ALTO_TILE 16

#define ANCHO_BARRIL 12
#define ALTO_BARRIL 10

#define ANCHO_ENEMIGO_FUEGO 15
#define ALTO_ENEMIGO_FUEGO 16

#define ANCHO_FUEGO 16
#define ALTO_FUEGO 16

#define ANCHO_PLATAFORMA 16
#define ALTO_PLATAFORMA 8

#define ANCHO_POLEA 11
#define ALTO_POLEA 10

#define ANCHO_MARIO 16
#define ALTO_MARIO 16

#define DONKEYKONG_ANCHO 40
#define DONKEYKONG_ALTO 34

#define PAULINE_POS_X 88
#define PAULINE_POS_Y 34

#define ANCHO_DEFAULT_CONFIG 108
#define ALTO_DEFAULT_CONFIG 7

// Nivel 1
#define N1_POS_X_FUEGO1 48
#define N1_POS_X_FUEGO2 88
#define N1_POS_X_FUEGO3 128
#define N1_POS_X_FUEGO4 168

#define N1_POS_Y_FUEGO 240

#define N1_POS_X1_POLEA 10
#define N1_POS_X2_POLEA 203

#define N1_POS_Y1_POLEA 128
#define N1_POS_Y2_POLEA 168

// Nivel 2
#define N2_POS_X_FUEGO 16
#define N2_POS_Y_FUEGO 216

#define N2_POS_Y_BARRIL 30

// Common
#define MARIO_START_X 0
#define MARIO_START_Y 232

#define DONKEYKONG_POS_X 24
#define DONKEYKONG_POS_Y 50

#define PAULINE_ANCHO 16
#define PAULINE_ALTO 22

#define DEFAULT_CONFIG_X 56
#define DEFAULT_CONFIG_Y 20

// Login
#define LOGIN_OK 0
#define LOGIN_ABORTED 1
#define LOGIN_INVALID_USER_PASS 2
#define LOGIN_USER_ALREADY_CONNECTED 3
#define LOGIN_MAX_USERS_CONNECTED 4
#define LOGIN_INVALID_USER 5

// Name
const std::string NOMBRE_JUEGO = "Donkey Kong 2 Jumpman Returns";
// Config
const std::string CONFIG_FILE = "archivo.json";
// Default
const std::string IMG_DEFAULT = "res/default.png";