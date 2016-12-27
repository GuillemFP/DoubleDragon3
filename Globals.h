#ifndef GLOBALS_H
#define GLOBALS_H

#include "MemLeaks.h"

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Deletes a buffer
#define RELEASE( x ) \
    {									  \
       if( x != nullptr )   \
       {						      \
         delete x;                  \
	     x = nullptr;             \
       }                      \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{                              \
       if( x != nullptr )              \
       {                            \
           delete[] x;                \
	       x = nullptr;                    \
		 }                            \
                              \
	 }

// Configuration from JSON file -----------
#define CONFIGJSON "config.json"

#define TITLE "Config.App.Title"
#define SCREEN_WIDTH "Config.App.Resolution.Width"
#define SCREEN_HEIGHT "Config.App.Resolution.Height"
#define SCREEN_SIZE "Config.App.Resolution.Scale"

const char* JSON_GetString(const char* name);
bool JSON_GetString(const char* name, const char** value);
int JSON_GetInt(const char* name);
bool JSON_GetInt(const char* name, int &value);
float JSON_GetFloat(const char* name);
bool JSON_GetFloat(const char* name, float &value);
bool JSON_GetBool(const char* name);

#endif // !GLOBALS_H
