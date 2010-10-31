/**
 * Author: Matthias Goldhoorn (matthias.goldhoorn@dfki.de)
 * Company: Deutsches Forschungszentrum für Künstliche Intelligenz - Robotics Innovation Center (DFKI RIC)
 * Year 2010
 * Desc:
 *
*/
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
