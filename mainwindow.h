#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
class TForm1;
class TForm2;
class TForm3;
class QTimer;
class QSerialPort;
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
    void sendPortData();
    QByteArray calculateCRCArray(const QByteArray &data, int length);
    void cacheReceiveData();
    bool receiveDataCRCCheck(const QByteArray &data);
    void dealMessage(const QByteArray &data);
private slots:
    void on_sendTimer_timeout();
    void on_receiveTimer_timeout();
    void on_pushButton_5_clicked();
    void onTFormDestroyed(QObject *obj);
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_6_clicked();
    void sendSerialData(const QByteArray &data);
private:
    Ui::MainWindow *ui;
    QSerialPort* serialPort;
    TForm1* tform1 = nullptr;
    TForm2* tform2 = nullptr;
    TForm3* tform3 = nullptr;
    QTimer* timer1 = nullptr;
    QTimer* sendTimer = nullptr;
    QTimer* receiveTimer = nullptr;


};
#endif // MAINWINDOW_H
