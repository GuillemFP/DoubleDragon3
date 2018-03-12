#ifndef TIMER_H
#define TIMER_H

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

	void Start() { Start(max_time); }

	void Start(float max_time) { Start((Uint32)max_time * 1000); }

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

	Uint32 GetTimeInMs() const { return accumulated_time + CurrentTime(); }

	float GetTimeInSeconds() const { return ((float) GetTimeInMs()/1000.0f); }

	Uint32 GetCounterInMs() const { return (max_time - GetTimeInMs() >= 0 ? max_time - GetTimeInMs() : 0); }

	int GetCounterInS() const { return (GetCounterInMs() / 1000); }

	TimerState GetState() const { return state; }

	Uint32 GetMaxTimeInMs() const { return max_time; }

	void DoubleMaxTime() { max_time *= 2; }

	bool MaxTimeReached() const
	{
		if (max_time > 0)
		{
			return (accumulated_time + CurrentTime() >= max_time);
		}
		else
			return false;
	}

	void SetMaxTime(Uint32 max_time)
	{
		this->max_time = max_time;
		Stop();
	}

private:
	Uint32 CurrentTime() const
	{
		if (state == PAUSED || state == OFF)
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