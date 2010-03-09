#include "LowLevelTask.hpp"

#include <rtt/FileDescriptorActivity.hpp>


using namespace low_level_driver;


RTT::FileDescriptorActivity* LowLevelTask::getFileDescriptorActivity()
{ return dynamic_cast< RTT::FileDescriptorActivity* >(getActivity().get()); }


LowLevelTask::LowLevelTask(std::string const& name)
    : LowLevelTaskBase(name)
{
	depthOffset=9999.9;
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
	return true;
}
// bool LowLevelTask::startHook()
// {
//     return true;
// }

void LowLevelTask::updateHook(std::vector<RTT::PortInterface*> const& updated_ports)
{
	try{
	if(isPortUpdated(_ShortExposure)){
		controlData::ShortExposure data;	
		if(!_ShortExposure.read(data)){
			fprintf(stderr,"Data not availible yet\n");	
		}else{
 			llpc.setShortExposure(data.value);
		}
	}
	if(isPortUpdated(_LongExposure)){
		controlData::LongExposure data;	
		if(!_LongExposure.read(data)){
			fprintf(stderr,"Data not availible yet\n");	
		}else{
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
	
	sensorData::DepthReading depth;	
	if(llpc.getData(depth.value)){
		if(depthOffset==9999.9){
			depthOffset=depth.value;
		}
		depth.value-=depthOffset;	
		depth.stamp = base::Time::now();
		_Depth.write(depth);
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

