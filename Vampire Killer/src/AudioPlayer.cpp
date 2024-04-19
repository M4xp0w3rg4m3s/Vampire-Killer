
#include "AudioPlayer.h"

AudioPlayer::AudioPlayer() {

}
void AudioPlayer::Update()
{
	for (auto& music : musicsData)
	{
		if (IsMusicStreamPlaying(music.second)) {
			UpdateMusicStream(music.second);
		}

	}
}

void AudioPlayer::PlaySoundByName(const char* soundName)
{
	if (!IsSoundLoaded(soundName))
	{
		printf("Sound File Not Loaded");
		return;
	}

	PlaySound(soundsData[soundName]);
}

void AudioPlayer::PlayMusicByName(const char* musicName)
{
	if (!IsMusicLoaded(musicName))
	{
		printf("Sound File Not Loaded");
		return;
	}
	PlayMusicStream(musicsData[musicName]);
}

void AudioPlayer::StopMusicByName(const char* musicName)
{
	if (!IsMusicLoaded(musicName))
	{
		printf("Music File Not Loaded");
		return;
	}
	StopMusicStream(musicsData[musicName]);

}

void AudioPlayer::DeleteSound(const char* soundName)
{
	if (!IsSoundLoaded(soundName))
	{
		printf("Sound File Not Loaded");
		return;
	}
	UnloadSound(soundsData[soundName]);
	soundsData.erase(soundName);
}

void AudioPlayer::DeleteMusic(const char* musicName)
{
	if (!IsMusicLoaded(musicName))
	{
		printf("Music File Not Loaded");
		return;
	}
	UnloadMusicStream(musicsData[musicName]);
	musicsData.erase(musicName);
}

void AudioPlayer::SetMusicLoopStatus(const char* musicName, bool isLoop)
{
	if (!IsMusicLoaded(musicName))
	{
		printf("Music File Not Loaded");
		return;
	}
	musicsData[musicName].looping = isLoop;
}

void AudioPlayer::CreateSound(const char* path, const char* soundName)
{
	if (IsSoundLoaded(soundName))
	{
		printf("Sound File Already Loaded");
		return;
	}


	soundsData[soundName] = LoadSound(path);
}

void AudioPlayer::CreateMusic(const char* path, const char* musicName)
{
	if (IsMusicLoaded(musicName))
	{
		printf("Music File Already Loaded");
		return;
	}


	musicsData[musicName] = LoadMusicStream(path);
}


bool AudioPlayer::IsSoundLoaded(const char* soundName)
{
	return soundsData.find(soundName) != soundsData.end();
}

bool AudioPlayer::IsMusicLoaded(const char* musicName)
{
	return musicsData.find(musicName) != musicsData.end();
}

AudioPlayer::~AudioPlayer()
{

	for (; soundsData.size() != 0;)
	{
		DeleteSound(soundsData.begin()->first);
	}
	soundsData.clear();
	for (; musicsData.size() != 0;)
	{
		DeleteMusic(musicsData.begin()->first);
	}
	musicsData.clear();
}
