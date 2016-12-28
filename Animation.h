#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "Globals.h"
#include "JsonHandler.h"
#include "SDL/include/SDL_rect.h"

#define ANIMATION_SPEED "Speed"
#define ANIMATION_FRAMES "Frames"
#define ANIMATION_SPRITES "Sprites"

class Animation
{
public:
	Animation() {};
	~Animation() {};

	SDL_Rect& GetCurrentFrame()
	{
		float last_frame = (float)frames.size();
		current_frame += speed;
		if (current_frame >= last_frame)
		{
			current_frame = (bloop == true) ? 0.0f : MAX(last_frame - 1.0f, 0.0f);
			loops++;
		}
		return frames[(int)current_frame];
	}

	bool LoadAnimation(JSONParser* parser, const char* name)
	{
		bool ret = true;
		SDL_Rect rect = { 0,0,0,0 };

		if (parser->LoadObject(name) == true)
		{
			iframes = parser->GetInt(ANIMATION_FRAMES);
			speed = parser->GetFloat(ANIMATION_SPEED);

			if (parser->LoadArrayInObject(ANIMATION_SPRITES))
			{
				for (int i = 0; i < iframes; i++)
				{
					rect.x = parser->GetRectFromArray(i, 0);
					rect.y = parser->GetRectFromArray(i, 1);
					rect.w = parser->GetRectFromArray(i, 2);
					rect.h = parser->GetRectFromArray(i, 3);
					frames.push_back(rect);
				}
			}

			ret = parser->UnloadObject();
		}
		else
			ret = false;

		return ret;
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0.0f;
	}

public:
	std::vector<SDL_Rect> frames;
	float speed = 1.0f;
	bool bloop = true;

private:
	float current_frame = 0.0f;
	int loops = 0;
	int iframes = 0;
};

#endif // !ANIMATION_H