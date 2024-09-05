#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tform1.h"
#include "tform2.h"
#include <QDateTime>
#include <QTimer>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::isCreator(int num)
{
    switch (num) {
    case 1:
        return tform1 != nullptr;
    case 2:
        return tform2 != nullptr;
    default:
        return false;
    }
}

void MainWindow::timerUpDate()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
    ui->currentTimeLabel->setText(str);
}

void MainWindow::init()
{
    tform1 = new TForm1(this);
    tform1->show();
    tform2 = new TForm2(this);
    tform2->show();

    //时间显示
    timer1 = new QTimer(this);
    connect(timer1, &QTimer::timeout, this, &MainWindow::timerUpDate);
    timer1->setInterval(1000);
    timer1->start();
    //发送数据
    sendTimer = new QTimer(this);
    connect(sendTimer, &QTimer::timeout, this, &MainWindow::on_sendTimer_timeout);
    sendTimer->setInterval(100);
    sendTimer->start();
    //接收数据
    receiveTimer = new QTimer(this);
    connect(receiveTimer, &QTimer::timeout, this, &MainWindow::on_receiveTimer_timeout);
    receiveTimer->setInterval(10);
    receiveTimer->start();
}

void MainWindow::on_sendTimer_timeout()
{

}

void MainWindow::on_receiveTimer_timeout()
{

}
