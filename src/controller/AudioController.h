#pragma once
#include "../utils/marioStructs.h"
#include <SDL2/SDL_mixer.h>

class AudioController {
public:
    static void loadAudioFiles();
    static void closeAudioFiles();

    static void toggleMusic();
    static void checkToggleMusicEvent();
    static void playSounds(sounds_t sounds);
private:
    static Mix_Music* music;
    static Mix_Chunk* jumpSound;
    static Mix_Chunk* deathSound;
    static Mix_Chunk* finishedLevelSound;
    static Mix_Chunk* itemSound;
    static Mix_Chunk* enemyDeathSound; 
    static bool isKeyDown;
};