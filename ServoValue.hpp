#ifndef SERVOVALUE_H 
#define SERVOVALUE_H

#include <inttypes.h>
#include <base/time.h>

namespace controlData
{
	struct ServoValue{
		base::Time stamp;
		int16_t value;
	};

};


#endif 
