#include "JsonHandler.h"
#include "Globals.h"
#include "parson\parson.h"
#include "SDL/include/SDL_rect.h"
#include "Animation.h"



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

bool JSONParser::GetRect(SDL_Rect& rect, const char* name)
{
	bool ret = false;

	JSON_Array* loaded_array;

	if (loaded_object != nullptr)
	{
		if (json_object_dothas_value_of_type(loaded_object, name, JSONArray))
		{
			loaded_array = json_object_get_array(loaded_object, name);
			ret = ArrayToRect(rect, loaded_array);
			if (ret == false)
			{
				LOG("JSONParser: Rectangle array %s has incorrect number of elements.", name);
				bparsing_success = false;
			}
		}
		else
		{
			LOG("JSONParser: Rectangle name %s not found.", name);
			bparsing_success = false;
		}
	}
	else
	{
		LOG("JSONParser: No section loaded. Rectangle %s cannot load.", name);
		bparsing_success = false;
	}

	return ret;
}

bool JSONParser::GetPoint(iPoint& point, const char * name)
{
	bool ret = false;

	JSON_Array* loaded_array;

	if (loaded_object != nullptr)
	{
		if (json_object_dothas_value_of_type(loaded_object, name, JSONArray))
		{
			loaded_array = json_object_get_array(loaded_object, name);
			ret = ArrayToPoint(point, loaded_array);
			if (ret == false)
			{
				LOG("JSONParser: Point array %s has incorrect number of elements.", name);
				bparsing_success = false;
			}
		}
		else
		{
			LOG("JSONParser: Point name %s not found.", name);
			bparsing_success = false;
		}
	}
	else
	{
		LOG("JSONParser: No section loaded. Point %s cannot load.", name);
		bparsing_success = false;
	}

	return ret;
}

bool JSONParser::GetPoint3D(Point3d & point, const char * name)
{
	bool ret = false;

	JSON_Array* loaded_array;

	if (loaded_object != nullptr)
	{
		if (json_object_dothas_value_of_type(loaded_object, name, JSONArray))
		{
			loaded_array = json_object_get_array(loaded_object, name);
			ret = ArrayToPoint3d(point, loaded_array);
			if (ret == false)
			{
				LOG("JSONParser: Point array %s has incorrect number of elements.", name);
				bparsing_success = false;
			}
		}
		else
		{
			LOG("JSONParser: Point name %s not found.", name);
			bparsing_success = false;
		}
	}
	else
	{
		LOG("JSONParser: No section loaded. Point %s cannot load.", name);
		bparsing_success = false;
	}

	return ret;
}

bool JSONParser::GetPoint(fPoint& point, const char * name)
{
	bool ret = false;

	JSON_Array* loaded_array;

	if (loaded_object != nullptr)
	{
		if (json_object_dothas_value_of_type(loaded_object, name, JSONArray))
		{
			loaded_array = json_object_get_array(loaded_object, name);
			ret = ArrayToPoint(point, loaded_array);
			if (ret == false)
			{
				LOG("JSONParser: Point array %s has incorrect number of elements.", name);
				bparsing_success = false;
			}
		}
		else
		{
			LOG("JSONParser: Point name %s not found.", name);
			bparsing_success = false;
		}
	}
	else
	{
		LOG("JSONParser: No section loaded. Point %s cannot load.", name);
		bparsing_success = false;
	}

	return ret;
}

bool JSONParser::GetAnimation(Animation& anim, const char* name)
{
	bool ret = true;
	int iframes = NULL;
	JSON_Object* animation;
	JSON_Array* sprites;
	JSON_Array* rect_array;
	SDL_Rect rect;

	if (loaded_object != nullptr)
	{
		animation = json_object_get_object(loaded_object, name);
		if (animation != nullptr)
		{
			if (json_object_has_value_of_type(animation, ANIMATION_SPEED, JSONNumber))
				anim.speed = (float)json_object_get_number(animation, ANIMATION_SPEED);
			if (json_object_has_value_of_type(animation, ANIMATION_LOOP, JSONBoolean))
				anim.bloop = (json_object_get_boolean(animation, ANIMATION_LOOP) > 0 ? true : false);
			else
				anim.bloop = true;
			if (json_object_has_value_of_type(animation, ANIMATION_FRAMES, JSONNumber))
				iframes = (int)json_object_get_number(animation, ANIMATION_FRAMES);
			if (json_object_has_value_of_type(animation, ANIMATION_SPRITES, JSONArray))
			{
				sprites = json_object_get_array(animation, ANIMATION_SPRITES);

				if (json_array_get_count(sprites) == iframes)
				{
					for (int i = 0; i < iframes; i++)
					{
						rect_array = json_array_get_array(sprites, i);
						ret = ret && ArrayToRect(rect, rect_array);
						anim.frames.push_back(rect);
					}
					if (ret == false)
					{
						LOG("JSONParser: Error retrieving one or more rectangle array from %s.", name);
						ret = false;
						bparsing_success = false;
					}
				}
				else
				{
					LOG("JSONParser: Sprites array %s has incorrect number of elements.", name);
					bparsing_success = false;
					ret = false;
				}
			}
		}
		else
		{
			LOG("JSONParser: Animation name %s not found.", name);
			bparsing_success = false;
			ret = false;
		}
	}
	else
	{
		LOG("JSONParser: No section loaded. Animation %s cannot load.", name);
		bparsing_success = false;
		ret = false;
	}

	return ret;
}

bool JSONParser::GetIntArray(const char* name, int* int_array)
{
	bool ret = true;
	JSON_Array* box_array;
	int count = 0;

	int x0, x1, y0, y1;

	ret = LoadArrayInObject(name);

	for (size_t i = 0; i < json_array_get_count(loaded_array) - 1;)
	{
		box_array = json_array_get_array(loaded_array, i);
		x0 = (int) json_array_get_number(box_array, 0);
		y0 = (int) json_array_get_number(box_array, 1);
		box_array = json_array_get_array(loaded_array, i + 1);
		x1 = (int) json_array_get_number(box_array, 0);
		y1 = (int) json_array_get_number(box_array, 1);
		if (count >= x0 && count <= x1)
		{
			float slope = 1.0f;
			if (x1 != x0)
				slope = ((float)(y1 - y0)) / ((float)(x1 - x0));
			int_array[count] = y0 + (int)(slope * (count - x0));
			count++;
		}
		else
		{
			i++;
		}
	}

	return ret;
}

const char* JSONParser::GetString(const char* name)
{
	const char* ret = nullptr;

	if (loaded_object != nullptr)
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
		if (json_object_dothas_value_of_type(loaded_object, name, JSONNumber))
		{
			ret = (int)json_object_dotget_number(loaded_object, name);
		}
		else
		{
			LOG("JSONParser: Incorrect integer value for %s.", name);
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
		LOG("JSONParser: No section loaded.");
		bparsing_success = false;
	}

	return ret;
}

bool JSONParser::GetBool(const char * name)
{
	bool ret = false;

	if (loaded_object != nullptr)
	{
		if (json_object_dothas_value_of_type(loaded_object, name, JSONBoolean))
			ret = (json_object_dotget_boolean(loaded_object, name) > 0 ? true : false);
	}

	return ret;
}

bool JSONParser::GetBoolMandatory(const char * name)
{
	bool ret = NULL;

	if (loaded_object != nullptr)
	{
		if (json_object_dothas_value_of_type(loaded_object, name, JSONBoolean))
		{
			ret = (json_object_dotget_boolean(loaded_object, name) > 0 ? true : false);
		}
		else
		{
			LOG("JSONParser: Incorrect boolean value for %s.", name);
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

int JSONParser::GetIntFromArray(size_t index_array)
{
	int ret = NULL;

	if (loaded_array != nullptr)
	{
		if (index_array < json_array_get_count(loaded_array))
		{
			ret = (int) json_array_get_number(loaded_array, index_array);
		}
		else
		{
			LOG("JSONParser: Error loading element in loaded array. Index %i out of range.", index_array);
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

const char * JSONParser::GetStringFromArray(size_t index_array)
{
	const char* ret = nullptr;

	if (loaded_array != nullptr)
	{
		if (index_array < json_array_get_count(loaded_array))
		{
			ret = json_array_get_string(loaded_array, index_array);
		}
		else
		{
			LOG("JSONParser: Error loading element in loaded array. Index %i out of range.", index_array);
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

bool JSONParser::GetRectFromArray(SDL_Rect& rect, size_t index_array)
{
	bool ret = false;

	if (loaded_array != nullptr)
	{
		if (index_array < json_array_get_count(loaded_array))
		{
			JSON_Array* in_array = json_array_get_array(loaded_array, index_array);
			if (json_array_get_count(in_array) == 4)
			{
				ret = ArrayToRect(rect, in_array);
			}
		}
		else
		{
			LOG("JSONParser: Error loading element in loaded array. Index %i out of range.", index_array);
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

bool JSONParser::GetPointFromArray(iPoint& point, size_t index_array)
{
	bool ret = false;

	if (loaded_array != nullptr)
	{
		if (index_array < json_array_get_count(loaded_array))
		{
			JSON_Array* in_array = json_array_get_array(loaded_array, index_array);
			if (json_array_get_count(in_array) == 2)
			{
				ret = ArrayToPoint(point, in_array);
			}
		}
		else
		{
			LOG("JSONParser: Error loading element in loaded array. Index %i out of range.", index_array);
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

int JSONParser::GetIntFromArrayInArray(size_t array_element, size_t index_array)
{
	int ret = NULL;

	if (loaded_array != nullptr)
	{
		if (array_element < json_array_get_count(loaded_array))
		{
			JSON_Array* in_array = json_array_get_array(loaded_array, array_element);
			if (index_array < json_array_get_count(in_array))
			{
				ret = (int) json_array_get_number(in_array, index_array);
			}
			else
			{
				LOG("JSONParser: Error loading element in array. Index %i out of range.", index_array);
				bparsing_success = false;
			}
		}
		else
		{
			LOG("JSONParser: Error loading array in loaded array. Index %i out of range.", array_element);
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

const char* JSONParser::GetStringFromArrayInArray(size_t array_element, size_t index_array)
{
	const char* ret = nullptr;

	if (loaded_array != nullptr)
	{
		if (array_element < json_array_get_count(loaded_array))
		{
			JSON_Array* in_array = json_array_get_array(loaded_array, array_element);
			if (index_array < json_array_get_count(in_array))
			{
				ret = json_array_get_string(in_array, index_array);
			}
			else
			{
				LOG("JSONParser: Error loading element in array. Index %i out of range.", index_array);
				bparsing_success = false;
			}
		}
		else
		{
			LOG("JSONParser: Error loading array in loaded array. Index %i out of range.", array_element);
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

bool JSONParser::ArrayToRect(SDL_Rect& rect, JSON_Array* rect_array)
{
	bool ret = false;

	if (json_array_get_count(rect_array) == 4)
	{
		rect.x = (int)json_array_get_number(rect_array, 0);
		rect.y = (int)json_array_get_number(rect_array, 1);
		rect.w = (int)json_array_get_number(rect_array, 2);
		rect.h = (int)json_array_get_number(rect_array, 3);
		ret = true;
	}

	return ret;
}

bool JSONParser::ArrayToPoint(iPoint& point, JSON_Array* point_array)
{
	bool ret = false;

	if (json_array_get_count(point_array) == 2)
	{
		point.x = (int)json_array_get_number(point_array, 0);
		point.y = (int)json_array_get_number(point_array, 1);
		ret = true;
	}

	return ret;
}

bool JSONParser::ArrayToPoint(fPoint& point, JSON_Array* point_array)
{
	bool ret = false;

	if (json_array_get_count(point_array) == 2)
	{
		point.x = (float) json_array_get_number(point_array, 0);
		point.y = (float) json_array_get_number(point_array, 1);
		ret = true;
	}

	return ret;
}

bool JSONParser::ArrayToPoint3d(Point3d& point, JSON_Array* point_array)
{
	bool ret = false;

	if (json_array_get_count(point_array) == 3)
	{
		point.x = (int) json_array_get_number(point_array, 0);
		point.y = (int) json_array_get_number(point_array, 1);
		point.z = (int) json_array_get_number(point_array, 2);
		ret = true;
	}

	return ret;
}
