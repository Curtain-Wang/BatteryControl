#include "mainwindow.h"
#include "AsyncLogger.h"
#include <QDebug>
#include <QApplication>

void logMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString logMessage;
    switch (type) {
    case QtDebugMsg:
        logMessage = "[DEBUG] " + msg;
        break;
    case QtInfoMsg:
        logMessage = "[INFO] " + msg;
        break;
    case QtWarningMsg:
        logMessage = "[WARNING] " + msg;
        break;
    case QtCriticalMsg:
        logMessage = "[CRITICAL] " + msg;
        break;
    case QtFatalMsg:
        logMessage = "[FATAL] " + msg;
        break;
    }
    AsyncLogger::instance().logMessage(logMessage);  // 使用异步日志写入
}

int main(int argc, char *argv[])
{
    AsyncLogger::instance().start();
    qInstallMessageHandler(logMessageOutput); // 安装全局日志处理器
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
