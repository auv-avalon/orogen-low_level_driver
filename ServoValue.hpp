#ifndef SERVOVALUE_H 
#define SERVOVALUE_H

#include <inttypes.h>
#include <dfki/time.h>

namespace controlData
{
	struct ServoValue{
		DFKI::Time stamp;
		int16_t value;
	};

};


#endif 
