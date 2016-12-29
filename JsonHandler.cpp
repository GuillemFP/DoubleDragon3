#include "JsonHandler.h"
#include "Globals.h"
#include "parson\parson.h"


JSONParser::JSONParser(const char* file)
{
	root_object = json_value_get_object(json_parse_file(file));
}

JSONParser::~JSONParser()
{
}

bool JSONParser::LoadObject(const char* name)
{
	bparsing_success = false;

	if (loaded_object == nullptr)
	{
		if (json_object_dothas_value(root_object, name))
		{
			loaded_object = json_object_dotget_object(root_object, name);
			bparsing_success = true;
		}
		else
			LOG("JSONParser: Unable to load section %s.", name);
	}
	else
		LOG("JSONParser: Already one section loaded. Section %s not loaded.", name);
	
	return bparsing_success;
}

bool JSONParser::UnloadObject()
{
	bool ret = bparsing_success;

	if (bparsing_success == false)
		LOG("JSONParser: Parsing ended with some errors.");

	bparsing_success = true;
	loaded_object = nullptr;
	loaded_array = nullptr;
	return ret;
}

bool JSONParser::LoadArrayInObject(const char * name)
{
	bool ret = false;

	if (loaded_object != nullptr)
	{
		if (json_object_dotget_value(loaded_object, name))
		{
			if (json_object_dothas_value_of_type(loaded_object, name, JSONArray))
			{
				loaded_array = json_object_get_array(loaded_object, name);
				ret = true;
			}
			else
				LOG("JSONParser: Incorrect array value for %s.", name);
		}
		else
			LOG("JSONParser: Array %s not found.", name);
	}
	else
		LOG("JSONParser: No section loaded. Array %s cannot load.", name);

	return ret;
}

const char* JSONParser::GetString(const char* name)
{
	const char* ret = nullptr;

	if (loaded_object != nullptr)
	{
		if (json_object_dotget_value(loaded_object, name))
		{
			if (json_object_dothas_value_of_type(loaded_object, name, JSONString))
			{
				ret = json_object_dotget_string(loaded_object, name);
			}
			else
			{
				LOG("JSONParser: Incorrect string value for %s.", name);
				bparsing_success = false;
			}
		}
		else
		{
			LOG("JSONParser: %s not found.", name);
			bparsing_success = true;
		}
	}
	else
	{
		LOG("JSONParser: No section loaded.");
		bparsing_success = true;
	}

	return ret;
}

int JSONParser::GetInt(const char * name)
{
	int ret = NULL;

	if (loaded_object != nullptr)
	{
		if (json_object_dotget_value(loaded_object, name))
		{
			if (json_object_dothas_value_of_type(loaded_object, name, JSONNumber))
			{
				ret = (int) json_object_dotget_number(loaded_object, name);
			}
			else
			{
				LOG("JSONParser: Incorrect integer value for %s.", name);
				bparsing_success = false;
			}
		}
		else
		{
			LOG("JSONParser: %s not found.", name);
			bparsing_success = false;
		}
	}
	else
	{
		LOG("JSONParser: No section loaded.");
		bparsing_success = false;
	}

	return ret;
}

float JSONParser::GetFloat(const char * name)
{
	float ret = NULL;

	if (loaded_object != nullptr)
	{
		if (json_object_dotget_value(loaded_object, name))
		{
			if (json_object_dothas_value_of_type(loaded_object, name, JSONNumber))
			{
				ret = (float)json_object_dotget_number(loaded_object, name);
			}
			else
			{
				LOG("JSONParser: Incorrect float value for %s.", name);
				bparsing_success = false;
			}
		}
		else
		{
			LOG("JSONParser: %s not found.", name);
			bparsing_success = false;
		}
	}
	else
	{
		LOG("JSONParser: No section loaded.");
		bparsing_success = false;
	}

	return ret;
}

bool JSONParser::GetBool(const char * name)
{
	bool ret = NULL;

	if (loaded_object != nullptr)
	{
		if (json_object_dotget_value(loaded_object, name))
		{
			if (json_object_dothas_value_of_type(loaded_object, name, JSONBoolean))
			{
				ret = json_object_dotget_boolean(loaded_object, name);
			}
			else
			{
				LOG("JSONParser: Incorrect boolean value for %s.", name);
				bparsing_success = false;
			}
		}
		else
		{
			LOG("JSONParser: %s not found.", name);
			bparsing_success = false;
		}
	}
	else
	{
		LOG("JSONParser: No section loaded.");
		bparsing_success = false;
	}

	return ret;
}

int JSONParser::GetArrayValueFromArray(int index_array, int rect_coordinate)
{
	int ret = NULL;
	
	if (loaded_array != nullptr)
	{
		if (index_array < json_array_get_count(loaded_array))
		{
			JSON_Array* rect = json_array_get_array(loaded_array, index_array);
			ret = json_array_get_number(rect, rect_coordinate);
		}
		else
		{
			LOG("JSONParser: Error loading array in loaded array. Index %i out of range.", index_array);
			bparsing_success = false;
		}
	}
	else
	{
		LOG("JSONParser: No array loaded to extract value.");
		bparsing_success = false;
	}

	return ret;
	// TODO: insert return statement here
}

int JSONParser::GetValueFromArray(int index_array)
{
	int ret = NULL;

	if (loaded_array != nullptr)
	{
		if (index_array < json_array_get_count(loaded_array))
		{
			ret = json_array_get_number(loaded_array, index_array);
		}
		else
		{
			LOG("JSONParser: Error loading array in loaded array. Index %i out of range.", index_array);
			bparsing_success = false;
		}
	}
	else
	{
		LOG("JSONParser: No array loaded to extract value.");
		bparsing_success = false;
	}

	return ret;
}

bool JSONParser::GetError() const
{
	return !bparsing_success;
}