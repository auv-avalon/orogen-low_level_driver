#include "LowLevelTask.hpp"

#include <aggregator/TimestampEstimator.hpp>

using namespace low_level_driver;

static const double UNINITIALIZED_Z_VALUE = 9999.9;


LowLevelTask::LowLevelTask(std::string const& name)
    : LowLevelTaskBase(name), timestamp_estimator(0)
{
        zOffset  = UNINITIALIZED_Z_VALUE;
	error = 0;
}

LowLevelTask::~LowLevelTask(){
}


void LowLevelTask::reset()
{
	if(llpc.isValid()){	
		llpc.reset();
	}
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See LowLevelTask.hpp for more detailed
// documentation about them.

bool LowLevelTask::configureHook()
{
	activity = getActivity<RTT::extras::FileDescriptorActivity>();

	if(!llpc.init(_port.value())){
		return false;
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
        if (activity)
        {
            activity->watch(llpc.getFileDescriptor());
            activity->setTimeout(1000);
        }
        return true;
}

void LowLevelTask::updateHook()
{
        if (activity)
        {
            if (activity->hasError() || activity->hasTimeout())
                return exception(IO_ERROR);
        }
	if(!llpc.getData()){
	    if(error++ > 50)
	            return exception(IO_ERROR);
        }else{
		error = 0;
	}


	{
		controlData::ShortExposure data;	
		while(_ShortExposure.read(data,false) == RTT::NewData){
 			printf("Got new Short Exposure %i\n",data.value);
			llpc.setShortExposure(data.value);
		}
	}
	
	{
		controlData::LongExposure data;	
		while(_LongExposure.read(data,false) == RTT::NewData){
 			printf("Got new Long Exposure %i\n",data.value);
 			llpc.setLongExposure(data.value);
		}
	}
	
	{
		controlData::LightValue data;	
		while(_LightValue.read(data,false) == RTT::NewData){
 			llpc.setServoValue(data.value);
		}
	}
	
	{
		controlData::DebugLED data;	
		while(_DebugLED.read(data,false) == RTT::NewData){
 			llpc.setLEDs(data.value);
		}
	}
	
	if (llpc.depthTime == zLastTime)
	    return;

	double zReading = llpc.depthValue;
	zLastTime = llpc.depthTime;
        base::Time now = timestamp_estimator->update(llpc.depthTime);

        if(zOffset == UNINITIALIZED_Z_VALUE){
                zOffset  = zReading;
        }
        zReading -= zOffset;

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
    if (activity)
    	activity->clearAllWatches();
    delete timestamp_estimator;
    timestamp_estimator = 0;
}
void LowLevelTask::cleanupHook()
{
    llpc.close();
}

