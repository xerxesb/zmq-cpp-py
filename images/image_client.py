import zmq
import cv2
import numpy as np

context = zmq.Context()

# Socket to talk to server
print("Connecting to hello world server...")
socket = context.socket(zmq.REQ)
# socket.connect("tcp://localhost:5555")
socket.connect("ipc:///tmp/zmqdemo/0")

window1_name = "Python (Orig) (ESC to exit)"
window2_name = "Python (Alt) (ESC to exit)"

def decode_image(encoded_data):
    nparr = np.fromstring(encoded_data, np.uint8).reshape(480, 640, 3)
    print(f"Array shape: {nparr.shape}")
    return nparr

request = 0
key = 0
while (key != 27):
    print("Sending request ", request, "...")
    socket.send_string(str(request))

    # Get the reply.
    frame = socket.recv_multipart()
    print("Received reply ", request)

    status = frame[0] # The command is in the first frame element
    raw_orig_image = frame[1] # The original image is in the second element
    raw_alt_image = frame[2] # The alternate image is in the third element

    print(f"Status from Server: {status}")
    cv2.imshow(window1_name, decode_image(raw_orig_image))
    cv2.imshow(window2_name, decode_image(raw_alt_image))

    key = cv2.waitKey(10)
    request += 1