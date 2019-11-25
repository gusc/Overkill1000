#ifndef __ASIO_DRIVER_LIST_H
#define __ASIO_DRIVER_LIST_H

#include <Windows.h>

#define DRVERR			-5000
#define DRVERR_INVALID_PARAM		DRVERR-1
#define DRVERR_DEVICE_ALREADY_OPEN	DRVERR-2
#define DRVERR_DEVICE_NOT_FOUND		DRVERR-3

#define MAXPATHLEN			512
#define MAXDRVNAMELEN		128

struct ASIODriver {
	int						drvID;
	CLSID					clsid;
	char					dllpath[MAXPATHLEN];
	char					drvname[MAXDRVNAMELEN];
	LPVOID				asiodrv;
	struct ASIODriver *next;
};

class ASIODriverList {
	// internal ASIO driver structure
	ASIODriver *lpdrvlist;
	int numdrv;

public:
	/// Create ASIO driver list
	ASIODriverList();
	/// Destroy ASIO driver list
	~ASIODriverList();
	/// Open ASIO driver
	/// @param int - driver index
	/// @param IASIO * - driver interface pointer
	/// @return long
	long openDriver (int, void **);
	/// Close ASIO driver
	/// @param int - driver index
	/// @return long
	long closeDriver (int);
	/// Get number of ASIO drivers
	/// @return long
	long getNumDev (void);
	/// Get ASIO driver name
	/// @param int - driver index
	/// @param char * - driver name buffer
	/// @param int - max length of driver name to copy
	/// @return long
	long getDriverName (int,char *,int);		
	/// Get ASIO driver path
	/// @param int - driver index
	/// @param char * - driver name buffer
	/// @param int - max length of driver name to copy
	/// @return long
	long getDriverPath (int,char *,int);
	/// Get ASIO driver CLSID
	/// @param int - driver index
	/// @param CLSID * - driver CLSID
	/// @return long
	long getDriverCLSID (int,CLSID *);
	
};

#endif
