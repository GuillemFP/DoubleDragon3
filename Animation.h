#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "Globals.h"
#include "SDL/include/SDL_rect.h"

#define ANIMATION_SPEED "Speed"
#define ANIMATION_FRAMES "Frames"
#define ANIMATION_SPRITES "Sprites"
#define ANIMATION_LOOP "Loop"

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

	int GetCurrentFrameNumber() const { return (int)current_frame; }

	bool Finished() const { return loops > 0; }

	int GetNumberOfFrames() const { return frames.size(); }

	void Reset()
	{
		current_frame = 0.0f;
		loops = 0;
	}

public:
	std::vector<SDL_Rect> frames;
	float speed = 1.0f;
	bool bloop = true;

private:
	float current_frame = 0.0f;
	int loops = 0;
};

#endif // !ANIMATION_H