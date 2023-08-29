#include "video_process_webcam.h"

namespace vpw
{
	std::mutex mx;

	std::string generate_dir_webcam(std::string path)
	{
		path = path + "webcam";
		_mkdir(path.c_str());
		return path + char(92);
	}

	int save_data(std::list<cv::Mat> &buffer_img, std::list<long> &buffer_num, bool &f, std::string path)
	{
		std::string name;
		long i = 0;
		cv::Mat frame;
		long num;
		std::string path_img;
		std::string path_num;
		std::ofstream file;

		//Создание необходимых файлов
		path_num = path + "Number.txt";
		file.open(path_num);
		path_img = path + "Pictures" + char(92);
		//std::cout << path << endl;
		_mkdir(path_img.c_str());

		//Начало регистрации
		//std::cout << "Начало сохранения" << std::endl;
		while (1)
		{
			//Если в буфере достаточно эллементов, то сохранить элементы
			if (buffer_img.size() > 3)
			{
				//Получение 1-ого элемента очереди
				mx.lock();
				frame = *buffer_img.begin();
				buffer_img.pop_front();
				num = *buffer_num.begin();
				buffer_num.pop_front();
				mx.unlock();
				//Сохранение данных
				i++;
				name = path_img + std::to_string(i) + ".jpg";
				cv::imwrite(name, frame);
				file << num << "\n";
				//Продолжение сохранения
				continue;
			}
			//cout << "Ожидание кадров" << endl;
			//Проверка на завершение регистрации
			if (f == false)
			{
				break;
			}
		}
		while (!buffer_img.empty())
		{
			mx.lock();
			frame = *buffer_img.begin();
			buffer_img.pop_front();
			num = *buffer_num.begin();
			buffer_num.pop_front();
			mx.unlock();
			//Сохранение данных
			i++;
			name = path_img + std::to_string(i) + ".jpg";
			cv::imwrite(name, frame);
			file << num << "\n";
		}

		file.close();
		//std::cout << "Сохранение завершено" << std::endl;
		return 0;
	}

	int video_process(double time, double fps, bool &event, std::string path)
	{
		cv::Mat frame; //Объект картинки
		cv::VideoCapture cap(0); //Объект камеры
		std::list<cv::Mat> buffer_img; //Буфер сохранения картинок
		std::list<long> buffer_num; //Буфер сохранения номера
		bool f = true; //Флаг сохранения (True запись идёт, False запись прекращена)
		long n = (long)(time * fps); //Колличество кадров
		clock_t begin;
		clock_t end;

		//Создание папки для сохранения
		path = generate_dir_webcam(path);

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		//Создание и запуск потока сохранения
		std::thread saving(save_data, std::ref(buffer_img), std::ref(buffer_num), std::ref(f), path);

		//Включение камеры
		if (!cap.isOpened()) {
			std::cout << "Камера не включена" << std::endl;
			return -1;
		}

		while (!event);

		//Регистрация данных
		//std::cout << "Старт регистрации" << std::endl;
		begin = clock();
		for (long i = 0; i < n; i++)
		{
			cap >> frame;
			buffer_img.push_back(frame);
			buffer_num.push_back(i);
		}
		end = clock();
		//std::cout << "Регистрация завершена" << std::endl;
		double time_spent = ((double)(end - begin) / CLOCKS_PER_SEC);
		//std::cout << "Время регистрации: " << time_spent << std::endl;

		//Завершение программы
		cap.release();
		f = false;
		saving.join();
		std::cout << "Webcam ready" << std::endl;
		return 0;
	}
}