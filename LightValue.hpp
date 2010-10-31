/**
 * Author: Matthias Goldhoorn (matthias.goldhoorn@dfki.de)
 * Company: Deutsches Forschungszentrum für Künstliche Intelligenz - Robotics Innovation Center (DFKI RIC)
 * Year 2010
 * Desc:
 *
*/
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
