#include "device_info.h"

#include <iostream>

using namespace OpenHardwareMonitor::Hardware;

// https://blog.csdn.net/ganzheyu/article/details/50154705 托管与非托管的混合编程

DeviceInfo::DeviceInfo() {

	Computer ^computer = gcnew Computer();

	computer->CPUEnabled = true;
	computer->FanControllerEnabled = true;
	computer->MainboardEnabled = true;
	computer->RAMEnabled = true;
	computer->GPUEnabled = true;
	computer->HDDEnabled = true;

	computer->Open();

	gcroot<Computer^> *pp = new gcroot<Computer^>(computer);
	this->m_pComputeClr = static_cast<void*>(pp);
}

DeviceInfo::~DeviceInfo() {

	if (this->m_pComputeClr)
	{
		// Get the CLR handle wrapper
		gcroot<Computer^> *pp = static_cast<gcroot<Computer^>*>(this->m_pComputeClr);

		((Computer^)*pp)->Close();

		// Delete the wrapper; this will release the underlying CLR instance
		delete pp;
		// Set to null
		this->m_pComputeClr = 0;
	}
}

int DeviceInfo::GetCpuInfo() {

	if (this->m_pComputeClr) {

		// Get the CLR handle wrapper
		gcroot<Computer^> *pp = static_cast<gcroot<Computer^>*>(this->m_pComputeClr);

		for (int i = 0; i < ((Computer^)*pp)->Hardware->Length; i++) {

			auto hardware = ((Computer^)*pp)->Hardware[i];

			if (HardwareType::CPU == hardware->HardwareType) {

				hardware->Update();

				for (int j = 0; j < hardware->Sensors->Length; j++) {

					if (SensorType::Temperature == hardware->Sensors[j]->SensorType) {
						auto name = hardware->Sensors[j]->Name;
						auto value = hardware->Sensors[j]->Value;
						printf("Temperature ---- %s\t%f\n", name, value.Value);
					}

					else if (SensorType::Load == hardware->Sensors[j]->SensorType) {

						auto name = hardware->Sensors[j]->Name;
						auto value = hardware->Sensors[j]->Value;
						printf("Utilization ---- %s\t%f\n", name, value.Value);
					}
					else {

					}
				}
				std::cout << std::endl;
			}
		}
	}

	return 0;
}