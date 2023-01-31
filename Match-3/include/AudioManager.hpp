#ifndef __Match3_Audio_Manager_h__
#define __Match3_Audio_Manager_h__

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "ECS/Manager.hpp"


extern ECS_Manager ecsManager;

class AudioManager
{
public:
    void Init()
    {
        gameMusic.push_back(Mix_LoadMUS( "../res/Audio/Bumper Tag - John Deley.mp3"));
        gameMusic.push_back(Mix_LoadMUS( "../res/Audio/Covert Affair - Film Noire - Kevin MacLeod.mp3"));
        gameMusic.push_back(Mix_LoadMUS( "../res/Audio/Piano Store - Jimmy Fontanez_Media Right Productions.mp3"));

        clearNormal = Mix_LoadWAV( "../res/Audio/Sounds/Normal.wav" );
        clearRowCol = Mix_LoadWAV( "../res/Audio/Sounds/Row Col.wav" );
        clearExplosion = Mix_LoadWAV( "../res/Audio/Sounds/Explosion.wav" );
        clearBigExplosion = Mix_LoadWAV( "../res/Audio/Sounds/Big Explosion.wav" );
        clearConsumption = Mix_LoadWAV( "../res/Audio/Sounds/Comsumption.wav" );
    }

    void PlayRandomSoundtrack()
    {
        int musicIndex = rand() % gameMusic.size();
        Mix_PlayMusic( gameMusic[musicIndex], -1 );
    }
private:
    std::vector<Mix_Music*> gameMusic;
    Mix_Chunk* clearNormal;
    Mix_Chunk* clearRowCol;
    Mix_Chunk* clearExplosion;
    Mix_Chunk* clearBigExplosion;
    Mix_Chunk* clearConsumption;
};

#endif //__Match3_Audio_Manager_h__