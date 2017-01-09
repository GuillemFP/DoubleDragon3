#ifndef MODULERENDER_H
#define MODULERENDER_H

#include "Module.h"
#include "SDL/include/SDL_rect.h"
#include "Point.h"

#define MODULE_RENDER "ModuleRender"
#define RENDER_SECTION "Config.Modules.Render"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	bool BlitScreenCentered(SDL_Texture* texture, SDL_Rect* section) { return BlitScreenCentered(texture, section, fDEFAULT_SPEED); }
	bool BlitScreenCentered(SDL_Texture* texture, SDL_Rect* section, float speed);

	bool BlitScreenXCentered(SDL_Texture* texture, int y, SDL_Rect* section) { return BlitScreenXCentered(texture, y, section, fDEFAULT_SPEED); }
	bool BlitScreenXCentered(SDL_Texture* texture, int y, SDL_Rect* section, float speed);

	bool Blit(SDL_Texture* texture, const iPoint& position, SDL_Rect* section, bool inverse = false) { return Blit(texture, position, section, fDEFAULT_SPEED, inverse); }
	bool Blit(SDL_Texture* texture, const iPoint& position, SDL_Rect* section, float speed, bool inverse = false);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera = true);

	void CameraInsideScene(int player_x, int x_min, int x_max, int room_y);

	void DebugCamera();

private:
	bool ConstantConfig();

public:
	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera = { 0,0,0,0 };
	bool bCenterCamera = true;

private:
	int iSCREENSIZE = 0;
	int iSCREENWIDTH = 0;
	int iSCREENHEIGHT = 0;
	float fDEFAULT_SPEED = 1.0f;
	bool bVSYNC = true;
	float fCAMERA_MARGIN = 0.5f;
};

#endif // !MODULERENDER_H
