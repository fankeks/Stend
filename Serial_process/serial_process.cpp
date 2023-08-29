#include "serial_process.h"

namespace sp
{
	Serial::Serial()
	{

	}

	int Serial::open(int baud, std::string comport)
	{
		//Открытие порта
		connectedPort = CreateFileA
		(
			("\\\\.\\" + comport).c_str(),
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (connectedPort == INVALID_HANDLE_VALUE)
		{
			return -4; //Устройство не подключено
		}

		//Настройка параметров
		serialParams.DCBlength = sizeof(serialParams);
		if (!GetCommState(connectedPort, &serialParams))
		{
			return -3; //Не получены параметры
		}
		serialParams.BaudRate = baud;
		serialParams.ByteSize = 8;
		serialParams.StopBits = ONESTOPBIT;
		serialParams.Parity = NOPARITY;
		if (!SetCommState(connectedPort, &serialParams))
		{
			return -2; //Не удалось установить параметры
		}

		//Настройка таймаута
		serialTimeouts.ReadIntervalTimeout = 1;
		serialTimeouts.ReadTotalTimeoutConstant = 1;
		serialTimeouts.ReadTotalTimeoutMultiplier = 1;
		serialTimeouts.WriteTotalTimeoutConstant = 1;
		serialTimeouts.WriteTotalTimeoutMultiplier = 1;
		if (!SetCommTimeouts(connectedPort, &serialTimeouts))
		{
			return -1; //Не удалось настроить таймаут
		}

		//Завершение настройки
		return 0;
	}

	int Serial::read()
	{
		return 0;
	}

	int Serial::write()
	{
		return 0;
	}

	int Serial::close()
	{
		CloseHandle(connectedPort);
		return 0;
	}

	Serial::~Serial()
	{

	}

	int serial_process(double time, double fps, std::string path)
	{
		return 0;
	}
}