#ifndef DEPTHREADING_H
#define DEPTHREADING_H

#include <dfki/time.h>

namespace sensorData
{
	struct DepthReading{
		DFKI::Time stamp;
		double value;
	};

};


#endif 
