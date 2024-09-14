#include "globalparam.h"
#include <QByteArray>
#include <QString>
#include "mainwindow.h"
const int DATA_REFRESH_CYCLE = 20;
int dataRefreshRemaingTime = 0;
int dataRefreshStep = 1;
int waitMessageRemaingTime = 0;
const char READ_CMD = 3;
const char WRITE_ONE_CMD = 6;
const char WRITE_MULTIPLE_CMD = 16;
const int MODULE = 1;
//手动标记，0自动 1手动 2双手动
int manualFlag = 0;
//第二次手动命令标志，0无 1设置A到B，2设置B到A
int secCmdType = 0;
int timingDataBuf[65];
QByteArray receiveDataBuf(500, 0);
int receiveStartIndex = 0;
int receiveEndIndex = 0;
QByteArray manualSendDataBuf;
int connFlag = 0;
int cycleNum = 0;
const int BR = 19200;
QString displayInfo = "电流方向：%1，循环次数：%2";
QString connStatus = "连接状态：%1";
QString runningStatus = "运行模式：%1";
int editAddrLow = 0;
MainWindow* mainwindow = nullptr;
