#ifndef MODULEFADETOBLACK_H
#define MODULEFADETOBLACK_H

#include "Module.h"
#include "SDL/include/SDL_stdinc.h"

#define MODULE_FADETOBLACK "ModuleFadeToBlack"
#define DEFAULT_TIME "Config.Modules.FadeToBlack.DefaultTime"

class Timer;

class ModuleFadeToBlack : public Module
{
public:
	ModuleFadeToBlack();
	~ModuleFadeToBlack();

	bool Init();
	bool Start();
	update_status Update();
	void FadeToBlack(Module* module_on, float time);
	void FadeToBlack(Module* module_on, Module* module_off);
	void FadeToBlack(Module* module_on, Module* module_off, float time);
	bool isFading() const;
	bool isFadingIn() const;

	bool ConstantConfig();

private:

	Timer* fade_time = nullptr;

	bool fading_in = true;
	Module* module_on = nullptr;
	Module* module_off = nullptr;

	float fDEFAULT_TIME = 1.0f;
};



#endif // !MODULEFADETOBLACK_H