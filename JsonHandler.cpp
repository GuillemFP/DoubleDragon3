#include "JsonHandler.h"
#include "Globals.h"
#include "parson\parson.h"
const char* JSON_GetString(const char* name)
{
	const char* ret = nullptr;
	JSON_Value* root_value = json_parse_file(CONFIGJSON);
	JSON_Object* root_object = json_value_get_object(root_value);

	if (json_object_dotget_value(root_object, name))
	{
		if (json_object_dothas_value_of_type(root_object, name, JSONString))
			ret = json_object_dotget_string(root_object, name);
		else
			LOG("%s: Incorrect value for %s.", CONFIGJSON, name);
	}
	else
		LOG("%s: %s not found.", CONFIGJSON, name);

	return ret;
}

bool JSON_GetString(const char* name, const char** value)
{
	bool ret = true;

	*value = JSON_GetString(name);
	if (*value == nullptr)
		ret = false;

	return ret;
}

int JSON_GetInt(const char* name)
{
	int ret = NULL;
	JSON_Value* root_value = json_parse_file(CONFIGJSON);
	JSON_Object* root_object = json_value_get_object(root_value);
	if (json_object_dotget_value(root_object, name))
	{
		if (json_object_dothas_value_of_type(root_object, name, JSONNumber))
			ret = (int) json_object_dotget_number(root_object, name);
		else
			LOG("%s: Incorrect value for %s.", CONFIGJSON, name);
	}
	else
		LOG("%s: %s not found.", CONFIGJSON, name);

	return ret;
}

bool JSON_GetInt(const char* name, int &value)
{
	bool ret = true;

	value = JSON_GetInt(name);
	if (value == NULL)
		ret = false;

	return ret;
}

bool JSON_GetBool(const char* name)
{
	bool ret = NULL;
	JSON_Value* root_value = json_parse_file(CONFIGJSON);
	JSON_Object* root_object = json_value_get_object(root_value);
	if (json_object_dotget_value(root_object, name))
	{
		if (json_object_dothas_value_of_type(root_object, name, JSONBoolean))
		{
			ret = (json_object_dotget_boolean(root_object, name) != 0);
		}
		else
			LOG("%s: Incorrect value for %s.", CONFIGJSON, name);
	}
	else
		LOG("%s: %s not found.", CONFIGJSON, name);

	return ret;
}