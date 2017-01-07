#ifndef MODULESTAGES_H
#define MODULESTAGES_H

#include "Module.h"

#define MODULESTAGES "ModuleStages"

class ModuleStages : public Module
{
public:
	ModuleStages(const char* name, bool active) : Module(name, active) {}
	~ModuleStages() {}

	virtual bool InsideScene_LeftBorder(const Point3d& positions, const Point3d& dimensions, bool in_plataform = false) const { return true; }
	virtual bool InsideScene_RightBorder(const Point3d& positions, const Point3d& dimensions, bool in_plataform = false) const { return true; }
	virtual bool InsideScene_LowBorder(const Point3d& positions, const Point3d& dimensions, bool in_plataform = false) const { return true; }
	virtual bool InsideScene_HighBorder(const Point3d& positions, const Point3d& dimensions, bool in_plataform = false) const { return true; }

	virtual bool InPlataform(int x, int z) const { return false; }

public:
	int plataform_height = 0;
};

#endif // !MODULESTAGES_H