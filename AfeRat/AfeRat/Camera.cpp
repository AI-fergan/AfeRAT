#include "Camera.h"

Camera::Camera(char* ip, short port) {
    _communication = new Communication(ip, port);
    _cap = 0;
}

Camera::~Camera() {
    _cap.release();
}

void Camera::shotFrame() {
    Mat frame;
    string filename = FRAME;

    remove(filename.c_str());

    // Capture a new frame from the camera
    _cap >> frame;

    // Check if the frame is empty
    if (frame.empty()) {
        cerr << "Error: Captured frame is empty" << endl;
        return;
    }

    // Save the frame as an image file
    imwrite(filename, frame);
}

void Camera::run() {
    while (true) {
        try {
            if (_communication->getMsg() == "shot") {
                shotFrame();
                _communication->sendFile(FRAME);
                Sleep(2000);
            }
        }
        catch (...) {
            while (!_communication->connect()) {}
        }
    }
}
