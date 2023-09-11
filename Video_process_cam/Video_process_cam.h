#pragma once
#include <fstream>
#include <list>
#include <mutex>
#include <string>
#include <thread>
#include <direct.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>


namespace vpc
{
	int video_process(double time, double fps, bool& event, std::string path);
}