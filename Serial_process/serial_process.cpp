#include "serial_process.h"

namespace sp
{
	Serial::Serial()
	{

	}

	int Serial::open(int baud, std::string comport)
	{
		//�������� �����
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
			return -4; //���������� �� ����������
		}

		//��������� ����������
		serialParams.DCBlength = sizeof(serialParams);
		if (!GetCommState(connectedPort, &serialParams))
		{
			return -3; //�� �������� ���������
		}
		serialParams.BaudRate = baud;
		serialParams.ByteSize = 8;
		serialParams.StopBits = ONESTOPBIT;
		serialParams.Parity = NOPARITY;
		if (!SetCommState(connectedPort, &serialParams))
		{
			return -2; //�� ������� ���������� ���������
		}

		//��������� ��������
		serialTimeouts.ReadIntervalTimeout = 1;
		serialTimeouts.ReadTotalTimeoutConstant = 1;
		serialTimeouts.ReadTotalTimeoutMultiplier = 1;
		serialTimeouts.WriteTotalTimeoutConstant = 1;
		serialTimeouts.WriteTotalTimeoutMultiplier = 1;
		if (!SetCommTimeouts(connectedPort, &serialTimeouts))
		{
			return -1; //�� ������� ��������� �������
		}

		//���������� ���������
		isConnect = true;
		return 0;
	}

	int Serial::read(std::string& message)
	{
		//�������� �� ����������� �����
		if (!isConnect)
		{
			return -2; //���� �� ���������
		}
		if (!SetCommMask(connectedPort, EV_RXCHAR))
		{
			return -1; //�������� ��� � ������
		}
		
		//������ ������
		DWORD iSize;
		char s_now, s_prev;
		message = "";
		do
		{
			if (ReadFile(connectedPort, &s_now, 1, &iSize, NULL)) //�������� 1 ����
			{
				if (iSize > 0)
				{
					if (s_now == '\n')
					{
						if (s_prev == '\r')
						{
							break;
						}
						s_prev = s_now;
						message += s_now;
					}
					else if (s_now == '\r')
					{
						s_prev = s_now;
					}
					else
					{
						s_prev = s_now;
						message += s_now;
					}
				}	
			}
		} while (1);
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