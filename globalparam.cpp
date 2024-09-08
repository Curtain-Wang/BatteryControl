#include "globalparam.h"
#include <QByteArray>
#include <QString>
const int DATA_REFRESH_CYCLE = 20;
int dataRefreshRemaingTime = 0;
int dataRefreshStep = 1;
int waitMessageRemaingTime = 0;
int cmd = -1;
int module = 1;
int messageSize = -1;
//手动标记，0自动 1手动
int manualFlag = 0;
int timingDataBuf[119];
const int TIMING_MESSAGE_SIZE = 243;
QByteArray receiveDataBuf(500, 0);
int receiveStartIndex = 0;
int receiveEndIndex = 0;
QByteArray sendDataBuf(500, 0);
int sendLength = 0;
int connFlag = 0;
int aov = 0;
int auv = 0;
int bov = 0;
int buv = 0;
const int BR = 19200;
