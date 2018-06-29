// STL Header
#include <iostream>
#include <string>

// 1. include OpenNI Header
#include <OpenNI.h>

// using namespace
using namespace std;
using namespace openni;

/**
* The listener for depth video stream
*/
class OutputDpeth : public VideoStream::NewFrameListener
{
public:
	// this function will be called when video stream get new frame
	void onNewFrame(VideoStream& rStream)
	{
		VideoFrameRef vfDepth;
		if (rStream.readFrame(&vfDepth) == STATUS_OK)
		{
			// a1 get data array
			const DepthPixel* pDepth
				= static_cast<const DepthPixel*>(vfDepth.getData());

			// a2 output the depth value of center point
			int w = vfDepth.getWidth(),
				h = vfDepth.getHeight();
			int x = w / 2, y = h / 2;
			cout << pDepth[x + y * w] << endl;

			// a3 release frame
			vfDepth.release();
		}
		else
		{
			cerr << "Can not read frame\n";
			cerr << OpenNI::getExtendedError() << endl;
		}
	}
};

int main(int argc, char** argv)
{
	// 2. initialize OpenNI
	OpenNI::initialize();

	// 3. open a device
	Device devDevice;
	devDevice.open(ANY_DEVICE);

	// 4. create depth stream
	VideoStream vsDepth;
	vsDepth.create(devDevice, SENSOR_DEPTH);

	OutputDpeth mOutputDepth;

	vsDepth.addNewFrameListener(&mOutputDepth);
	
	vsDepth.start();

	// 5 wait any input (some char and Enter) to quit
	string sInput;
	cin >> sInput;

	// 6 stop reading
	vsDepth.removeNewFrameListener(&mOutputDepth);
	vsDepth.stop();

	// destroy depth stream
	vsDepth.destroy();

	// close device
	devDevice.close();

	// 7. shutdown
	OpenNI::shutdown();

	return 0;
}