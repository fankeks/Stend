#include "serial_process.h"
#include <iostream>
#include <thread>

using namespace std;

void printconnect(int f)
{
	if (f == -4)
	{
		cout << "���������� �� ����������" << endl;
	}
	if (f == -3)
	{
		cout << "��������� �� �������" << endl;
	}
	if (f == -2)
	{
		cout << "��������� �� �����������" << endl;
	}
	if (f == -1)
	{
		cout << "��������� �������� �� �����������" << endl;
	}
}

int main(int args, char* argv[])
{
	setlocale(LC_ALL, "ru");
	sp::Serial arduino;
	int f = arduino.open(115200, "COM3");
	printconnect(f);
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	string message = "30";
	if (arduino.write(message) == 0)
	{
		cout << "��������� ����������" << endl;
	}

	string s;
	arduino.read(s);
	cout << s;
	arduino.close();
	system("pause");
	return 0;
}