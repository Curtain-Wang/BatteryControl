#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
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
    void refreshAll();
    void test();
    void loadConfig();
    void AOVUpdate();
    void BOVUpdate();
    void AUVUpdate();
    void BUVUpdate();
    void manualReadCMDBuild(char startHigh, char startLow, char numHigh, char numLow);
    void manualWriteOneCMDBuild(char startHigh, char startLow, char valueHigh, char valueLow);
    void manualWriteMultipleCMDBuild(QByteArray buf);
private slots:
    void on_sendTimer_timeout();
    void on_receiveTimer_timeout();
    void on_pushButton_5_clicked();
    void onTFormDestroyed(QObject *obj);
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void sendSerialData(const QByteArray &data);
    void on_connBtn_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_aov_editingFinished();

    void on_aov_returnPressed();

    void on_auv_editingFinished();

    void on_auv_returnPressed();

    void on_bov_editingFinished();

    void on_bov_returnPressed();

    void on_buv_editingFinished();

    void on_buv_returnPressed();

    void on_pushButton_clicked();

    void on_pushButton_7_clicked();

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
    QLabel* connectStatusLabel;
    QLabel* runningStatusLabel;
    int dataRefreshStep = 1;
};
#endif // MAINWINDOW_H
