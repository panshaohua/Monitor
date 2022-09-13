#include <iostream>
#include <windows.h>

using namespace OpenHardwareMonitor::Hardware;


int main()
{
	Computer ^computer = gcnew Computer();

	computer->CPUEnabled = true;
	computer->FanControllerEnabled = true;
	computer->MainboardEnabled = true;
	computer->RAMEnabled = true;
	computer->GPUEnabled = true;	
	computer->HDDEnabled = true;

	computer->Open();

	while (true) {

		for (int i = 0; i < computer->Hardware->Length; i++) {

			auto hardware = computer->Hardware[i];

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

			else if (HardwareType::RAM == hardware->HardwareType) {

				hardware->Update();

				for (int j = 0; j < hardware->Sensors->Length; j++) {

					if (SensorType::Load == hardware->Sensors[j]->SensorType) {

						auto name = hardware->Sensors[j]->Name;
						auto value = hardware->Sensors[j]->Value;
						printf("Utilization ---- %s\t\t\t%f\n", name, value.Value);
					}
					else if (SensorType::Data == hardware->Sensors[j]->SensorType) {
						
						auto name = hardware->Sensors[j]->Name;
						auto value = hardware->Sensors[j]->Value;
						printf("%s\t%f\n", name, value.Value);
					}
				}

				std::cout << std::endl;
			}
			else if (HardwareType::GpuNvidia == hardware->HardwareType || HardwareType::GpuAti == hardware->HardwareType) {

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

						System::String ^name_("GPU Core");
						if (name->Contains(name_)) {
							printf("GPU utilization %s\t%f\n", name, value.Value);
						}
					}
					else if (SensorType::SmallData == hardware->Sensors[j]->SensorType) {

						auto name = hardware->Sensors[j]->Name;
						auto value = hardware->Sensors[j]->Value;
						printf("%s\t%f\n", name, value.Value);
					}
					else {


					}
				}
				std::cout << std::endl;
			}
			else if (HardwareType::TBalancer == hardware->HardwareType) {

				hardware->Update();

				for (int j = 0; j < hardware->Sensors->Length; j++) {

					std::cout << "111\n";
				}
			}
		}

		Sleep(1000);
	}
	computer->Close();

    std::cout << "Hello World!\n";
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
