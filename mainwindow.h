#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QQueue>
#include <QMutex>
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
    QByteArray calculateCRCArray(const QByteArray &data, int length);
    void cacheReceiveData();
    bool receiveDataCRCCheck(const QByteArray &data);
    void dealMessage(const QByteArray &data);
    void refreshPort();
    void addGetAllDataCMD();
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
    //添加通信任务到队列
    void enqueueCommand(const QByteArray &command);
    void clearQueueCommand();
private slots:
    //处理队列中的通信任务
    void processNextCommand();
    //处理串口数据读取
    void handleReadyRead();
    //超时处理
    void handleTimeout();
    void onDataRefreshTimerTimeout();
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
    QTimer* dataRefreshTimer = nullptr;
    QTimer* chargeTimeCountTimer = nullptr;
    QLabel* connectStatusLabel;
    QLabel* runningStatusLabel;
    QLabel* companyNameLabel;
    QLabel* companyImageLabel;
    int dataRefreshStep = 1;
    int spaceKeyPressCount = 0; // 空格键按下次数
    QTimer *resetTimer;     // 用于重置按键计数的定时器
    QQueue<QByteArray> commandQueue;    //通信队列
    QMutex queueMutex;                // 互斥锁，保护队列
    QTimer *timeoutTimer;             // 超时计时器
    QByteArray receivedDataBuffer;    // 缓存接收到的数据
    QByteArray currentCommand;        // 保存当前正在发送的指令
    const int timeoutDuration = 2000; // 超时设为2秒
    int retryCount;
    const int maxRetries = 3;         // 最大重试次数


    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
