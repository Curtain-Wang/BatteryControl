#include "tform6.h"
#include "ui_tform6.h"
#include "globalparam.h"
#include "mainwindow.h"
#include <QMessageBox>
TForm6::TForm6(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TForm6)
{
    ui->setupUi(this);
    //设置窗口标志，确保有边框和标题栏、最小化、关闭，最大化
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    setWindowIcon(QIcon(":/icons/images/battery_control_icon.ico"));
    refresh();
}

TForm6::~TForm6()
{
    delete ui;
}

void TForm6::refresh()
{
    ui->d7->setText(QString::number(timingDataBuf[7]));
    ui->d11->setText(QString::number(timingDataBuf[11]));
    ui->d32->setText(QString::number(timingDataBuf[32]));
    ui->d33->setText(QString::number(timingDataBuf[33]));
    ui->d36->setText(QString::number(static_cast<float>(static_cast<qint16>(timingDataBuf[36])) / 100, 'f', 2));
    ui->d37->setText(QString::number(static_cast<float>(static_cast<qint16>(timingDataBuf[37])) / 100, 'f', 2));
    ui->d64->setText(QString::number(timingDataBuf[64]));
    ui->d9->setText(QString::number(timingDataBuf[9]));
    ui->d31->setText(QString::number(static_cast<float>(timingDataBuf[31]) / 10, 'f', 1));
}
