#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include "Point.h"
#include "Globals.h"

typedef struct json_object_t JSON_Object;
typedef struct json_array_t  JSON_Array;

struct SDL_Rect;
class Animation;

class JSONParser
{
public:
	JSONParser(const char* file);
	~JSONParser();

	bool LoadObject(const char* name);
	bool UnloadObject();
	bool LoadArrayInObject(const char* name);

	bool GetRect(SDL_Rect& rect, const char* name);
	bool GetPoint(iPoint& point, const char* name);
	bool GetPoint3D(Point3d& point, const char* name);
	bool GetPoint(fPoint& point, const char* name);
	bool GetAnimation(Animation& anim, const char* name);
	bool GetIntArray(const char* name, int* int_array);

	const char* GetString(const char* name);
	int GetInt(const char* name);
	float GetFloat(const char* name);
	bool GetBool(const char* name);
	bool GetBoolMandatory(const char* name);

	int GetIntFromArray(size_t index_array);
	const char* GetStringFromArray(size_t index_array);
	bool GetRectFromArray(SDL_Rect& rect, size_t index_array);
	bool GetPointFromArray(iPoint& point, size_t index_array);
	int GetIntFromArrayInArray(size_t array_element, size_t index_array);
	const char* GetStringFromArrayInArray(size_t array_element, size_t index_array);

private:
	bool ArrayToRect(SDL_Rect& rect, JSON_Array* rect_array);
	bool ArrayToPoint(iPoint& point, JSON_Array* point_array);
	bool ArrayToPoint(fPoint& point, JSON_Array* point_array);
	bool ArrayToPoint3d(Point3d& point, JSON_Array* point_array);

private:
	JSON_Object* root_object = nullptr;
	JSON_Object* loaded_object = nullptr;
	JSON_Array* loaded_array = nullptr;
	bool bparsing_success = true;
};

#endif // !JSONHANDLER_H