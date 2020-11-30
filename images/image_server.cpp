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
        zmq::message_t request_rgb, request_depth;
        socket.recv(request_rgb, zmq::recv_flags::none);
        if (request_rgb.more()) {
            socket.recv(request_depth, zmq::recv_flags::none);
        }

        std::vector<uchar> input_arr(request_rgb.size());
        cv::Mat rgb_frame(480, 640, CV_8UC3);
        cv::Mat depth_frame(480, 640, CV_8UC3);
        std::memcpy(rgb_frame.data, request_rgb.data(), request_rgb.size());
        std::memcpy(depth_frame.data, request_depth.data(), request_depth.size());


        imshow("(C++) RGB Frame (ESC to exit)", rgb_frame);
        imshow("(C++) Depth Frame (ESC to exit)", depth_frame);

        // Convert frame to serialisable format
        std::vector<uchar> array;
        array.assign(rgb_frame.data, rgb_frame.data + rgb_frame.total() * rgb_frame.channels());
        
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