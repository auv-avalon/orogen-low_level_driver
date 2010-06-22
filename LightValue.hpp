#ifndef LIGHTVALUE_H 
#define LIGHTVALUE_H

#include <inttypes.h>
#include <base/time.h>

namespace controlData
{
	struct LightValue{
		base::Time stamp;
		int16_t value;
	};

};


#endif 
