#ifndef DEBUGLED_H
#define DEBUGLED_H

#include <inttypes.h>
#include <base/time.h>

namespace controlData
{
	struct DebugLED{
		base::Time stamp;
		int8_t value;
	};

};


#endif 
