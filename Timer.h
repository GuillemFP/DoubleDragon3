#ifndef TIMER_H
#define TIMER

#include "SDL/include/SDL_timer.h"

enum TimerState
{
	OFF,
	RUNNING,
	PAUSED
};

class Timer
{
public:
	Timer(Uint32 max_time = 0) : max_time(max_time) {};
	~Timer() {};

	void Start()
	{
		Start((Uint32)0);
	}

	void Start(float max_time)
	{
		Start((Uint32)max_time * 1000);
	}

	void Start(Uint32 max_time)
	{
		if (state == OFF)
		{
			start_time = SDL_GetTicks();
			this->max_time = max_time;
			state = RUNNING;
		}
	}

	void Stop()
	{
		start_time = 0;
		accumulated_time = 0;
		state = OFF;
	}

	void Reset()
	{
		Stop();
		Start(max_time);
	}

	void Pause()
	{
		if (state == RUNNING)
		{
			accumulated_time += SDL_GetTicks() - start_time;
			start_time = 0;
			state = PAUSED;
		}
	}

	void Resume()
	{
		if (state == PAUSED)
		{
			start_time = SDL_GetTicks();
			state = RUNNING;
		}
	}

	Uint32 GetTimeInMs()
	{
		return accumulated_time + CurrentTime();
	}

	float GetTimeInSeconds()
	{
		return ((float) GetTimeInMs()/1000.0f);
	}

	TimerState GetState()
	{
		return state;
	}

	Uint32 GetMaxTimeInMs()
	{
		return max_time;
	}

	bool MaxTimeReached()
	{
		if (max_time > 0)
		{
			return (accumulated_time + CurrentTime() >= max_time);
		}
		else
			return false;
	}

private:
	Uint32 CurrentTime()
	{
		if (state == PAUSED)
			return 0;
		else
			return SDL_GetTicks() - start_time;
	}

private:
	Uint32 start_time = 0;
	Uint32 accumulated_time = 0;
	Uint32 max_time = 0;
	TimerState state = OFF;
};

#endif // !TIMER_H