#include "LowLevelTask.hpp"

#include <rtt/FileDescriptorActivity.hpp>
#include <TimestampEstimator.hpp>

using namespace low_level_driver;

static const double UNINITIALIZED_Z_VALUE = 9999.9;

RTT::FileDescriptorActivity* LowLevelTask::getFileDescriptorActivity()
{ return dynamic_cast< RTT::FileDescriptorActivity* >(getActivity().get()); }


LowLevelTask::LowLevelTask(std::string const& name)
    : LowLevelTaskBase(name), timestamp_estimator(0)
{
        zOffset  = UNINITIALIZED_Z_VALUE;
}





/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See LowLevelTask.hpp for more detailed
// documentation about them.

bool LowLevelTask::configureHook()
{
	if(!llpc.init(_port.value())){
		return false;
	}

        RTT::FileDescriptorActivity* activity = getFileDescriptorActivity();
        if (activity)
        {
            activity->watch(llpc.getFileDescriptor());
            activity->setTimeout(1000);
        }
 	llpc.setShortExposure(_shortExposure.value());
 	llpc.setLongExposure(_longExposure.value());
        zOffset  = UNINITIALIZED_Z_VALUE;
	return true;
}
bool LowLevelTask::startHook()
{
        zCurrent.invalidate();
        zCurrent.time = base::Time();
	llpc.clear();
	timestamp_estimator = new aggregator::TimestampEstimator(base::Time::fromSeconds(2));
        return true;
}

void LowLevelTask::updateHook(std::vector<RTT::PortInterface*> const& updated_ports)
{
        RTT::FileDescriptorActivity* activity = getFileDescriptorActivity();
        if (activity)
        {
            if (activity->hasError() || activity->hasTimeout())
                return fatal(IO_ERROR);
        }

//	if(isPortUpdated(_ShortExposure))
	{
		controlData::ShortExposure data;	
		if(!_ShortExposure.read(data)){
			fprintf(stderr,"Data not availible yet\n");	
		}else{
 			printf("Got new Short Exposure %i\n",data.value);
			llpc.setShortExposure(data.value);
		}
	}
//	if(isPortUpdated(_LongExposure))
	{
		controlData::LongExposure data;	
		if(!_LongExposure.read(data)){
			fprintf(stderr,"Data not availible yet\n");	
		}else{
 			printf("Got new Long Exposure %i\n",data.value);
 			llpc.setLongExposure(data.value);
		}
	}
//	if(isPortUpdated(_ServoValue))
	{
		controlData::ServoValue data;	
		if(!_ServoValue.read(data)){
			fprintf(stderr,"Data not availible yet\n");	
		}else{
 			llpc.setServoValue(data.value);
		}
	}
//	if(isPortUpdated(_DebugLED))
	{
		controlData::DebugLED data;	
		if(!_DebugLED.read(data)){
			fprintf(stderr,"Data not availible yet\n");	
		}else{
 			llpc.setLEDs(data.value);
		}
	}
	
        double zReading;
	if(!llpc.getData(zReading)){
            return fatal(IO_ERROR);
        }

        if(zOffset == UNINITIALIZED_Z_VALUE){
                zOffset  = zReading;
        }
        zReading -= zOffset;
        base::Time now = timestamp_estimator->update(base::Time::now());

        if(zCurrent.time.isNull())
        {
                zCurrent.position.z() = zReading;
                zCurrent.cov_position(2, 2) = 0.5;
        }
        else
        {
                double z_iir = _zIIR.get();
                double zNew  = zCurrent.position.z() * (1 - z_iir) + zReading * z_iir;

                double delta_t = 0.0879; // update rate of the depth sensor
                double zNewVelocity = (zNew - zCurrent.position.z())
                    / delta_t;


                double z_velocity_iir = _zVelocityIIR.get();
                if (isinf(zCurrent.cov_velocity(2, 2)))
                    zCurrent.velocity.z() = zNewVelocity;
                else
                    zCurrent.velocity.z() = zCurrent.velocity.z() * (1 - z_velocity_iir) + zNewVelocity * z_velocity_iir;

                zCurrent.position.z() = zNew;
                zCurrent.cov_velocity(2, 2) = 0.1;
        }

        zCurrent.time = now;
        _depth_samples.write(zCurrent);
}

// void LowLevelTask::errorHook()
// {
// }
void LowLevelTask::stopHook()
{
    delete timestamp_estimator;
    timestamp_estimator = 0;
}
void LowLevelTask::cleanupHook()
{
    llpc.close();
}

