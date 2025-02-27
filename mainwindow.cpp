#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core_screencapturer.h"
#include<QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 在需要的地方使用截图器
    ScreenShooter* shooter = ScreenShooter::instance();

    // 设置截图间隔为200ms
    shooter->setInterval(200);

    // 连接截图信号到处理槽
    connect(shooter, &ScreenShooter::screenshotTaken,
            this, [this](const QPixmap& frame){
                // 在这里处理截图数据（比如保存或显示）
                //frame.save("screenshot.png");
        ui->showLabel->setPixmap(frame);
            });

    // 开始截图
    shooter->start();

    // 需要停止时调用
    // shooter->stop();


}

MainWindow::~MainWindow()
{
    delete ui;
}
