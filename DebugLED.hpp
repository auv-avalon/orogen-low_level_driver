/**
 * Author: Matthias Goldhoorn (matthias.goldhoorn@dfki.de)
 * Company: Deutsches Forschungszentrum für Künstliche Intelligenz - Robotics Innovation Center (DFKI RIC)
 * Year 2010
 * Desc:
 *
*/
#ifndef DEBUGLED_H
#define DEBUGLED_H

#include <inttypes.h>
#include <base/time.h>

namespace controlData
{
	struct DebugLED{
		base::Time stamp;
		int8_t value;
	};

};


#endif 
