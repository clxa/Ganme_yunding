#include "mainwindow.h"
#include<QDebug>
#include <QApplication>
#include"core_screencapturer.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    // 获取所有显示器信息
    QList<QRect> resolutions = ScreenCapturer::getScreenResolutions();

    // 执行屏幕捕获（约3-5ms @4K分辨率）
    //QPixmap screenshot = ScreenCapturer::captureScreen();

    // 保存到文件
    //screenshot.save("screenshot.png");


    w.show();
    return a.exec();
}
