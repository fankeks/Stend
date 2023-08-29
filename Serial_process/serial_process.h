#pragma once
#include <iostream>
#include <string>
#include <Windows.h>

namespace sp
{
	class Serial
	{
	private:
		HANDLE connectedPort;
		DCB serialParams;
		COMMTIMEOUTS serialTimeouts;

	public:
		bool isConnect = false;

		Serial();

		int open(int baud, std::string comport);

		int read(std::string &message);

		int write(std::string message);

		int close();

		~Serial();
	};

	int serial_process(double time, double fps, std::string path);
}