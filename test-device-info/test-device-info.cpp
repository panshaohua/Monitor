// test-device-info.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "device_info.h"

int main()
{
	DeviceInfo *device_info = new DeviceInfo();

	while (true)
	{
		device_info->GetCpuInfo();
		Sleep(1000);

	}

    std::cout << "Hello World!\n";
}
