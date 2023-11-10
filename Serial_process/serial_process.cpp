#include "serial_process.h"

std::string generate_dir_arduino(std::string path)
{
	path = path + "arduino";
	_mkdir(path.c_str());
	return path + char(92);
}

namespace sp
{
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
		isConnect = true;
		return 0;
	}

	int Serial::read(std::string& message)
	{
		//Проверка на подключение порта
		if (!isConnect)
		{
			return -2; //Порт не подключен
		}
		if (!SetCommMask(connectedPort, EV_RXCHAR))
		{
			return -1; //Символов нет в буфере
		}
		
		//Данные пришли
		DWORD iSize;
		char s_now, s_prev;
		message = "";
		do
		{
			if (ReadFile(connectedPort, &s_now, 1, &iSize, NULL)) //Получаем 1 байт
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

	int Serial::write(std::string message)
	{
		if (!isConnect)
		{
			return -2; //Порт не подключен
		}
		
		//Преобразование строки в массив char
		int length = message.length();
		const char* mes = message.c_str();

		//Передача сообщения
		DWORD dwSize = length;
		DWORD BytesIterated;
		if (!WriteFile(connectedPort, mes, dwSize, &BytesIterated, NULL))
		{
			return -1; //Сообщение не отправлено
		}
		return 0;
	}

	int Serial::close()
	{
		CloseHandle(connectedPort);
		return 0;
	}

	Serial::~Serial()
	{
		CloseHandle(connectedPort);
	}

	std::string choose_port()
	{
		Serial connect;
		std::list<std::string> ports;
		std::string port;
		for (int i = 0; i < 50; i++)
		{
			port = "COM" + std::to_string(i);
			if (connect.open(115200, port) == 0)
			{
				ports.push_back(port);
			}
			connect.close();
		}
		if (ports.size() > 1)
		{
			int n = 0;
			for (auto i = ports.begin(); i != ports.end(); i++)
			{
				n++;
				std::cout << "Выберите порт" << std::endl;
				std::cout << *i << " -- " << n << std::endl;
			}
			std::cin >> n;
			n -= 1;
			for (int i = 0; i < n; i++)
			{
				port = *ports.begin();
				ports.pop_front();
			}
		}

		if (ports.size() == 1)
		{
			return *ports.begin();
		}

		std::cout << "Нет подключенных устройств" << std::endl;
		return "";
	}

	int serial_process(double time, double fps, bool &event, std::string path)
	{
		//Создание необходимых переменных
		path = generate_dir_arduino(path);
		std::ofstream FPG1;
		FPG1.open(path + "FPG1.txt");
		std::ofstream FPG2;
		FPG2.open(path + "FPG2.txt");
		std::ofstream ECG;
		ECG.open(path + "ECG.txt");
		std::ofstream Numbers;
		Numbers.open(path + "Numbers.txt");
		long num = (long)(fps * time);
		
		//Выбираем порт
		std::string port = choose_port();
		if (port == "")
		{
			return -5; //Устройство не подключено
		}
		
		//Подключаемся по выбраному порту
		Serial arduino;
		int f = arduino.open(115200, port);
		if (f != 0)
		{
			return f; //Неполадки с подключением
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		
		//Отправляем fps на arduino
		f = (int)fps;
		std::string message = std::to_string(f);
		arduino.write(message);
		event = true;
		std::cout << "Старт регистрации" << std::endl;

		//Регистрация данных
		for (long i = 0; i < num; i++)
		{
			arduino.read(message);
			FPG1 << message << "\n";
			arduino.read(message);
			FPG2 << message << "\n";
			arduino.read(message);
			ECG << message << "\n";
			Numbers << i << "\n";
		}

		//Закрытие файлов
		FPG1.close();
		FPG2.close();
		ECG.close();
		Numbers.close();
		std::cout << "Arduino ready" << std::endl;
		return 0;
	}
}