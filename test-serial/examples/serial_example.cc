/***
 * This example expects the serial port has a loopback on it.
 *
 * Alternatively, you could use an Arduino:
 *
 * <pre>
 *  void setup() {
 *    Serial.begin(<insert your baudrate here>);
 *  }
 *
 *  void loop() {
 *    if (Serial.available()) {
 *      Serial.write(Serial.read());
 *    }
 *  }
 * </pre>
 */

#include <string>
#include <iostream>
#include <cstdio>

// OS Specific sleep
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "serial/serial.h"

using std::string;
using std::exception;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

void my_sleep(unsigned long milliseconds) {
#ifdef _WIN32
      Sleep(milliseconds); // 100 ms
#else
      usleep(milliseconds*1000); // 100 ms
#endif
}

void enumerate_ports()
{
	vector<serial::PortInfo> devices_found = serial::list_ports();

	vector<serial::PortInfo>::iterator iter = devices_found.begin();

	while( iter != devices_found.end() )
	{
		serial::PortInfo device = *iter++;

		printf( "(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(),
     device.hardware_id.c_str() );
	}
}

void print_usage()
{
	cerr << "Usage: test_serial {-e|<serial port address>} ";
    cerr << "<baudrate> [test string]" << endl;
}

int run(int argc, char **argv)
{
  if(argc < 2) {
	  print_usage();
    return 0;
  }

  // Argument 1 is the serial port or enumerate flag
  string port(argv[1]);

  if( port == "-e" ) {
	  enumerate_ports();
	  return 0;
  }
  else if( argc < 3 ) {
	  print_usage();
	  return 1;
  }

  // Argument 2 is the baudrate
  unsigned long baud = 0;
#if defined(WIN32) && !defined(__MINGW32__)
  sscanf_s(argv[2], "%lu", &baud);
#else
  sscanf(argv[2], "%lu", &baud);
#endif

  // port, baudrate, timeout in milliseconds
  serial::Serial my_serial(port, baud, serial::Timeout::simpleTimeout(1000));

  cout << "Is the serial port open?";
  if(my_serial.isOpen())
    cout << " Yes." << endl;
  else
    cout << " No." << endl;

  // Get the Test string
  int count = 0;
  string test_string;
  if (argc == 4) {
    test_string = argv[3];
  } else {
    test_string = "Testing.";
  }

  // Test the timeout, there should be 1 second between prints
  cout << "Timeout == 1000ms, asking for 1 more byte than written." << endl;
  while (count < 10) {
    size_t bytes_wrote = my_serial.write(test_string);

    string result = my_serial.read(test_string.length()+1);

    cout << "Iteration: " << count << ", Bytes written: ";
    cout << bytes_wrote << ", Bytes read: ";
    cout << result.length() << ", String read: " << result << endl;

    count += 1;
  }

  // Test the timeout at 250ms
  my_serial.setTimeout(serial::Timeout::max(), 250, 0, 250, 0);
  count = 0;
  cout << "Timeout == 250ms, asking for 1 more byte than written." << endl;
  while (count < 10) {
    size_t bytes_wrote = my_serial.write(test_string);

    string result = my_serial.read(test_string.length()+1);

    cout << "Iteration: " << count << ", Bytes written: ";
    cout << bytes_wrote << ", Bytes read: ";
    cout << result.length() << ", String read: " << result << endl;

    count += 1;
  }

  // Test the timeout at 250ms, but asking exactly for what was written
  count = 0;
  cout << "Timeout == 250ms, asking for exactly what was written." << endl;
  while (count < 10) {
    size_t bytes_wrote = my_serial.write(test_string);

    string result = my_serial.read(test_string.length());

    cout << "Iteration: " << count << ", Bytes written: ";
    cout << bytes_wrote << ", Bytes read: ";
    cout << result.length() << ", String read: " << result << endl;

    count += 1;
  }

  // Test the timeout at 250ms, but asking for 1 less than what was written
  count = 0;
  cout << "Timeout == 250ms, asking for 1 less than was written." << endl;
  while (count < 10) {
    size_t bytes_wrote = my_serial.write(test_string);

    string result = my_serial.read(test_string.length()-1);

    cout << "Iteration: " << count << ", Bytes written: ";
    cout << bytes_wrote << ", Bytes read: ";
    cout << result.length() << ", String read: " << result << endl;

    count += 1;
  }

  while (1) {
	  size_t bytes_wrote = my_serial.write(test_string);

	  string result = my_serial.read(test_string.length() - 1);

	  cout << "Iteration: " << count << ", Bytes written: ";
	  cout << bytes_wrote << ", Bytes read: ";
	  cout << result.length() << ", String read: " << result << endl;

	  count += 1;
  }

  return 0;
}

int main(int argc, char **argv) {
  try {
    return run(argc, argv);
  } catch (exception &e) {
    cerr << "Unhandled Exception: " << e.what() << endl;
  }
}

unsigned int modbusCRC16(const vector<unsigned char> &data)
{
	unsigned short CRC = 0xffff;//（1）CRC寄存器初值0xffff

	int dataSize = data.size();
	for (int i = 0; i < dataSize; i++)//（5）重复步骤2~4
	{
		CRC = CRC ^ data[i];//(2)数据与CRC异或
		for (int j = 0; j < 8; j++)//（4）重复8次步骤3
		{
			//(3)检测低位是否为1。方法：与1相与，低位为1则结果为1，低位为0则结果为0
			if (CRC & 1)//如果低位为1，则先右移一位，再与A001H相异或
			{
				CRC >>= 1;
				CRC ^= 0xA001;
			}
			else//低位为0，则右移一位
				CRC >>= 1;
		}
	}
	return CRC;
}
int main1()
{
	vector<unsigned char> vec{ 0x55, 0xAA, 0x02, 0x91, 0x00 };

	unsigned int CRC = modbusCRC16(vec);
	printf("%X", CRC);
	return 0;
}
