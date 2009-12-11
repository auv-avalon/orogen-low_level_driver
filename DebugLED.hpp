#ifndef DEBUGLED_H
#define DEBUGLED_H

#include <inttypes.h>
#include <dfki/time.h>

namespace controlData
{
	struct DebugLED{
		DFKI::Time stamp;
		int8_t value;
	};

};


#endif 
