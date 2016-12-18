#ifndef MODULE_H
#define MODULE_H

#include "Globals.h"

class Module
{
public:
	Module(const char* name, bool active = true) : strname(name), bactive(active) {}
	
	virtual ~Module() {}

	bool IsEnabled() const
	{
		return bactive;
	}

	bool Enable()
	{
		if (bactive == false)
		{
			bactive = Start();
			if (bactive == false)
				LOG("Module unable to start correctly: %s ---", strname);
			return bactive;
		}
		else
		{
			LOG("Module already active: %s --------------", strname);
			return true;
		}

		return true;
	}

	bool Disable()
	{
		if (bactive == true)
		{
			bactive = !CleanUp();
			if (bactive == true)
				LOG("Module unable to cleanup correctly: %s -", strname);
			return !bactive;
		}
		else
		{
			LOG("Module already disabled: %s ------------", strname);
			return true;
		}

		return true;
	}

	virtual bool Init()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp()
	{
		return true;
	}

private:
	const char* strname = "";
	bool bactive = true;
};

#endif // !MODULE_H