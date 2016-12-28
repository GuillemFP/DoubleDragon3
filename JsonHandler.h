#ifndef JSONHANDLER_H
#define JSONHANDLER_H

typedef struct json_object_t JSON_Object;
typedef struct json_array_t  JSON_Array;

class JSONParser
{
public:
	JSONParser(const char* file);
	~JSONParser();

	bool LoadObject(const char* name);
	bool UnloadObject();
	bool LoadArrayInObject(const char* name);

	const char* GetString(const char* name);
	int GetInt(const char* name);
	float GetFloat(const char* name);
	bool GetBool(const char* name);

	int GetRectFromArray(int index_array, int rect_coordinate);

	bool GetError() const;

private:
	JSON_Object* root_object = nullptr;
	JSON_Object* loaded_object = nullptr;
	JSON_Array* loaded_array = nullptr;
	bool bparsing_success = true;
};

#endif // !JSONHANDLER_H