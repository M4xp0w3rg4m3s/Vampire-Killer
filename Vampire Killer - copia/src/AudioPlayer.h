#pragma once
#include "Entity.h"
#include <unordered_map>
#include "raylib.h"

class AudioPlayer : public Entity
{
public:
    static AudioPlayer& Instance()
    {
        static AudioPlayer instance; //Guaranteed to be initialized only once
        return instance;
    }
    void Update() override;
    void PlaySoundByName(const char* soundName);
    void PlayMusicByName(const char* musicName);
    void StopMusicByName(const char* musicName);
    void CreateSound(const char* path, const char* soundName);
    void CreateMusic(const char* path, const char* musicName);
    void DeleteSound(const char* soundName);
    void DeleteMusic(const char* musicName);
    void SetMusicLoopStatus(const char* musicName, bool isLoop);
    bool IsMusicPlaying(const char* musicName);
private:

    //Private constructor to prevent instantiation
    AudioPlayer();
    //Destructor
    ~AudioPlayer();

    bool IsSoundLoaded(const char* soundName);
    bool IsMusicLoaded(const char* musicName);
    std::unordered_map<const char*, Sound> soundsData;
    std::unordered_map<const char*, Music> musicsData;



};