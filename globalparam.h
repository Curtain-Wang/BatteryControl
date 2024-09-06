#ifndef GLOBALPARAM_H
#define GLOBALPARAM_H
class QByteArray;

extern const int DATA_REFRESH_CYCLE;
extern int dataRefreshRemaingTime;
extern int dataRefreshStep;
extern int waitMessageRemaingTime;
extern int cmd;
extern int module;
extern int messageSize;
//0没有 1有
extern int manualFlag;
extern QByteArray receiveDataBuf;
extern int receiveStartIndex;
extern int receiveEndIndex;
extern QByteArray sendDataBuf;
extern int sendLength;
//0未连接 1已连接
extern int connFlag;
#endif // GLOBALPARAM_H
