#ifndef LONGEXPOSURE_H 
#define LONGEXPOSURE_H 

#include <inttypes.h>
#include <dfki/time.h>

namespace controlData
{
	struct LongExposure{
		DFKI::Time stamp;
		int16_t value;
	};

};


#endif 
