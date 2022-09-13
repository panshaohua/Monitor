#ifndef SERIAL_H
#define SERIAL_H

#include <vector>
#include <string>
#include <windows.h>

#include < vcclr.h >

class DeviceInfo {
public:
	DeviceInfo();
	~DeviceInfo();

	int GetCpuInfo();

private:
	void* m_pComputeClr;
};

#endif