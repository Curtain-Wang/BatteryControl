#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class TForm1;
class TForm2;
class QTimer;
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

    bool isCreator(int num);
    void timerUpDate();
    void init();
private slots:
    void on_sendTimer_timeout();
    void on_receiveTimer_timeout();
private:
    Ui::MainWindow *ui;
    TForm1* tform1;
    TForm2* tform2;
    QTimer* timer1;
    QTimer* sendTimer;
    QTimer* receiveTimer;

};
#endif // MAINWINDOW_H
