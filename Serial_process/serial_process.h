#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <list>
#include <thread>
#include <direct.h>
#include <fstream>

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

		int open(int baud, std::string comport);

		int read(std::string &message);

		int write(std::string message);

		int close();

		~Serial();
	};

	std::string choose_port();

	int serial_process(double time, double fps, bool &event, std::string path);
}