#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>

#include <zmq.hpp>

#include "base64.h"

using namespace cv;

//! Write cv::Mat as binary
/*!
\param[out] ofs output file stream
\param[in] out_mat mat to save
*/
bool writeMatBinary(std::ofstream &ofs, const cv::Mat &out_mat)
{
    if (!ofs.is_open())
    {
        return false;
    }
    if (out_mat.empty())
    {
        int s = 0;
        ofs.write((const char *)(&s), sizeof(int));
        return true;
    }
    int type = out_mat.type();
    ofs.write((const char *)(&out_mat.rows), sizeof(int));
    ofs.write((const char *)(&out_mat.cols), sizeof(int));
    ofs.write((const char *)(&type), sizeof(int));
    ofs.write((const char *)(out_mat.data), out_mat.elemSize() * out_mat.total());

    return true;
}

//! Save cv::Mat as binary
/*!
\param[in] filename filaname to save
\param[in] output cvmat to save
*/
bool SaveMatBinary(const std::string &filename, const cv::Mat &output)
{
    std::ofstream ofs(filename, std::ios::binary);
    return writeMatBinary(ofs, output);
}

void SaveVecBinary(const std::string &filename, const std::vector<uchar> data)
{
    std::ofstream ofs(filename, std::ios::binary);
    ofs.write((char *)&data[0], data.size());
}

std::string type2str(int type)
{
    std::string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch (depth)
    {
    case CV_8U:
        r = "8U";
        break;
    case CV_8S:
        r = "8S";
        break;
    case CV_16U:
        r = "16U";
        break;
    case CV_16S:
        r = "16S";
        break;
    case CV_32S:
        r = "32S";
        break;
    case CV_32F:
        r = "32F";
        break;
    case CV_64F:
        r = "64F";
        break;
    default:
        r = "User";
        break;
    }

    r += "C";
    r += (chans + '0');

    return r;
}

int main()
{
    // open the first webcam plugged in the computer
    cv::VideoCapture camera(0);
    if (!camera.isOpened())
    {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return 1;
    }

    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};
    // construct a REP (reply) socket and bind to interface
    zmq::socket_t socket{context, zmq::socket_type::rep};
    // socket.bind("tcp://*:5555");
    socket.bind("ipc:///tmp/zmqdemo/0");

    // create a window to display the images from the webcam
    cv::namedWindow("C++", CV_WINDOW_AUTOSIZE);
    cv::Mat frame;

    std::cout << "Starting loop..." << std::endl;
    // display frames indefinitely until ESC pressed
    for (int i = 0; i >= 0; i++)
    {
        std::cout << "Server Frame " << i << std::endl;
        // receive a request from client
        zmq::message_t request;
        socket.recv(request, zmq::recv_flags::none);
        std::cout << "Received " << request.to_string() << std::endl;

        camera >> frame;
        //   if(frame.empty()) break; // end of video stream
        imshow("C++ (ESC to exit)", frame);
        // SaveMatBinary("cpp_bin.raw", frame);

        std::string ty = type2str(frame.type());
        printf("Matrix: %s %dx%d \n", ty.c_str(), frame.cols, frame.rows);
        // std::cout << "Mat Type = " << frame.type() << std::endl;

        // std::vector<uchar> jpg;
        // std::vector<int> param(2);
        // param[0] = cv::IMWRITE_JPEG_QUALITY;
        // param[1] = 80;//default(95) 0-100k
        // cv::imencode(".jpg", frame, jpg, param);
        // SaveVecBinary("cpp_bin.jpg", jpg);

        // Convert frame to serialisable format
        std::vector<uchar> array;
        array.assign(frame.data, frame.data + frame.total() * frame.channels());
        // std::string encodedData = base64_encode(&array[0], array.size());

        // std::string encodedData = base64_encode(&jpg[0], jpg.size());

        std::cout << "Sending payload " << i << std::endl;
        socket.send(zmq::buffer(array));

        // std::cout << encodedData << std::endl;

        // std::this_thread::sleep_for(std::chrono::seconds(1));

        if (cv::waitKey(10) == 27)
            break; // stop capturing by pressing ESC
    }

    return 0;
}