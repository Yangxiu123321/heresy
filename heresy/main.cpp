// STL Header
#include <iostream>

// 1. include OpenNI Header
#include "OpenNI.h"

#include "opencv2/opencv.hpp"

int main(int argc, char** argv)
{
	openni::Status status = openni::STATUS_OK;
	// 2. initialize OpenNI
	status = openni::OpenNI::initialize();
	if (status != openni::STATUS_OK)
	{
		std::cout << "�����ʼ��ʧ��" << std::endl;
	}

	// 3. open a device
	openni::Device devAnyDevice;
	status = devAnyDevice.open(openni::ANY_DEVICE);
	if (status != openni::STATUS_OK)
	{
		std::cout << "�����ʧ��" << std::endl;
	}

	// �������Ƿ�֧���ӽǽ�������
	//if (devAnyDevice.isImageRegistrationModeSupported(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR))
	//	devAnyDevice.setImageRegistrationMode(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR);

	// 4. create depth stream
	openni::VideoStream streamDepth;
	status = streamDepth.create(devAnyDevice, openni::SENSOR_DEPTH);
	if (status != openni::STATUS_OK)
	{
		std::cout << "�������������ʧ��" << std::endl;
		return -1;
	}

	// ���������ȵ�ģʽ
	// set video mode
	openni::VideoMode setDepthMode;
	setDepthMode.setFps(30);
	setDepthMode.setResolution(640, 480);
	setDepthMode.setPixelFormat(openni::PIXEL_FORMAT_RGB888);
	setDepthMode.setPixelFormat(openni::PIXEL_FORMAT_DEPTH_1_MM);
	if (streamDepth.setVideoMode(setDepthMode) != openni::STATUS_OK)
	{
		std::cout << "���ģʽ����ʧ��" << std::endl;
		return -1;
	}

	// 4a. create color stream
	openni::VideoStream streamColor;
	status = streamColor.create(devAnyDevice, openni::SENSOR_COLOR);
	if (streamDepth.setVideoMode(setDepthMode) != openni::STATUS_OK)
	{
		std::cout << "�������������ʧ��" << std::endl;
		return -1;
	}

	// ���������ɫ��ģʽ
	// set video mode
	openni::VideoMode setColorMode;
	setColorMode.setFps(30);
	setColorMode.setResolution(640, 480);
	setColorMode.setPixelFormat(openni::PIXEL_FORMAT_RGB888);
	setColorMode.setPixelFormat(openni::PIXEL_FORMAT_DEPTH_1_MM);
	if (streamDepth.setVideoMode(setColorMode) != openni::STATUS_OK)
	{
		std::cout << "��ɫģʽ����ʧ��" << std::endl;
		return -1;
	}
	// �Զ��ع�
	openni::CameraSettings *cameraSetting = streamColor.getCameraSettings();
	if (cameraSetting == NULL)
	{
		std::cout << "�ع�����ô���ʧ��" << std::endl;
		return -1;
	}
	status = cameraSetting->setAutoExposureEnabled(true);
	if (status != openni::STATUS_OK)
	{
		std::cout << "�Զ��ع�����ʧ��" << std::endl;
		return -1;
	}
	const int iMaxDepth = streamDepth.getMaxPixelValue();
	// 5 main loop, continue read
	openni::VideoFrameRef frameDepth;
	openni::VideoFrameRef frameColor;
	streamDepth.start();
	streamColor.start();
	cv::namedWindow("color",cv::WINDOW_AUTOSIZE);
	cv::namedWindow("depth", cv::WINDOW_AUTOSIZE);
	while(true)
	{
		// 5.1 get frame
		streamDepth.readFrame(&frameDepth);
		streamColor.readFrame(&frameColor);

		// �õ���ɫͼ��
		cv::Mat mImageRGB(frameColor.getHeight(), frameColor.getWidth(),
			CV_8UC3, (void*)frameColor.getData());
		cv::Mat mImageBGR;
		cv::cvtColor(mImageRGB, mImageBGR, CV_RGB2BGR);
		cv::imshow("color", mImageBGR);

		// �õ����ͼ
		cv::Mat mImageDepthTmp(frameDepth.getHeight(), frameDepth.getWidth(),
			CV_16UC1, (void*)frameDepth.getData());
		cv::Mat mImageDepth;
		mImageDepthTmp.convertTo(mImageDepth, CV_8U, 255.0f / iMaxDepth);
		cv::imshow("depth", mImageDepth);

		if (cv::waitKey(1) == 'q')
		{
			break;
		}
	    
	}

	// 6. close
	streamDepth.destroy();
	streamColor.destroy();
	devAnyDevice.close();
	
	// 7. shutdown
	openni::OpenNI::shutdown();

	return 0;
}