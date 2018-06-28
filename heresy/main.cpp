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


	// get cameta infomation(得到相机支持的格式)
	const openni::SensorInfo& rInfo = streamDepth.getSensorInfo();
	const openni::Array<openni::VideoMode>& aModes = rInfo.getSupportedVideoModes();
	for (int i = 0; i < aModes.getSize(); ++i)
	{
		const openni::VideoMode& rMode = aModes[i];
		std::cout << "Video Mode : " << rMode.getResolutionX();
		std::cout << " * " << rMode.getResolutionY();
		std::cout << " @ " << rMode.getFps() << "FPS";
		switch (rMode.getPixelFormat())
		{
		case openni::PIXEL_FORMAT_DEPTH_1_MM:
			std::cout << " , Unit is 1mm" << std::endl;
			break;

		case openni::PIXEL_FORMAT_DEPTH_100_UM:
			std::cout << " , Unit is 100um" << std::endl;
			break;
		default:
			break;
		}
	}

	// 设置相机的模式
	// set video mode
	openni::VideoMode setVmMode;
	setVmMode.setFps(30);
	setVmMode.setResolution(6400, 480);
	setVmMode.setPixelFormat(openni::PIXEL_FORMAT_DEPTH_1_MM);
	if (streamDepth.setVideoMode(setVmMode) == openni::STATUS_OK)
	{
		// OK
		std::cout << "set ok\n";
	}

	// 读取相机的模式
	openni::VideoMode vmMode = streamDepth.getVideoMode();
	std::cout << "Video Mode : " << vmMode.getResolutionX();
	std::cout << " * " << vmMode.getResolutionY();
	std::cout << " @ " << vmMode.getFps() << "FPS";
	switch (vmMode.getPixelFormat())
	{
	case openni::PIXEL_FORMAT_DEPTH_1_MM:
		std::cout << " , Unit is 1mm" << std::endl;
		break;

	case openni::PIXEL_FORMAT_DEPTH_100_UM:
		std::cout << " , Unit is 100um" << std::endl;
		break;
	}

	// 获得相机像素的极大值与极小值
	const int maxPixelValue = streamDepth.getMaxPixelValue();
	const int minPixelValue = streamDepth.getMinPixelValue();
	std::cout <<"maxPixelValue:" << maxPixelValue << std::endl;
	std::cout <<"minPixelValue:" << minPixelValue << std::endl;


	// 取得水平／垂直的 FOV。(视角)
	const float hFovColor = streamColor.getHorizontalFieldOfView();
	const float vFovColor = streamColor.getVerticalFieldOfView();
	std::cout << "hColorFOV:" << hFovColor << std::endl;
	std::cout << "vColorFOV:" << vFovColor << std::endl;
	const float hFovDepth = streamDepth.getHorizontalFieldOfView();
	const float vFovDepth = streamDepth.getVerticalFieldOfView();
	std::cout << "hDepthFOV:" << hFovDepth << std::endl;
	std::cout << "vDepthFOV:" << vFovDepth << std::endl;


	// 相机的白平衡及常用设置(彩色图像)
	openni::CameraSettings *cameraSetting = streamColor.getCameraSettings();
	if (cameraSetting == NULL)
	{
		std::cout << "not support Set" << std::endl;
	}
	cameraSetting->setAutoExposureEnabled(false);
	bool autoExposureValue = cameraSetting->getAutoExposureEnabled();
	std::cout << "自动曝光值:" << autoExposureValue << std::endl;
	cameraSetting->setExposure(15);
	const int exposureValue = cameraSetting->getExposure();
	std::cout << "手动曝光值：" << exposureValue << std::endl;
	cameraSetting->setAutoExposureEnabled(true);

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