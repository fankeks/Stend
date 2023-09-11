#include "xiApiPlusOcv.hpp"
#include <iostream>
#include <time.h>
#include <string>
#include "Video_process_cam.h"


using namespace cv;
using namespace std;


std::string generate_dir_data(std::string path)
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
	try
	{
		setlocale(LC_ALL, "ru");
		string name;
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

		long EXPECTED_IMAGES = 200 * 10;

		xiAPIplusCameraOcv cam;
		cout << "Opening first camera..." << endl;
		cam.OpenFirst();
		int ex = 1000000 / fps;
		cam.SetExposureTime(ex); //10000 us = 10 ms
		cam.SetImageDataFormat(XI_RGB24);

		cout << "Starting acquisition..." << endl;
		cam.StartAcquisition();
		//Sleep(3);

		clock_t tStart = clock();
		for (int images = 0; images < EXPECTED_IMAGES; images++)
		{
			Mat cv_mat_image = cam.GetNextImageOcvMat();
			name = path + std::to_string(images) + ".bmp";
			imwrite(name, cv_mat_image);
		}
		clock_t tEnd = clock();
		printf("Time taken: %.2fs\n", (double)(tEnd - tStart) / CLOCKS_PER_SEC);


		cam.StopAcquisition();
		cam.Close();
		cout << "Done" << endl;
		waitKey(0);
	}
	catch (xiAPIplus_Exception& exp)
	{
		cout << "Error:" << endl;
		exp.PrintError();
		Sleep(3000);
		return -1;
	}
	return 0;
}
