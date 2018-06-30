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
class GetCamera : public OpenNI::DeviceConnectedListener
{
public:

	void onDeviceConnected(const DeviceInfo* rDevInfo)
	{
		Device orbbec;
		if (orbbec.open(rDevInfo->getUri()) == STATUS_OK)
		{
			std::cout << "open ok" << std::endl;
		}
		else
		{
			std::cerr << OpenNI::getExtendedError() << std::endl;
		}
	}
};

int main(int argc, char** argv)
{
	// 2. initialize OpenNI
	OpenNI::initialize();

	GetCamera getOrbbec;

	OpenNI::addDeviceConnectedListener(&getOrbbec);
	

	// 5 wait any input (some char and Enter) to quit
	string sInput;
	cin >> sInput;

	// 7. shutdown
	OpenNI::shutdown();

	return 0;

	
}