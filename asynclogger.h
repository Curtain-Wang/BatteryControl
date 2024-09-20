#ifndef ASYNCLOGGER_H
#define ASYNCLOGGER_H

#include <QThread>
#include <QFile>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>

class AsyncLogger : public QThread
{
    Q_OBJECT

public:
    static AsyncLogger& instance();  // 单例模式

    void logMessage(const QString& message);
    void stop();  // 停止日志线程

protected:
    void run() override;

private:
    AsyncLogger(QObject* parent = nullptr);
    ~AsyncLogger();

    QFile logFile;
    QQueue<QString> logQueue;
    QMutex queueMutex;
    QWaitCondition waitCondition;
    bool exitFlag;
};

#endif // ASYNCLOGGER_H
