#include "AsyncLogger.h"
#include <QDateTime>
#include <QTextStream>

AsyncLogger::AsyncLogger(QObject* parent)
    : QThread(parent), exitFlag(false)
{
    logFile.setFileName("async_application.log");
    if (!logFile.open(QIODevice::Append | QIODevice::Text)) {
        qCritical() << "Failed to open log file.";
    }
}

AsyncLogger::~AsyncLogger()
{
    stop();  // 确保日志线程结束
    logFile.close();
}

AsyncLogger& AsyncLogger::instance()
{
    static AsyncLogger loggerInstance;  // 单例模式确保全局只有一个日志线程实例
    return loggerInstance;
}

void AsyncLogger::logMessage(const QString& message)
{
    QMutexLocker locker(&queueMutex);  // 线程安全地访问日志队列
    logQueue.enqueue(message);
    waitCondition.wakeAll();  // 通知日志线程有新消息待处理
}

void AsyncLogger::stop()
{
    exitFlag = true;
    waitCondition.wakeAll();  // 唤醒线程以终止循环
    wait();  // 等待线程结束

    // 强制写入剩余日志
    while (!logQueue.isEmpty()) {
        QString message = logQueue.dequeue();
        QTextStream out(&logFile);
        out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") << " - " << message << Qt::endl;
    }
}

void AsyncLogger::run()
{
    while (!exitFlag) {
        QString message;
        {
            QMutexLocker locker(&queueMutex);
            if (logQueue.isEmpty()) {
                waitCondition.wait(&queueMutex);  // 阻塞线程直到有新的日志信息
            }
            if (!logQueue.isEmpty()) {
                message = logQueue.dequeue();
            }
        }

        if (!message.isEmpty()) {
            QTextStream out(&logFile);
            out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") << " - " << message << Qt::endl;
            logFile.flush();  // 确保日志写入磁盘
        }
    }
}
