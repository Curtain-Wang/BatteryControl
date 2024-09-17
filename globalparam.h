#ifndef GLOBALPARAM_H
#define GLOBALPARAM_H
class QByteArray;
class QString;
class MainWindow;
extern int deviceType;
extern const int DATA_REFRESH_CYCLE;
extern int dataRefreshRemaingTime;
extern int dataRefreshStep;
extern int waitMessageRemaingTime;
extern const char READ_CMD;
extern const char WRITE_ONE_CMD;
extern const char WRITE_MULTIPLE_CMD;
extern const int MODULE;
//手动标记，0自动 1手动 2双手动
extern int manualFlag;
//第二次手动命令标志，0无 1设置A到B，2设置B到A
extern int secCmdType;
extern QByteArray receiveDataBuf;
extern int receiveStartIndex;
extern int receiveEndIndex;
extern QByteArray manualSendDataBuf;
//定时获取到的数据
extern int timingDataBuf[];
//0未连接 1已连接
extern int connFlag;
extern const int BR;
extern int cycleNum;
extern QString displayInfo1;
extern QString displayInfo2;
extern int chargeTime;
extern QString connStatus;
extern QString runningStatus;
extern int editAddrLow;
extern MainWindow* mainwindow;
extern int ATurnHighV;
extern int ATurnLowV;
extern int BTurnHighV;
extern int BTurnLowV;
extern QString companyName;
//被编辑的lineEdit名称
extern QString lineEditName;
#endif // GLOBALPARAM_H
