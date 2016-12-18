#ifndef MODULEWINDOW_H
#define MODULEWINDOW_H

#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

class ModuleWindow : public Module
{
public:
	ModuleWindow();
	~ModuleWindow();

	bool Init();
	bool CleanUp();

	int GetScreenWidth() const;
	int GetScreenHeight() const;
	int GetScreenSize() const;

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