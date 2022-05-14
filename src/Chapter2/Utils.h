#pragma once
#include <cstdlib>
#include <time.h>


namespace Utils
{
	int random(int min, int max) //range : [min, max]
	{
		static bool first = true;
		if (first)
		{
			srand(time(NULL)); //seeding for the first time only!
			first = false;
		}
		return min + rand() % ((max + 1) - min);
	}
}
