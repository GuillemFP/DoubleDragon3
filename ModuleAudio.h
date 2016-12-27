#ifndef MODULEAUDIO_H
#define MODULEAUDIO_H

#include <vector>
#include "Module.h"

#define MODULE_AUDIO "ModuleAudio"
#define DEFAULT_MUSIC_FADE_TIME "Config.Modules.Audio.MusicDefaultFadeTime"
#define VOLUME_FX "Config.Modules.Audio.EffectsVolume"
#define VOLUME_MUSIC "Config.Modules.Audio.MusicVolume"

struct _Mix_Music;
struct Mix_Chunk;
typedef struct _Mix_Music Mix_Music;

class ModuleAudio : public Module
{
public:
	ModuleAudio();
	~ModuleAudio();

	bool Init();
	bool CleanUp();

	bool PlayMusic(const char* path);
	bool PlayMusic(const char* path, float fade_time);
	unsigned int LoadFx(const char* path);
	bool PlayFx(unsigned int fx, int repeat = 0);

	bool ConstantConfig();

private:
	Mix_Music* music = nullptr;
	std::vector<Mix_Chunk*> fx;

	float fDEFAULT_FADE = 2.0f;
};



#endif // !MODULEAUDIO_H