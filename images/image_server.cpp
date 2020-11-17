#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;

int main()
{
    // open the first webcam plugged in the computer
    cv::VideoCapture camera(0);
    if (!camera.isOpened()) {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return 1;
    }

    // create a window to display the images from the webcam
    cv::namedWindow("C++", CV_WINDOW_AUTOSIZE);

    // this will contain the image from the webcam
    cv::Mat frame;
        
    // display the frame until you press a key
    for(;;)
    {
          camera >> frame;
          if(frame.empty()) break; // end of video stream

          imshow("C++", frame);

          if( waitKey(10) == 27 ) break; // stop capturing by pressing ESC 
    }
    
    return 0;
}