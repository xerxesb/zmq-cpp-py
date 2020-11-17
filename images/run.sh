#!/bin/bash

(./image_server & time python3 ./image_client.py) && killall image_server