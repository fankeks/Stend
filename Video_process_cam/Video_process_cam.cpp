#include "Video_process_cam.h"
#include "xiApiPlusOcv.hpp"

namespace vpc
{
	std::mutex mx;

	std::string generate_dir_cam(std::string path)
	{
		path = path + "Cam";
		_mkdir(path.c_str());
		return path + char(92);
	}

	int save_data(xiAPIplusCameraOcv& cam, long& buffer_num, long& buffer_counter, std::string path)
	{
		cv::Mat frame;
		std::string name;
		long i = 0;
		long num;
		std::string path_img;

		path_img = path + "Pictures" + char(92);
		//std::cout << path << endl;
		_mkdir(path_img.c_str());

		//Начало регистрации
		//std::cout << "Начало сохранения" << std::endl;
		while(buffer_counter < buffer_num)
		{
			mx.lock();
			frame = cam.GetNextImageOcvMat();
			name = path_img + std::to_string(buffer_counter) + ".bmp";
			buffer_counter++;
			mx.unlock();
			//Сохранение данных

			cv::imwrite(name, frame);
			frame.release();
			//Продолжение сохранения
		}
		return 0;
	}

	int video_process(double time, double fps, bool& event, std::string path)
	{
		try
		{
			xiAPIplusCameraOcv cam;
			//std::cout << "Opening first camera..." << std::endl;
			cam.OpenFirst();
			long ex = 1000000 / fps;
			cam.SetExposureTime(ex); //10000 us = 10 ms
			cam.SetImageDataFormat(XI_RGB24);
			cam.SetAcquisitionBufferSizeBytesUnit(1024*1024);
			cam.SetAcquisitionBufferSizeBytes(10 * 1024);
			cam.SetAcquisitionQueueImagesCount(1500);

			/*
			cv::Mat frame; //Îáúåêò êàðòèíêè
			cv::VideoCapture cap(0); //Îáúåêò êàìåðû
			*/
			std::string name;
			std::string path_img;
			cv::Mat frame;
			long n = (long)(time * fps); //Êîëëè÷åñòâî êàäðîâ
			long buffer_counter = 0;
			clock_t begin;
			clock_t end;

			//Ñîçäàíèå ïàïêè äëÿ ñîõðàíåíèÿ
			path = generate_dir_cam(path);

			path_img = path + "Pictures" + char(92);

			//Создание необходимых файлов
			//std::cout << path << endl;
			_mkdir(path_img.c_str());

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			//Ñîçäàíèå è çàïóñê ïîòîêà ñîõðàíåíèÿ

			
			while (!event);

			//Ðåãèñòðàöèÿ äàííûõ
			//std::cout << "Ñòàðò ðåãèñòðàöèè" << std::endl;

			//cout << "Starting acquisition..." << endl;
			begin = clock();
			cam.StartAcquisition();

			//Создание и запуск потока сохранения
			std::thread saving1(save_data, std::ref(cam), std::ref(n), std::ref(buffer_counter), path);
			//std::thread saving2(save_data, std::ref(cam), std::ref(n), std::ref(buffer_counter), std::ref(f), path);
			save_data(cam, n, buffer_counter, path);

			
			//Çàâåðøåíèå ïðîãðàììû
			saving1.join();
			//saving2.join();
			cam.StopAcquisition();
			end = clock();
			//std::cout << "Регистрация завершена" << std::endl;
			double time_spent = ((double)(end - begin) / CLOCKS_PER_SEC);
			//std::cout << "Время регистрации: " << time_spent << std::endl;
			cam.Close();
			std::cout << "Cam ready" << std::endl;
		}
		catch (xiAPIplus_Exception& exp)
		{
			std::cout << "Error:" << std::endl;
			exp.PrintError();
			Sleep(3000);
			return -1;
		}
		return 0;
	}

}