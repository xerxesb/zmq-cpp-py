import zmq
import cv2
import numpy as np
import time

context = zmq.Context()

# Socket to talk to server
print("Connecting to hello world server...")
socket = context.socket(zmq.REQ)
# socket.connect("tcp://localhost:5555")
socket.connect("ipc:///tmp/zmqdemo/0")

window_name_input = "Python (Input) (ESC to exit)"
window_name_output_orig = "Python (Orig from cpp) (ESC to exit)"
window_name_output_alt = "Python (Alt from cpp) (ESC to exit)"

def decode_image(encoded_data):
    nparr = np.fromstring(encoded_data, np.uint8).reshape(480, 640, 3)
    print(f"Array shape: {nparr.shape}")
    return nparr

def decode_metadata(encoded_data):
    def obj(e):
        o = {}
        o["id"] = e[0]
        o["intensity"] = e[1]
        return o
    if len(encoded_data) == 0:
        return []
    arr = np.frombuffer(encoded_data, np.uint32)
    return list(map(obj, np.split(arr, len(arr) / 2)))


camera = cv2.VideoCapture(0)

request = 0
key = 0
while (key != 27):
    # time.sleep(0.1)

    ret, frame = camera.read() 
    cv2.imshow(window_name_input, frame)

    print("Sending request ", request, "...")
    socket.send(frame)

    # Get the reply.
    frame = socket.recv_multipart()
    print("Received reply ", request)

    status = str(frame[0]) # The command is in the first frame element
    raw_metadata = frame[1] # The command is in the first frame element
    raw_orig_image = frame[2] # The original image is in the second element
    raw_alt_image = frame[3] # The alternate image is in the third element

    if ("OK" in status):
        print(f"Status from Server: {status}")
        print(f"Metadata from Server: {decode_metadata(raw_metadata)}")
        cv2.imshow(window_name_output_orig, decode_image(raw_orig_image))
        cv2.imshow(window_name_output_alt, decode_image(raw_alt_image))

        key = cv2.waitKey(10)
        request += 1

camera.release()
cv2.destroyAllWindows()