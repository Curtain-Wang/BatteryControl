#ifndef GLOBALPARAM_H
#define GLOBALPARAM_H
class QByteArray;
class QString;
class MainWindow;
extern const int DATA_REFRESH_CYCLE;
extern int dataRefreshRemaingTime;
extern int dataRefreshStep;
extern int waitMessageRemaingTime;
extern const char READ_CMD;
extern const char WRITE_ONE_CMD;
extern const char WRITE_MULTIPLE_CMD;
extern const int MODULE;
//0没有 1有
extern int manualFlag;
extern QByteArray receiveDataBuf;
extern int receiveStartIndex;
extern int receiveEndIndex;
extern QByteArray manualSendDataBuf;
//定时获取到的数据
extern int timingDataBuf[];
//0未连接 1已连接
extern int connFlag;
extern const int BR;
extern float aov;
extern float auv;
extern float bov;
extern float buv;
extern int cycleNum;
extern int autoOrManual;
extern QString displayInfo;
extern QString connStatus;
extern QString runningStatus;
extern int editAddrLow;
extern MainWindow* mainwindow;
#endif // GLOBALPARAM_H
