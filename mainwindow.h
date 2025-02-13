#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "zlgcan.h"
class TForm1;
class TForm2;
class TForm3;
class TForm4;
class TForm5;
class TForm6;
class QTimer;
class QSerialPort;
class QLabel;
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool isCreated(int num);
    void timerUpDate();
    void init();
    void sendPortData(QByteArray data = nullptr);
    QByteArray calculateCRCArray(const QByteArray &data, int length);
    void cacheReceiveData();
    bool receiveDataCRCCheck(const QByteArray &data);
    void dealMessage(const QByteArray &data);
    void refreshPort();
    void sendGetAllDataCMD();
    void refresh();
    void refresh(int num);
    void refreshRunningStatusInfo();
    void refreshAll();
    void loadConfig();
    void manualReadCMDBuild(char startHigh, char startLow, char numHigh, char numLow);
    void manualWriteOneCMDBuild(char startHigh, char startLow, char valueHigh, char valueLow, quint8 secFlag = 0);
    void manualWriteMultipleCMDBuild(QByteArray buf, quint8 secFlag = 0);
    void setAtoB();
    void setBtoA();
    void secondCMDSend();
    //CAN
    bool initCAN();
    void closeCAN();
    quint32 generateCANId(quint8 module_id, quint8 command, quint8 frame_sequence, quint8 module_number);
    void receiveCANData();
    bool sendCANData(quint32 canId, uint8_t data[8]);
    void decodeCANData(can_frame frame);
    void writeCanData(quint8 addr, char valueLow, char valueHigh);
    void staticCanDataInit();
private slots:
    void onSendTimerTimeout();
    void onReceiveTimerTimeout();
    void on_pushButton_5_clicked();
    void onTFormDestroyed(QObject *obj);
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void sendSerialData(const QByteArray &data);
    void on_connBtn_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();

    void resetKeyPressCount();  // 重置空格键按下计数

    void onEditingFinished();

    void on_comboBox_3_currentIndexChanged(int index);

    void chargeTimeCountAdd();

    void on_pushButton_10_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort* serialPort;
    TForm1* tform1 = nullptr;
    TForm2* tform2 = nullptr;
    TForm3* tform3 = nullptr;
    TForm4* tform4 = nullptr;
    TForm5* tform5 = nullptr;
    TForm6* tform6 = nullptr;
    QTimer* timer1 = nullptr;
    QTimer* sendTimer = nullptr;
    QTimer* receiveTimer = nullptr;
    QTimer* chargeTimeCountTimer = nullptr;
    QLabel* connectStatusLabel;
    QLabel* runningStatusLabel;
    QLabel* companyNameLabel;
    QLabel* companyImageLabel;
    int dataRefreshStep = 1;
    int spaceKeyPressCount = 0; // 空格键按下次数
    QTimer *resetTimer;     // 用于重置按键计数的定时器
    int step = 0;
    //CAN相关
    IProperty* property; //CAN属性
    DEVICE_HANDLE dhandle;
    CHANNEL_HANDLE chHandle;
    //写数据的时候可以根据canid获取到can_frame
    QHash<canid_t, can_frame> canFrameHash;
    //写数据的时候可以根据之前485的地址获取到读的canid
    QHash<quint8, canid_t> canidHash;
    //写数据的时候可以根据485地址获取到低Byte的索引
    QHash<quint8, qint8> lowByteIndexHash;
    //写数据的时候可以根据485地址获取到高Byte的索引
    QHash<quint8, qint8> highByteIndexHash;
    //写数据的时候可以根据485地址获取到写的canid
    QHash<quint8, canid_t> writeCanidHash;

    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
