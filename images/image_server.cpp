#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

#include <zmq.hpp>

using namespace cv;

//! Write cv::Mat as binary
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
    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};
    // construct a REP (reply) socket and bind to interface
    zmq::socket_t socket{context, zmq::socket_type::rep};
    // socket.bind("tcp://*:5555");
    socket.bind("ipc:///tmp/zmqdemo/0");

    // create a window to display the images from the webcam
    cv::namedWindow("C++", CV_WINDOW_AUTOSIZE);

    std::cout << "Starting loop..." << std::endl;
    // display frames indefinitely until ESC pressed
    for (int i = 0; i >= 0; i++)
    {
        std::cout << "Server Frame " << i << std::endl;
        // receive a request from client
        zmq::message_t request;
        socket.recv(request, zmq::recv_flags::none);

        std::vector<uchar> input_arr(request.size());
        cv::Mat input_frame(480, 640, CV_8UC3);
        std::memcpy(input_frame.data, request.data(), request.size());

        imshow("(C++) Input Frame (ESC to exit)", input_frame);

        // Convert frame to serialisable format
        std::vector<uchar> array;
        array.assign(input_frame.data, input_frame.data + input_frame.total() * input_frame.channels());
        
        std::cout << "Sending payload " << i << std::endl;
        socket.send(zmq::buffer("OK"), zmq::send_flags::sndmore);  // Send Status
        socket.send(zmq::buffer(std::vector<uint>{}), zmq::send_flags::sndmore);  // Send metadata payload
        socket.send(zmq::buffer(array), zmq::send_flags::sndmore); // Send original image
        std::reverse(array.begin(), array.end());
        socket.send(zmq::buffer(array)); // Second alternate image

        if (cv::waitKey(10) == 27)
            break; // stop capturing by pressing ESC
    }

    return 0;
}