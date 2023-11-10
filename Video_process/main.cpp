#include <iostream>
#include "video_process_webcam.h"
#include <chrono>

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
	bool event = true;

	//Ввод параметров регистрации
	std::cout << "Введите время регистрации: ";
	cin >> time;
	std::cout << endl << "Введите fps: ";
	cin >> fps;
	std::cout << endl;

	//Создание директории с данными
	path = generate_dir_data(path);
	//Запуск процесса регистрации
	vpw::video_process(time, fps, event, path);

	//event = true;

	system("pause");
	return 0;
}

//int main(int argc, char* argv[])
//{
//	setlocale(LC_ALL, "ru");
//
//	cv::Mat img = cv::Mat(1080, 1920, CV_8UC3);
//	uchar* data;
//	data = new(uchar[1920 * 1080 * 3]);
//	for (int i = 0; i < 1920 * 1080 * 3; i++)
//	{
//		data[i] = 255;
//	}
//	img.data = data;
//	cv::imshow("img", img);
//	cv::waitKey(0);
//
//	auto start_time = std::chrono::steady_clock::now();
//	cv::imwrite("1.bmp", img);
//	auto end_time = std::chrono::steady_clock::now();
//	auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
//	std::cout << elapsed_ns.count() << " ns\n";
//	delete[] data;
//	system("pause");
//	return 0;
//}