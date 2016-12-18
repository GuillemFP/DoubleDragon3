#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#define TITLE "Config.App.Title"
#define SCREEN_WIDTH "Config.App.Resolution.Width"
#define SCREEN_HEIGHT "Config.App.Resolution.Height"
#define SCREEN_SIZE "Config.App.Resolution.Scale"

#define MODULE_INPUT "Config.Modules.Input.Name"
#define NUM_MOUSE_BUTTONS "Config.Modules.Input.MouseButtons"
#define MAX_KEYS "Config.Modules.Input.KeyboardKeys"

#define MODULE_WINDOW "Config.Modules.Window.Name"
#define FULLSCREEN "Config.Modules.Window.Fullscreen"
#define BORDERLESS "Config.Modules.Window.Borderless"
#define RESIZABLE "Config.Modules.Window.Resizable"
#define FULLSCREEN_DESKTOP "Config.Modules.Window.Fullscreen_Window"

#define MODULE_RENDER "Config.Modules.Render.Name"
#define VSYNC "Config.Modules.Render.Vsync"

const char* JSON_GetString(const char* name);
bool JSON_GetString(const char* name, const char** value);
int JSON_GetInt(const char* name);
bool JSON_GetInt(const char* name, int &value);
bool JSON_GetBool(const char* name);

#endif // !JSONHANDLER_H
