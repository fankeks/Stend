#include "serial_process.h"
#include <iostream>

using namespace std;

int main(int args, char* argv[])
{
	setlocale(LC_ALL, "ru");
	sp::serial_process(2.0, 2.0, "stv");
	system("pause");
	return 0;
}