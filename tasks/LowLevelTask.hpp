#ifndef LOW_LEVEL_DRIVER_LOWLEVELTASK_TASK_HPP
#define LOW_LEVEL_DRIVER_LOWLEVELTASK_TASK_HPP

#include "low_level_driver/LowLevelTaskBase.hpp"
#include <lowlevel_processor.h>

#include <rtt/extras/FileDescriptorActivity.hpp>

namespace RTT
{
    class FileDescriptorActivity;
}

namespace aggregator
{
    class TimestampEstimator;
}

namespace low_level_driver {
    class LowLevelTask : public LowLevelTaskBase
    {
	friend class LowLevelTaskBase;
    protected:
	aggregator::TimestampEstimator* timestamp_estimator;
	RTT::extras::FileDescriptorActivity* activity;
	int error;
    public:
        ~LowLevelTask();
        LowLevelTask(std::string const& name = "low_level_driver::LowLevelTask");

	void reset();

        /** This hook is called by Orocos when the state machine transitions
         * from PreOperational to Stopped. If it returns false, then the
         * component will stay in PreOperational. Otherwise, it goes into
         * Stopped.
         *
         * It is meaningful only if the #needs_configuration has been specified
         * in the task context definition with (for example):
         *
         *   task_context "TaskName" do
         *     needs_configuration
         *     ...
         *   end
         */
         bool configureHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Stopped to Running. If it returns false, then the component will
         * stay in Stopped. Otherwise, it goes into Running and updateHook()
         * will be called.
         */
        bool startHook();

        /** This hook is called by Orocos when the component is in the Running
         * state, at each activity step. Here, the activity gives the "ticks"
         * when the hook should be called. See README.txt for different
         * triggering options.
         *
         * The warning(), error() and fatal() calls, when called in this hook,
         * allow to get into the associated RunTimeWarning, RunTimeError and
         * FatalError states. 
         *
         * In the first case, updateHook() is still called, and recovered()
         * allows you to go back into the Running state.  In the second case,
         * the errorHook() will be called instead of updateHook() and in the
         * third case the component is stopped and resetError() needs to be
         * called before starting it again.
         *
         */
         void updateHook();
        

        /** This hook is called by Orocos when the component is in the
         * RunTimeError state, at each activity step. See the discussion in
         * updateHook() about triggering options.
         *
         * Call recovered() to go back in the Runtime state.
         */
        // void errorHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Running to Stopped after stop() has been called.
         */
        void stopHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Stopped to PreOperational, requiring the call to configureHook()
         * before calling start() again.
         */
        void cleanupHook();
	private:
		LowLevelProcessor llpc;
		/** Time of last depth reading on llpc
		 * It is different from zCurrent.time */
		base::Time zLastTime;
                /** Last depth reading */
                base::samples::RigidBodyState zCurrent;
                /** Depth offset (i.e. depth reading at z=0) */
		double zOffset;
    };
}

#endif

