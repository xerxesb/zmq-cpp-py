#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

#include "base64.h"

using namespace cv;

int main()
{
    // open the first webcam plugged in the computer
    cv::VideoCapture camera(0);
    if (!camera.isOpened())
    {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return 1;
    }

    // create a window to display the images from the webcam
    cv::namedWindow("C++", CV_WINDOW_AUTOSIZE);

    // this will contain the image from the webcam
    cv::Mat frame;

    // display the frame until you press a key
    // for(;;)
    {
        camera >> frame;
        //   if(frame.empty()) break; // end of video stream

        imshow("C++", frame);

        // Convert frame to serialisable format
        std::vector<uchar> array;
        if (frame.isContinuous())
        {
            array.assign(frame.data, frame.data + frame.total() * frame.channels());
        }
        else
        {
            for (int i = 0; i < frame.rows; ++i)
            {
                array.insert(array.end(), frame.ptr<uchar>(i), frame.ptr<uchar>(i) + frame.cols * frame.channels());
            }
        }

        std::string encodedData = base64_encode(&array[0], array.size());
        std::cout << encodedData << std::endl;

        //   if(waitKey(10) == 27) break; // stop capturing by pressing ESC
    }

    return 0;
}