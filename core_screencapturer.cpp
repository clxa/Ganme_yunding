// screen_capturer.cpp
#include "core_screencapturer.h"
#include <QGuiApplication>
#include <QScreen>
#include <QWindow>

QList<QRect> ScreenCapturer::getScreenResolutions()
{
    QList<QRect> resolutions;
    const auto screens = QGuiApplication::screens();

    for (const QScreen* screen : screens) {
        resolutions.append(screen->geometry());
    }
    return resolutions;
}

QPixmap ScreenCapturer::captureScreen()
{
    // 获取物理屏幕尺寸
    const int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    const int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    const int x = GetSystemMetrics(SM_XVIRTUALSCREEN);
    const int y = GetSystemMetrics(SM_YVIRTUALSCREEN);

    // 创建设备上下文
    HDC hdcScreen = GetDC(nullptr);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);

    // 使用RAII守卫自动释放资源
    GDIObjGuard bitmapGuard(hBitmap);
    DCGuard dcGuard(hdcMem);

    SelectObject(hdcMem, hBitmap);
    BitBlt(hdcMem, 0, 0, width, height, hdcScreen, x, y, SRCCOPY);
    ReleaseDC(nullptr, hdcScreen);

    // 获取DIB数据
    BITMAPINFOHEADER bmi = {0};
    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biWidth = width;
    bmi.biHeight = -height; // 顶向下的位图
    bmi.biPlanes = 1;
    bmi.biBitCount = 32;
    bmi.biCompression = BI_RGB;

    QImage image(width, height, QImage::Format_ARGB32);
    GetDIBits(hdcMem, hBitmap, 0, height, image.bits(), (BITMAPINFO*)&bmi, DIB_RGB_COLORS);

    // 修复颜色通道顺序（BGRA -> ARGB）
    image = image.rgbSwapped();

    return QPixmap::fromImage(image);
}
