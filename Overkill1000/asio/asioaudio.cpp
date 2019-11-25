#include "asioaudio.h"
#include "asioconvert.h"
#include "asiodriverlist.h"
#include <iasiodrv.h>
#include <asiosys.h>
#include <asio.h>
#include "engine\engine.h"

extern Engine *engine;

IASIO *asioDriver = NULL;
ASIODriverList *asioList = new ASIODriverList();
ASIOConvert *convert = new ASIOConvert();
int driverId = -1;
struct ChannelCount {
	long in;
	long out;
} channelCount;
struct BufferSize {
	long min;
	long max;
	long preferred;
	long granularity;
} bufferSizes;
struct LatencySize {
	long in;
	long out;
} latencySizes;
long inputBuffers = 0;
long outputBuffers = 0;
bool postOutput = false;
ASIODriverInfo driverInfo;
ASIOSampleRate sampleRate = 44100.0;
ASIOBufferInfo bufferInfos[4];
ASIOChannelInfo channelInfos[4];
ASIOCallbacks asioCallbacks;

// callback prototypes
static void bufferSwitch(long index, ASIOBool processNow);
static ASIOTime *bufferSwitchTimeInfo(ASIOTime *timeInfo, long index, ASIOBool processNow);
static void sampleRateChanged(ASIOSampleRate sRate);
static long asioMessages(long selector, long value, void* message, double* opt);

void unloadAsio(){
	if (asioDriver != NULL){
		asioDriver->stop();
		asioDriver->disposeBuffers();
		asioList->closeDriver(driverId);
	}
}
bool loadAsio(int id){
	int i = 0;
	ASIOError result;
	ASIOBufferInfo *binf = bufferInfos;
	channelCount.in = 0;
	channelCount.out = 0;
	bufferSizes.min = 0;
	bufferSizes.max = 0;
	bufferSizes.preferred = 0;
	bufferSizes.granularity = 0;
	sampleRate = 44100.0;
	postOutput = false;
	inputBuffers = 0;
	outputBuffers = 0;
	if (asioList->openDriver(id, (void **)&asioDriver) == 0){
		driverId = id;
		if(asioDriver->init(&driverInfo.sysRef)){	
			asioDriver->getDriverName(driverInfo.name);
			driverInfo.driverVersion = asioDriver->getDriverVersion();
			if (asioDriver->getChannels(&channelCount.in, &channelCount.out) == ASE_OK){
				if (asioDriver->getBufferSize(&bufferSizes.min, &bufferSizes.max, &bufferSizes.preferred, &bufferSizes.granularity) == ASE_OK){
					if (asioDriver->getSampleRate(&sampleRate) == ASE_OK){
						if (sampleRate <= 0.0 || sampleRate > 96000.0) {
							if(asioDriver->setSampleRate(44100.0) != ASE_OK) {
								return false;
							} else if(asioDriver->getSampleRate(&sampleRate) != ASE_OK){
								return false;
							}
						}
						engine->setSampleRate((float)sampleRate);
						engine->setBlockSize((int)bufferSizes.preferred);
						if(asioDriver->outputReady() == ASE_OK){
							postOutput = true;
						} else {
							postOutput = false;
						}
						asioCallbacks.bufferSwitch = &bufferSwitch;
						asioCallbacks.sampleRateDidChange = &sampleRateChanged;
						asioCallbacks.asioMessage = &asioMessages;
						asioCallbacks.bufferSwitchTimeInfo = &bufferSwitchTimeInfo;
						if (channelCount.in > 2){
							inputBuffers = 2;
						} else {
							inputBuffers = channelCount.in;
						}
						for(i = 0; i < inputBuffers; i++, binf++)
						{
							binf->isInput = ASIOTrue;
							binf->channelNum = i;
							binf->buffers[0] = binf->buffers[1] = 0;
						}
						if (channelCount.out > 2){
							outputBuffers = 2;
						} else {
							outputBuffers = channelCount.out;
						}
						for(i = 0; i < outputBuffers; i++, binf++)
						{
							binf->isInput = ASIOFalse;
							binf->channelNum = i;
							binf->buffers[0] = binf->buffers[1] = 0;
						}
						if (asioDriver->createBuffers(bufferInfos, inputBuffers + outputBuffers, bufferSizes.preferred, &asioCallbacks) == ASE_OK){
							for (i = 0; i < inputBuffers + outputBuffers; i++){
								channelInfos[i].channel = bufferInfos[i].channelNum;
								channelInfos[i].isInput = bufferInfos[i].isInput;
								result = asioDriver->getChannelInfo(&channelInfos[i]);
								if (result != ASE_OK){
									break;
								}
							}
							if (result == ASE_OK){
								if (asioDriver->getLatencies(&latencySizes.in, &latencySizes.out) == ASE_OK){
									if (asioDriver->start() == ASE_OK){
										return true;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return false;
}

#if NATIVE_INT64
	#define ASIO64toDouble(a)  (a)
#else
	const double twoRaisedTo32 = 4294967296.;
	#define ASIO64toDouble(a)  ((a).lo + (a).hi * twoRaisedTo32)
#endif

unsigned long get_sys_reference_time()
{	// get the system reference time
#if WINDOWS
	return timeGetTime();
#elif MAC
static const double twoRaisedTo32 = 4294967296.;
	UnsignedWide ys;
	Microseconds(&ys);
	double r = ((double)ys.hi * twoRaisedTo32 + (double)ys.lo);
	return (unsigned long)(r / 1000.);
#endif
}

ASIOTime *bufferSwitchTimeInfo(ASIOTime *timeInfo, long index, ASIOBool processNow){	
	int i = 0;
	// the actual processing callback.
	// Beware that this is normally in a seperate thread, hence be sure that you take care
	// about thread synchronization. This is omitted here for simplicity.
	//static long processedSamples = 0;

	

	// store the timeInfo for later use
	//tInfo = *timeInfo;

	// get the time stamp of the buffer, not necessary if no
	// synchronization to other media is required
	//if (timeInfo->timeInfo.flags & kSystemTimeValid)
	//	nanoSeconds = ASIO64toDouble(timeInfo->timeInfo.systemTime);
	//else
	//	nanoSeconds = 0;

	//if (timeInfo->timeInfo.flags & kSamplePositionValid)
	//	samples = ASIO64toDouble(timeInfo->timeInfo.samplePosition);
	//else
	//	samples = 0;

	//if (timeInfo->timeCode.flags & kTcValid)
	//	tcSamples = ASIO64toDouble(timeInfo->timeCode.timeCodeSamples);
	//else
	//	tcSamples = 0;

	// get the system reference time
	//sysRefTime = get_sys_reference_time();

	// buffer size in samples
	long buffSize = bufferSizes.preferred;
	float *preLBuff = (float *)malloc(sizeof(float) * buffSize);
	float *preRBuff = (float *)malloc(sizeof(float) * buffSize);

	// perform the processing
	for (i = 0; i < inputBuffers; i ++){
		// TODO: process inputs
	}
	if (engine->generate(preLBuff, preRBuff, buffSize)){
		for (i = 2; i < 3; i ++){
			switch (channelInfos[i].type){
				case ASIOSTInt16LSB:
					break;
				case ASIOSTInt24LSB:		// used for 20 bits as well
					break;
				case ASIOSTInt32LSB:
					convert->float32toInt32inPlace(preLBuff, buffSize);
					convert->float32toInt32inPlace(preRBuff, buffSize);
					memcpy(bufferInfos[2].buffers[index], preLBuff, sizeof(long) * buffSize);
					memcpy(bufferInfos[3].buffers[index], preRBuff, sizeof(long) * buffSize);
					break;
				case ASIOSTFloat32LSB:		// IEEE 754 32 bit float, as found on Intel x86 architecture
					break;
				case ASIOSTFloat64LSB: 		// IEEE 754 64 bit double float, as found on Intel x86 architecture
					break;

					// these are used for 32 bit data buffer, with different alignment of the data inside
					// 32 bit PCI bus systems can more easily used with these
				case ASIOSTInt32LSB16:		// 32 bit data with 18 bit alignment
				case ASIOSTInt32LSB18:		// 32 bit data with 18 bit alignment
				case ASIOSTInt32LSB20:		// 32 bit data with 20 bit alignment
				case ASIOSTInt32LSB24:		// 32 bit data with 24 bit alignment
					break;

				case ASIOSTInt16MSB:
					break;
				case ASIOSTInt24MSB:		// used for 20 bits as well
					break;
				case ASIOSTInt32MSB:
					break;
				case ASIOSTFloat32MSB:		// IEEE 754 32 bit float, as found on Intel x86 architecture
					break;
				case ASIOSTFloat64MSB: 		// IEEE 754 64 bit double float, as found on Intel x86 architecture
					break;

					// these are used for 32 bit data buffer, with different alignment of the data inside
					// 32 bit PCI bus systems can more easily used with these
				case ASIOSTInt32MSB16:		// 32 bit data with 18 bit alignment
				case ASIOSTInt32MSB18:		// 32 bit data with 18 bit alignment
				case ASIOSTInt32MSB20:		// 32 bit data with 20 bit alignment
				case ASIOSTInt32MSB24:		// 32 bit data with 24 bit alignment
					break;
			}
		}
	}

	free(preLBuff);
	free(preRBuff);

	// finally if the driver supports the ASIOOutputReady() optimization, do it here, all data are in place
	if (postOutput){
		asioDriver->outputReady();
	}

	return 0L;
}

//----------------------------------------------------------------------------------
void bufferSwitch(long index, ASIOBool processNow)
{	// the actual processing callback.
	// Beware that this is normally in a seperate thread, hence be sure that you take care
	// about thread synchronization. This is omitted here for simplicity.

	// as this is a "back door" into the bufferSwitchTimeInfo a timeInfo needs to be created
	// though it will only set the timeInfo.samplePosition and timeInfo.systemTime fields and the according flags
	ASIOTime  timeInfo;
	memset (&timeInfo, 0, sizeof (timeInfo));

	// get the time stamp of the buffer, not necessary if no
	// synchronization to other media is required
	if(asioDriver->getSamplePosition(&timeInfo.timeInfo.samplePosition, &timeInfo.timeInfo.systemTime) == ASE_OK)
		timeInfo.timeInfo.flags = kSystemTimeValid | kSamplePositionValid;

	bufferSwitchTimeInfo (&timeInfo, index, processNow);
}


//----------------------------------------------------------------------------------
void sampleRateChanged(ASIOSampleRate sRate)
{
	// do whatever you need to do if the sample rate changed
	// usually this only happens during external sync.
	// Audio processing is not stopped by the driver, actual sample rate
	// might not have even changed, maybe only the sample rate status of an
	// AES/EBU or S/PDIF digital input at the audio device.
	// You might have to update time/sample related conversion routines, etc.
	if (engine != NULL){
		engine->setSampleRate((float)sRate);
	}
}

//----------------------------------------------------------------------------------
long asioMessages(long selector, long value, void* message, double* opt)
{
	// currently the parameters "value", "message" and "opt" are not used.
	long ret = 0;
	switch(selector)
	{
		case kAsioSelectorSupported:
			if(value == kAsioResetRequest
			|| value == kAsioEngineVersion
			|| value == kAsioResyncRequest
			|| value == kAsioLatenciesChanged
			// the following three were added for ASIO 2.0, you don't necessarily have to support them
			|| value == kAsioSupportsTimeInfo
			|| value == kAsioSupportsTimeCode
			|| value == kAsioSupportsInputMonitor)
				ret = 1L;
			break;
		case kAsioResetRequest:
			// defer the task and perform the reset of the driver during the next "safe" situation
			// You cannot reset the driver right now, as this code is called from the driver.
			// Reset the driver is done by completely destruct is. I.e. ASIOStop(), ASIODisposeBuffers(), Destruction
			// Afterwards you initialize the driver again.
			{
				int tmpdriverId = driverId;
				unloadAsio();
				loadAsio(tmpdriverId);
			}
			ret = 1L;
			break;
		case kAsioResyncRequest:
			// This informs the application, that the driver encountered some non fatal data loss.
			// It is used for synchronization purposes of different media.
			// Added mainly to work around the Win16Mutex problems in Windows 95/98 with the
			// Windows Multimedia system, which could loose data because the Mutex was hold too long
			// by another thread.
			// However a driver can issue it in other situations, too.
			ret = 1L;
			break;
		case kAsioLatenciesChanged:
			// This will inform the host application that the drivers were latencies changed.
			// Beware, it this does not mean that the buffer sizes have changed!
			// You might need to update internal delay data.
			ret = 1L;
			break;
		case kAsioEngineVersion:
			// return the supported ASIO version of the host application
			// If a host applications does not implement this selector, ASIO 1.0 is assumed
			// by the driver
			ret = 2L;
			break;
		case kAsioSupportsTimeInfo:
			// informs the driver wether the asioCallbacks.bufferSwitchTimeInfo() callback
			// is supported.
			// For compatibility with ASIO 1.0 drivers the host application should always support
			// the "old" bufferSwitch method, too.
			ret = 1;
			break;
		case kAsioSupportsTimeCode:
			// informs the driver wether application is interested in time code info.
			// If an application does not need to know about time code, the driver has less work
			// to do.
			ret = 0;
			break;
	}
	return ret;
}