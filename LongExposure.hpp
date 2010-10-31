/**
 * Author: Matthias Goldhoorn (matthias.goldhoorn@dfki.de)
 * Company: Deutsches Forschungszentrum für Künstliche Intelligenz - Robotics Innovation Center (DFKI RIC)
 * Year 2010
 * Desc:
 *
*/
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
