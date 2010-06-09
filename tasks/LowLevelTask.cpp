#include "LowLevelTask.hpp"

#include <rtt/FileDescriptorActivity.hpp>


using namespace low_level_driver;

static const double UNINITIALIZED_DEPTH_VALUE = 9999.9;

RTT::FileDescriptorActivity* LowLevelTask::getFileDescriptorActivity()
{ return dynamic_cast< RTT::FileDescriptorActivity* >(getActivity().get()); }


LowLevelTask::LowLevelTask(std::string const& name)
    : LowLevelTaskBase(name)
{
        depthOffset  = UNINITIALIZED_DEPTH_VALUE;
        depthCurrent = UNINITIALIZED_DEPTH_VALUE;
}





/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See LowLevelTask.hpp for more detailed
// documentation about them.

bool LowLevelTask::configureHook()
{
	if(!llpc.init(_port.value())){
		return false;
	}else{
	    //if(getFileDescriptorActivity() == 0){
		//fprintf(stderr,"Cannot use File Descriptor Activity, did you use periodic?\n");
		//return false;
	    //}else{
		//getFileDescriptorActivity()->watch(llpc.getReadFD());
	    //}
	}
 	llpc.setShortExposure(_shortExposure.value());
 	llpc.setLongExposure(_longExposure.value());
        depthOffset  = UNINITIALIZED_DEPTH_VALUE;
        depthCurrent = UNINITIALIZED_DEPTH_VALUE;
	return true;
}
bool LowLevelTask::startHook()
{
        depthCurrent = UNINITIALIZED_DEPTH_VALUE;
        return true;
}

void LowLevelTask::updateHook(std::vector<RTT::PortInterface*> const& updated_ports)
{
	try{
	if(isPortUpdated(_ShortExposure)){
		controlData::ShortExposure data;	
		if(!_ShortExposure.read(data)){
			fprintf(stderr,"Data not availible yet\n");	
		}else{
 			printf("Got new Short Exposure %i\n",data.value);
			llpc.setShortExposure(data.value);
		}
	}
	if(isPortUpdated(_LongExposure)){
		controlData::LongExposure data;	
		if(!_LongExposure.read(data)){
			fprintf(stderr,"Data not availible yet\n");	
		}else{
 			printf("Got new Long Exposure %i\n",data.value);
 			llpc.setLongExposure(data.value);
		}
	}
	if(isPortUpdated(_ServoValue)){
		controlData::ServoValue data;	
		if(!_ServoValue.read(data)){
			fprintf(stderr,"Data not availible yet\n");	
		}else{
 			llpc.setServoValue(data.value);
		}
	}
	if(isPortUpdated(_DebugLED)){
		controlData::DebugLED data;	
		if(!_DebugLED.read(data)){
			fprintf(stderr,"Data not availible yet\n");	
		}else{
 			llpc.setLEDs(data.value);
		}
	}
	
        double depthReading;
	if(llpc.getData(depthReading)){
		if(depthOffset == UNINITIALIZED_DEPTH_VALUE){
			depthOffset  = depthReading;
		}
		if(depthCurrent == UNINITIALIZED_DEPTH_VALUE){
			depthCurrent  = depthReading;
		}
                depthReading -= depthOffset;
                double iir = _depthIIR.get();
                depthCurrent = iir * depthReading + (1 - iir) * depthCurrent;

                base::samples::RigidBodyState depth_sample;
                depth_sample.invalidate();
		depth_sample.time = base::Time::now();
		depth_sample.position.z() = depthCurrent;
                depth_sample.cov_position(2, 2) = 0.5;

		_depth_samples.write(depth_sample);
	}
	}catch(...)
	{printf("error tiefensensor lowLevel Task skipping...\n");}

}

// void LowLevelTask::errorHook()
// {
// }
// void LowLevelTask::stopHook()
// {
// }
// void LowLevelTask::cleanupHook()
// {
// }

