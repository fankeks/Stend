#include <iostream>
#include <thread>
#include <direct.h>
#include <fstream>
#include <string>
#include <Windows.h>
#include "../Video_process/video_process_webcam.h"

using namespace std;

string generate_dir_data(string path)
{
	string it_s = "";
	string path1 = "";
	int ind = 0;
	for (int i = 0; i < path.size(); i++)
	{
		it_s += path[i];
		if ((int)path[i] == 92)
		{
			path1 += it_s;
			it_s = "";
		}
	}
	path1 += "Data";

	if (!_mkdir(path1.c_str()))
	{
		ofstream file;
		file.open(path1 + char(92) + "Name.txt");
		file << '1';
		file.close();
		file.open(path1 + char(92) + "Log.txt");
		file.close();
	}
	else
	{
		ifstream file;
		file.open(path1 + char(92) + "Name.txt");
		if (!file.is_open())
		{
			ofstream file;
			file.open(path1 + char(92) + "Name.txt");
			file << '1';
		}
		else
		{
			char ch;
			if (!file.get(ch))
			{
				file.close();
				ofstream file;
				file.open(path1 + char(92) + "Name.txt");
				file << '1';
				file.close();
			}
		}
		file.close();

		file.open(path1 + char(92) + "Log.txt");
		if (!file.is_open())
		{
			ofstream file;
			file.open(path1 + char(92) + "Log.txt");
		}
		file.close();
	}
	return path1 + char(92);
}

string generate_dir(string path)
{
	ifstream file;
	file.open(path + "Name.txt");
	char ch;
	string name;
	while (file.get(ch))
	{
		name += ch;
	}
	file.close();

	int a = stoi(name) + 1;
	ofstream file1;
	file1.open(path + "Name.txt");
	file1 << a;
	file1.close();

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	string log;
	cout << "Введите название эксперимента: ";
	cin >> log;
	cout << endl;
	file1.open(path + "Log.txt", ofstream::app);
	file1 << path + name + "\t" + log + "\n";

	path += name;
	_mkdir(path.c_str());
	return path + char(92);
}

int main(int args, char* argv[])
{
	setlocale(LC_ALL, "ru");

	//Создание директорий
	string path = argv[0];
	path = generate_dir_data(path);
	path = generate_dir(path);

	//Ввод параметров регистрации
	double time, fps;
	cout << "Введите время регистрации: ";
	cin >> time;
	cout << endl << "Введите fps: ";
	cin >> fps;
	cout << endl;

	//Запуск процессов регистрации
	std::thread webcam(vpw::video_process, time, fps, path);

	webcam.join();
	system("pause");
	return 0;
}