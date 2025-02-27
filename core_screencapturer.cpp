/* screenshooter.cpp */
#include "core_screencapturer.h"
#include <QThread>
#include<QTime>
// 单例实例指针初始化
ScreenShooter* ScreenShooter::instance()
{
    static ScreenShooter instance;
    return &instance;
}

// 构造函数初始化定时器
ScreenShooter::ScreenShooter(QObject *parent)
    : QObject(parent)
{
    m_timer = new QTimer(this);
    // 设置单次触发定时器（更精确控制时间间隔）
    m_timer->setSingleShot(true);

    // 连接定时器信号到截图槽
    connect(m_timer, &QTimer::timeout, this, &ScreenShooter::captureFrame);
}

QSize ScreenShooter::getPrimaryScreenResolution()
{
    // 获取主屏幕对象
    QScreen* primaryScreen = QGuiApplication::primaryScreen();
    if(!primaryScreen){
        qWarning() << "无法获取主屏幕信息";
        return QSize(0, 0);
    }
    // 返回屏幕的物理分辨率
    return primaryScreen->geometry().size();
}

void ScreenShooter::setInterval(int ms)
{
    // 限制最小间隔为10ms（保护低端CPU）
    m_interval = qMax(10, ms);
}

void ScreenShooter::start()
{
    if(m_isRunning) return;

    m_isRunning = true;
    // 立即触发第一次截图
    QMetaObject::invokeMethod(this, &ScreenShooter::captureFrame,
                              Qt::QueuedConnection);
}

void ScreenShooter::stop()
{
    m_isRunning = false;
    m_timer->stop();
}

bool ScreenShooter::isRunning() const
{
    return m_isRunning;
}

void ScreenShooter::captureFrame()
{
    if(!m_isRunning) return;

    // 记录开始时间用于精确控制间隔
    auto startTime = QDateTime::currentDateTime();

    // 获取主屏幕对象
    QScreen* screen = QGuiApplication::primaryScreen();
    if(screen){
        // 使用grabWindow方法获取全屏截图（效率最高）
        QPixmap screenshot = screen->grabWindow(0);
        if(!screenshot.isNull()){
            // 发送截图信号（使用const引用减少拷贝开销）
            emit screenshotTaken(screenshot);
        }
    }

    // 计算实际耗时
    auto elapsed = startTime.msecsTo(QDateTime::currentDateTime());
    // 动态调整下次触发时间（保持精确间隔）
    int nextInterval = qMax(0, m_interval - elapsed);

    // 如果仍在运行状态，则再次启动定时器
    if(m_isRunning){
        m_timer->start(nextInterval);
    }
}
