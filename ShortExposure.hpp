#ifndef SHORTEXPOSURE_H 
#define SHORTEXPOSURE_H 

#include <inttypes.h>
#include <base/time.h>

namespace controlData
{
	
	struct ShortExposure{
//	ShortExposure(uint16_t v){
//		value = v;
//	}
		base::Time stamp;
		int16_t value;
#ifndef __orogen
#endif
	};
};


#endif 
