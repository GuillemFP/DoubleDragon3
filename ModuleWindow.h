#ifndef MODULEWINDOW_H
#define MODULEWINDOW_H

#include "Module.h"
#include "Point.h"

#define MODULE_WINDOW "ModuleWindow"
#define WINDOW_SECTION "Config.Modules.Window"

struct SDL_Window;
struct SDL_Surface;

class ModuleWindow : public Module
{
public:
	ModuleWindow();
	~ModuleWindow();

	bool Init();
	bool CleanUp();

	int GetScreenWidth() const { return iSCREENWIDTH; }
	int GetScreenHeight() const { return iSCREENHEIGHT; }
	int GetScreenSize() const { return iSCREENSIZE; }

private:
	bool ConstantConfig();

public:
	SDL_Window* window = nullptr;
	SDL_Surface* screen_surface = nullptr;

private:
	const char* strTITLE = "";
	int iSCREENWIDTH = 0;
	int iSCREENHEIGHT = 0;
	int iSCREENSIZE = 0;

	bool bFULLSCREEN = false;
	bool bBORDERLESS = false;
	bool bRESIZABLE = false;
	bool bFULLSCREEN_DESKTOP = false;
};

#endif // !MODULEWINDOW_H
