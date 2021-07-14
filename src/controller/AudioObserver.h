#pragma once
#include "../utils/marioStructs.h"

class AudioObserver {
public:
    void update(SoundCode soundCode);
    void reset();
    sounds_t getState() const;
private:
    sounds_t sounds{0, 0, 0, 0, 0}; 
};