/**
 * Author: Matthias Goldhoorn (matthias.goldhoorn@dfki.de)
 * Company: Deutsches Forschungszentrum für Künstliche Intelligenz - Robotics Innovation Center (DFKI RIC)
 * Year 2010
 * Desc:
 *
*/
#ifndef LASERRATE_H 
#define LASERRATE_H 

#include <inttypes.h>
#include <base/time.h>
namespace controlData
{
	
	struct LaserRate{

		base::Time stamp;
		int16_t value;
#ifndef __orogen
#endif
	};
};


#endif 
