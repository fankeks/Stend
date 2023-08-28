#include <iostream>
#include "video_process_webcam.h"

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
		std::cout << "Data created" << endl;
	}
	return path1 + char(92);
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru");

	//Инициализация переменных
	string path = argv[0]; //Путь к файлу
	double time, fps; //Время и fps
	bool event = false;

	//Ввод параметров регистрации
	std::cout << "Введите время регистрации: ";
	cin >> time;
	std::cout << endl << "Введите fps: ";
	cin >> fps;
	std::cout << endl;

	//Создание директории с данными
	path = generate_dir_data(path);
	//Запуск процесса регистрации
	vpw::video_process(time, fps, ref(event), path);

	system("pause");
	return 0;
}