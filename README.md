Getting Started
---------------

Prerequisites:
* Assuming you're on Ubuntu 18.04 (LTS):
* Assuming you have a working c++, CMake set up
* Assuming you have decent Pythons. 


Install deps:
* Install libzmq3-dev - this is the standard distro libs for ZeroMQ
    * `sudo apt install libzme3-dev`
* Install cppzmq - this is a header-only cpp library for zmq
    * `git clone https://github.com/zeromq/cppzmq.git`
    * `cd cppzmq && mkdir build && cd build`
    * `cmake -DCPPZMQ_BUILD_TESTS=OFF ../`
    * `make && sudo make install`
* Install pyzmq - this is the python lib for ZeroMQ
    * `pip3 install pyzmq`
    

Building
--------

* CD into one of the example directories
* Create a `build` dir and `cd build`
* `cmake ../`
* `make`
* To get the `ipc` examples working, you need to create a directory `/tmp/zmqdemo`


To-do
-----

* Update cmake to copy py examples into build directory
* Build sh script
* Simple smoketest sh script
* ~~Change cmake to target std-11~~
* ~~Change examples to IPC using named pipes rather than TCP~~
* Build example with opencv mat data



References
----------

Examples/demos
* https://gist.github.com/wonkoderverstaendige/7521416
* https://gist.github.com/jeffbass/ebf877e964c9a0b84272
* https://stackoverflow.com/questions/6915191/simple-ipc-between-c-and-python-cross-platform
* https://gist.github.com/wonkoderverstaendige/7521416
* https://github.com/kzykmyzw/cpp-python-communication

Other references
* https://zeromq.org/languages/cplusplus/
* https://zeromq.org/languages/python/
* https://github.com/zeromq/cppzmq
* https://github.com/kzykmyzw/cpp-python-communication
* https://stackoverflow.com/questions/41251474/how-to-import-zeromq-libraries-in-cmake/41252437
* https://github.com/zeromq/cppzmq/issues/294#issuecomment-464647724
https://stackoverflow.com/questions/41251474/how-to-import-zeromq-libraries-in-cmake/41252437