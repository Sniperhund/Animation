//
// Created by Lucas on 09/11/2022.
//

#ifndef ANIMATION_UTIL_H
#define ANIMATION_UTIL_H

#include <opencv2/core/types_c.h>
#include "opencv2/opencv.hpp"

#undef cimg_display
#define cimg_display 0
#define cimg_use_jpeg
#include "CImg.h"

using namespace cimg_library;

class Util {
private:
    static void ConvertFrame(cv::VideoCapture& capture, int index);
    static void ConvertFrameToAscii(int threadNum, int start, int end);

    static CImg<> rgb2gray(CImg<> color_img);
    static void print_gray2ascii(CImg<> gray_img, const char* file_name, int w, int h);
public:
    static const char *ASCII_LIST;

    static void InitConsole();
    static void ShowVideo(bool playAudio);
    static void ConvertVideo(bool splitVideo);
};


#endif //ANIMATION_UTIL_H
