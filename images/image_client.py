import zmq
import cv2
import base64
import numpy as np

context = zmq.Context()

# Socket to talk to server
print("Connecting to hello world server...")
socket = context.socket(zmq.REQ)
# socket.connect("tcp://localhost:5555")
socket.connect("ipc:///tmp/zmqdemo/0")

window_name = "Python (ESC to exit)"

def decode_image(encoded_data):
    decoded = base64.b64decode(encoded_data)
    nparr = np.fromstring(decoded, np.uint8).reshape(480, 640, 3)
    # img = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
    print(f"Array shape: {nparr.shape}")
    # print("Img: " + img)
    return nparr

request = 0
key = 0
while (key != 27):
    print("Sending request ", request, "...")
    socket.send_string("Hello")

    # Get the reply.
    message = socket.recv()
    print("Received reply ", request)
    # print(message)

    image = decode_image(message)
    cv2.imwrite("py_bin.jpg", image)
    cv2.imshow(window_name, image)

    key = cv2.waitKey(10)
    request += 1