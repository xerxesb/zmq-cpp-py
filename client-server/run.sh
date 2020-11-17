#!/bin/bash

(./zmq_server & time python3 ./zmq_client.py) && killall zmq_server