/* screenshooter.h */
#ifndef SCREENSHOOTER_H
#define SCREENSHOOTER_H

#include <QObject>
#include <QTimer>
#include <QScreen>
#include <QGuiApplication>
#include <QPixmap>

// 使用单例模式确保只有一个截图实例
class ScreenShooter : public QObject
{
    Q_OBJECT
public:
    // 获取单例实例（线程安全）
    static ScreenShooter* instance();

    // 获取主屏幕分辨率（静态工具函数）
    static QSize getPrimaryScreenResolution();

    // 设置截图间隔（毫秒）
    void setInterval(int ms);
    // 开始截图
    void start();
    // 停止截图
    void stop();
    // 检查是否正在运行
    bool isRunning() const;

signals:
    // 截图完成信号（携带截图数据）
    void screenshotTaken(const QPixmap& frame);

private:
    // 私有构造函数（单例模式）
    explicit ScreenShooter(QObject *parent = nullptr);
    // 禁用拷贝构造和赋值
    ScreenShooter(const ScreenShooter&) = delete;
    ScreenShooter& operator=(const ScreenShooter&) = delete;

    QTimer* m_timer;        // 截图定时器
    int m_interval = 100;   // 默认100ms间隔
    bool m_isRunning = false;

private slots:
    void captureFrame();    // 实际执行截图的槽函数
};

#endif // SCREENSHOOTER_H
