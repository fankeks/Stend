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
		Serial();

		int open(int baud, std::string comport);

		int read();

		int write();

		int close();

		~Serial();
	};

	int serial_process(double time, double fps, std::string path);
}