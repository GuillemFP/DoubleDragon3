#ifndef MODULEFADETOBLACK_H
#define MODULEFADETOBLACK_H

#include "Module.h"
#include "SDL/include/SDL_stdinc.h"
#include "Timer.h"

#define MODULE_FADETOBLACK "ModuleFadeToBlack"
#define FADETOBLACK_SECTION "Config.Modules.FadeToBlack"

class ModuleFadeToBlack : public Module
{
public:
	ModuleFadeToBlack();
	~ModuleFadeToBlack();

	bool Init();
	bool Start();
	update_status Update();
	void FadeToBlack(Module* module_on, float time) { FadeToBlack(module_on, nullptr, time); }
	void FadeToBlack(Module* module_on, Module* module_off) { FadeToBlack(module_on, module_off, fDEFAULT_TIME); }
	void FadeToBlack(Module* module_on, Module* module_off, float time);

	void FadeToWhite(Module* module_on, float time) { FadeToWhite(module_on, nullptr, time); }
	void FadeToWhite(Module* module_on, Module* module_off) { FadeToWhite(module_on, module_off, fDEFAULT_TIME); }
	void FadeToWhite(Module* module_on, Module* module_off, float time);
	bool isFading() const { return (fade_time->GetState() == RUNNING); }

	bool ConstantConfig();

private:

	Timer* fade_time = nullptr;

	bool fading_in = true;
	Module* module_on = nullptr;
	Module* module_off = nullptr;

	float fDEFAULT_TIME = 1.0f;

	Uint8 fadecolor_r = 0;
	Uint8 fadecolor_g = 0;
	Uint8 fadecolor_b = 0;
};



#endif // !MODULEFADETOBLACK_H