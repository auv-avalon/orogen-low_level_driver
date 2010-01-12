#ifndef DEPTHREADING_H
#define DEPTHREADING_H

#include <base/time.h>

namespace sensorData
{
	struct DepthReading{
		base::Time stamp;
		double value;
	};

};


#endif 
