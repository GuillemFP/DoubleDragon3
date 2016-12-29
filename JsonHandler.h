#ifndef JSONHANDLER_H
#define JSONHANDLER_H

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
	bool GetAnimation(Animation& anim, const char* name);

	const char* GetString(const char* name);
	int GetInt(const char* name);
	float GetFloat(const char* name);
	bool GetBool(const char* name);

	int GetValueFromArray(int index_array);

private:
	bool ArrayToRect(SDL_Rect& rect, JSON_Array* rect_array);

private:
	JSON_Object* root_object = nullptr;
	JSON_Object* loaded_object = nullptr;
	JSON_Array* loaded_array = nullptr;
	bool bparsing_success = true;
};

#endif // !JSONHANDLER_H