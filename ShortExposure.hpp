#ifndef SHORTEXPOSURE_H 
#define SHORTEXPOSURE_H 

#include <inttypes.h>
#include <dfki/time.h>

namespace controlData
{
	
	struct ShortExposure{
//	ShortExposure(uint16_t v){
//		value = v;
//	}
		DFKI::Time stamp;
		int16_t value;
#ifndef __orogen
#endif
	};
};


#endif 
