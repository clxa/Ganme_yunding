#ifndef CORE_SCREENCAPTURER_H
#define CORE_SCREENCAPTURER_H

#endif // CORE_SCREENCAPTURER_H
// screen_capturer.h
#pragma once
#include <QList>
#include <QRect>
#include <QPixmap>
#include <windows.h>

class ScreenCapturer
{
public:
    // 获取所有显示器的逻辑分辨率（考虑系统缩放）
    static QList<QRect> getScreenResolutions();

    // 高效屏幕捕获（返回物理像素尺寸）
    static QPixmap captureScreen();

private:
    // 处理Windows GDI资源自动释放
    struct GDIObjGuard {
        HGDIOBJ obj;
        explicit GDIObjGuard(HGDIOBJ o) : obj(o) {}
        ~GDIObjGuard() { if(obj) DeleteObject(obj); }
    };

    struct DCGuard {
        HDC dc;
        explicit DCGuard(HDC h) : dc(h) {}
        ~DCGuard() { if(dc) DeleteDC(dc); }
    };
};
