#ifndef MODULESTAGES_H
#define MODULESTAGES_H

#include "Module.h"
#include "Point.h"
#include <vector>

#define MODULESTAGES "ModuleStages"

class Timer;

class ModuleStages : public Module
{
public:
	ModuleStages(const char* name, bool active) : Module(name, active) {}
	~ModuleStages() {}

	virtual bool CleanUp();
	virtual update_status Update();

	virtual bool InsideScene_LeftBorder(const Point3d& positions, const Point3d& dimensions, bool in_plataform = false) const { return true; }
	virtual bool InsideScene_RightBorder(const Point3d& positions, const Point3d& dimensions, bool in_plataform = false) const { return true; }
	virtual bool InsideScene_LowBorder(const Point3d& positions, const Point3d& dimensions, bool in_plataform = false) const { return true; }
	virtual bool InsideScene_HighBorder(const Point3d& positions, const Point3d& dimensions, bool in_plataform = false) const { return true; }

	virtual bool InPlataform(int x, int z) const { return false; }
	virtual bool InPlataform(const iPoint& point) const { return InPlataform(point.x, point.y); }

public:
	int plataform_height = 0;
	int enemies_killed = 0;
	int enemies_stage = 0;
	int max_number_enemies = 0;
	int current_enemies = 0;

protected:
	std::vector<iPoint> spawn_points;
	Timer* spawn_clock;
	bool game_over = false;
};

#endif // !MODULESTAGES_H