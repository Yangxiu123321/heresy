// STL Header
#include <iostream>

// 1. include OpenNI Header
#include "OpenNI.h"

int main(int argc, char** argv)
{
	// 2. initialize OpenNI
	openni::OpenNI::initialize();

	// 3. open a device
	openni::Device devAnyDevice;
	devAnyDevice.open(openni::ANY_DEVICE);

	// 4. create depth stream
	openni::VideoStream streamDepth;
	streamDepth.create(devAnyDevice, openni::SENSOR_DEPTH);
	streamDepth.start();

	// 5 main loop, continue read
	openni::VideoFrameRef frameDepth;
	for (int i = 0; i < 100; ++i)
	{
		// 5.1 get frame
		streamDepth.readFrame(&frameDepth);

		// 5.2 get data array
		const openni::DepthPixel* pDepth
			= (const openni::DepthPixel*)frameDepth.getData();

		// 5.3 output the depth value of center point
		int idx = frameDepth.getWidth() * (frameDepth.getHeight() + 1) / 2;
		std::cout << pDepth[idx] << std::endl;
	}

	// 6. close
	streamDepth.destroy();
	devAnyDevice.close();

	// 7. shutdown
	openni::OpenNI::shutdown();

	return 0;
}