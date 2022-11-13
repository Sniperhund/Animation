//
// Created by Lucas on 09/11/2022.
//

#include "Util.h"

#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>

const char *Util::ASCII_LIST = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
//const char *Util::ASCII_LIST = "BS#&@$%*!:.BS#&@$%*!:.BS#&@$%*!:.BS#&@$%*!:.BS#&@$%*!:.BS#&@$%*!:.*!:.";

void Util::InitConsole() {
#ifdef _WIN32
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = 18;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    std::wcscpy(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect);
    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 800 * 1.30, 600 * 1.30, TRUE);
#endif
}

void Util::ShowVideo(bool playAudio) {
    std::string result;

    try {
        std::ifstream file("ascii/" + std::to_string(0));
        if (file.is_open()) {
            std::cout << "Hello" << std::endl;
            std::string line;
            while (std::getline(file, line)) {
                result += line;
                result += "\n";
            }
            file.close();

            std::cout << result << std::endl;
        }
    } catch(const std::ifstream::failure& e) {
        std::cout << e.what() << std::endl;
    }
    std::cin.ignore();

    int i = 1;
    bool shouldContinue = true;

#ifdef _WIN32
    if (PlaySound("../music.wav", NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC) == false){
        std::cout << "Sound failed" << std::endl;
        return -1;
    }
#else
    if (playAudio) system("powershell.exe start ../music.wav");
#endif
    while (shouldContinue) {
        std::ifstream file("ascii/" + std::to_string(i));
        if (file.fail()) shouldContinue = false;
        if (file.is_open()) {
            for (int j = 0; j < 25; ++j) {
                result += "\n";
            }
            std::string line;
            while(std::getline(file, line)) {
                result += line;
                result += "\n";
            }
            file.close();
#ifdef _WIN32
            result += "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
#else
            system("clear");
#endif
            std::cout << result << std::endl;
        }

        i++;
        result = "";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 24));
    }
}

void Util::ConvertVideo(bool splitVideo) {
    std::cout << "Trying to convert video!" << std::endl;
    try {
        cv::VideoCapture capture("video.mp4");
        if (!capture.isOpened()) {
            CV_Error(CV_StsError, "Can not open Video file");
        }

        if (splitVideo) {
            std::cout << "Splitting video!" << std::endl;
            for (int frameNum = 0; frameNum < capture.get(cv::CAP_PROP_FRAME_COUNT); ++frameNum) {
                std::cout << "\r"
                          << std::to_string((int) std::round(frameNum / capture.get(cv::CAP_PROP_FRAME_COUNT) * 100))
                          << "%" << std::flush;
                ConvertFrame(capture, frameNum);
            }
        }
        std::cout << std::endl << "Converting frames to ascii, please wait!" << std::endl;
        std::thread convertThread1(ConvertFrameToAscii, 0, 0, capture.get(cv::CAP_PROP_FRAME_COUNT) * 0.25);
        std::thread convertThread2(ConvertFrameToAscii, 1, (capture.get(cv::CAP_PROP_FRAME_COUNT) * 0.25) + 1,
                                   capture.get(cv::CAP_PROP_FRAME_COUNT) * 0.50);
        std::thread convertThread3(ConvertFrameToAscii, 2, capture.get(cv::CAP_PROP_FRAME_COUNT) * 0.50 + 1,
                                   capture.get(cv::CAP_PROP_FRAME_COUNT) * 0.75);
        std::thread convertThread4(ConvertFrameToAscii, 3, capture.get(cv::CAP_PROP_FRAME_COUNT) * 0.75 + 1,
                                   capture.get(cv::CAP_PROP_FRAME_COUNT));

        convertThread1.join();
        convertThread2.join();
        convertThread3.join();
        convertThread4.join();

    } catch(cv::Exception& e) {
        std::cerr << e.msg << std::endl;
        exit(-1);
    }

    std::cout << "Done converting video!" << std::endl;
}

void Util::ConvertFrame(cv::VideoCapture& capture, int index) {
    cv::Mat frame;
    capture >> frame;
    std::string filePath = "frames/" + std::to_string(index) + ".jpg";
    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(25);
    cv::imwrite(filePath, frame);
}

CImg<> Util::rgb2gray(CImg<> color_img) {
    // create a gray image
    CImg<> gray_img(color_img.width(), color_img.height(), 1, 1, 0);

    // for all pixels x,y in image
    cimg_forXY(color_img, x, y) {
            // separation of channels
            int R = (int) color_img(x, y, 0, 0);
            int G = (int) color_img(x, y, 0, 1);
            int B = (int) color_img(x, y, 0, 2);

            // calculate gray value
            // (x, y) -> val_a,   (x, y) -> a, b, c
            int gray_val = (int) (0.299 * R + 0.587 * G + 0.114 * B);
            gray_img(x, y, 0, 0) = gray_val;
        }
    gray_img.normalize(0, 255);
    return gray_img;
}

void Util::print_gray2ascii(CImg<> gray_img, const char *file_name, int w, int h) {
    // output to out.txt
    std::ofstream out(file_name);

    gray_img.resize(w, h);

    cimg_forY(gray_img,y) {
        cimg_forX(gray_img,x) {
            int val = gray_img(x, y, 0, 0) / sizeof(ASCII_LIST);
            out << ASCII_LIST[val];
        }
        out << std::endl;
    }

    out.close();
}

void Util::ConvertFrameToAscii(int threadNum, int start, int end) {
    float aspect_ratio = 1080.0 / 1920.0;
    int new_width = 80;
    float new_height = aspect_ratio * new_width * 0.55;

    std::cout << "THREAD " << threadNum << " " << end - start << " images left" << std::endl;

    for (int i = 0; i < end - start; ++i) {
        std::string filePath = "frames/" + std::to_string(i + start) + ".jpg";
        CImg<> img(filePath.c_str());

        img = rgb2gray(img);

        std::string outPath = "ascii/" + std::to_string(i + start);
        print_gray2ascii(img, outPath.c_str(), new_width, (int)new_height);
    }
}
