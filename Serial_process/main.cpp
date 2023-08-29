#include "serial_process.h"
#include <iostream>

using namespace std;

void printconnect(int f)
{
	if (f == -4)
	{
		cout << "Устройство не подключено" << endl;
	}
	if (f == -3)
	{
		cout << "Параметры не приняты" << endl;
	}
	if (f == -2)
	{
		cout << "Параметры не установлены" << endl;
	}
	if (f == -1)
	{
		cout << "Параметры таймаута не установлены" << endl;
	}
}

int main(int args, char* argv[])
{
	setlocale(LC_ALL, "ru");
	sp::Serial arduino;
	int f = arduino.open(115200, "COM3");
	printconnect(f);
	string s;
	//cout << (int)'\r' << endl;
	
	while (1)
	{
		arduino.read(s);
		cout << s << endl;
	}
	
	arduino.close();
	system("pause");
	return 0;
}