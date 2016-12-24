#include "Globals.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL.h"

ModuleWindow::ModuleWindow() : Module(JSON_GetString(MODULE_WINDOW))
{
}

 ModuleWindow::~ModuleWindow()
{
}

 bool ModuleWindow::Init()
 {
	 LOG("Init SDL window & surface");
	 bool ret = ConstantConfig();

	 if (SDL_Init(SDL_INIT_VIDEO) < 0)
	 {
		 LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		 ret = false;
	 }
	 else
	 {
		 int width = iSCREENWIDTH * iSCREENSIZE;
		 int height = iSCREENHEIGHT * iSCREENSIZE;
		 Uint32 flags = SDL_WINDOW_SHOWN;

		 if (bFULLSCREEN)
			 flags |= SDL_WINDOW_FULLSCREEN;
		 
		 if (bRESIZABLE)
			 flags |= SDL_WINDOW_RESIZABLE;

		 if (bBORDERLESS)
			 flags |= SDL_WINDOW_BORDERLESS;

		 if (bFULLSCREEN_DESKTOP)
			 flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		 window = SDL_CreateWindow(strTITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		 if (window == nullptr)
		 {
			 LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			 ret = false;
		 }
		 else
		 {
			 screen_surface = SDL_GetWindowSurface(window);
		 }
	 }

	 return ret;
 }

 bool ModuleWindow::CleanUp()
 {
	 LOG("Destroying SDL window and quitting all SDL systems");

	 //Destroy window
	 if (window != nullptr)
	 {
		 SDL_DestroyWindow(window);
	 }

	 //Quit SDL subsystems
	 SDL_Quit();
	 return true;
 }

 bool ModuleWindow::ConstantConfig()
 {
	 bool ret = true;

	 ret = ret && JSON_GetString(TITLE, &strTITLE);
	 ret = ret && JSON_GetInt(SCREEN_WIDTH, iSCREENWIDTH);
	 ret = ret && JSON_GetInt(SCREEN_HEIGHT, iSCREENHEIGHT);
	 ret = ret && JSON_GetInt(SCREEN_SIZE, iSCREENSIZE);

	 bFULLSCREEN = JSON_GetBool(FULLSCREEN);
	 bBORDERLESS = JSON_GetBool(BORDERLESS);
	 bRESIZABLE = JSON_GetBool(RESIZABLE);
	 bFULLSCREEN_DESKTOP = JSON_GetBool(FULLSCREEN_DESKTOP);

	 return ret;
 }

 int ModuleWindow::GetScreenWidth() const
 {
	 return iSCREENWIDTH;
 }

 int ModuleWindow::GetScreenHeight() const
 {
	 return iSCREENHEIGHT;
 }

 int ModuleWindow::GetScreenSize() const
 {
	 return iSCREENSIZE;
 }