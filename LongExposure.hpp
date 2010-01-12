#ifndef LONGEXPOSURE_H 
#define LONGEXPOSURE_H 

#include <inttypes.h>
#include <base/time.h>

namespace controlData
{
	struct LongExposure{
		base::Time stamp;
		int16_t value;
	};

};


#endif 
