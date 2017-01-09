#ifndef MODULEAUDIO_H
#define MODULEAUDIO_H

#include <vector>
#include "Module.h"

#define MODULE_AUDIO "ModuleAudio"
#define AUDIO_SECTION "Config.Modules.Audio"

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

	bool PlayMusic(const char* path) { return PlayMusic(path, fDEFAULT_FADE); }
	bool PlayMusic(const char* path, float fade_time);
	void StopMusic();
	unsigned int LoadFx(const char* path);
	bool PlayFx(unsigned int fx, int repeat = 0);

	bool isPlayingMusic() const;

	bool ConstantConfig();

private:
	Mix_Music* music = nullptr;
	std::vector<Mix_Chunk*> fx;

	float fDEFAULT_FADE = 2.0f;
	int iVOLUME_MUSIC = 1;
	int iVOLUME_FX = 1;
};



#endif // !MODULEAUDIO_H