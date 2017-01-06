#ifndef MODULESTAGES_H
#define MODULESTAGES_H

#include "Module.h"

#define MODULESTAGES "ModuleStages"

class ModuleStages : public Module
{
public:
	ModuleStages(const char* name, bool active) : Module(name, active) {}
	~ModuleStages() {}

	virtual bool InsideScene_LeftBorder(const Point3d& positions, const Point3d& dimensions) const { return true; }
	virtual bool InsideScene_RightBorder(const Point3d& positions, const Point3d& dimensions) const { return true; }
	virtual bool InsideScene_LowBorder(const Point3d& positions, const Point3d& dimensions) const { return true; }
	virtual bool InsideScene_HighBorder(const Point3d& positions, const Point3d& dimensions) const { return true; }

private:

};

#endif // !MODULESTAGES_H