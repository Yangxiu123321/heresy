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

	// 4a. create color stream
	openni::VideoStream streamColor;
	streamColor.create(devAnyDevice, openni::SENSOR_COLOR);
	streamColor.start();

	// 5 main loop, continue read
	openni::VideoFrameRef frameDepth;
	openni::VideoFrameRef frameColor;
	for (int i = 0; i < 100; ++i)
	{
		// 5.1 get frame
		streamDepth.readFrame(&frameDepth);
		streamColor.readFrame(&frameColor);

		// 5.2 get data array
		const openni::DepthPixel* pDepth
			= (const openni::DepthPixel*)frameDepth.getData();
		const openni::RGB888Pixel* pColor
			= (const openni::RGB888Pixel*)frameColor.getData();

		// 5.3 output the depth value of center point
		int idx = frameDepth.getWidth() * (frameDepth.getHeight() + 1) / 2;
		std::cout << pDepth[idx] << "( "
			<< (int)pColor[idx].r << ","
			<< (int)pColor[idx].g << ","
			<< (int)pColor[idx].b << ")"
			<< std::endl;
	}

	// 6. close
	streamDepth.destroy();
	streamColor.destroy();
	devAnyDevice.close();

	// 7. shutdown
	openni::OpenNI::shutdown();

	return 0;
}