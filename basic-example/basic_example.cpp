#include <zmq.hpp>

int main()
{
    zmq::context_t ctx;
    zmq::socket_t sock(ctx, zmq::socket_type::push);
    sock.bind("inproc://test");

    // sock.send(zmq::buffer(std::string_view("Hello, world")), zmq::send_flags::dontwait);
    sock.send(zmq::str_buffer("Hello, world"), zmq::send_flags::dontwait);
}