#include "tform6.h"
#include "ui_tform6.h"
#include "globalparam.h"

TForm6::TForm6(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TForm6)
{
    ui->setupUi(this);
    //设置窗口标志，确保有边框和标题栏、最小化、关闭，最大化
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
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
    ui->d36_1->setText(QString::number(static_cast<float>(timingDataBuf[36]) / 10, 'f', 1));
    ui->d37_1->setText(QString::number(static_cast<float>(timingDataBuf[37]) / 10, 'f', 1));
    ui->d42->setText(QString::number(timingDataBuf[42]));
    ui->d64->setText(QString::number(timingDataBuf[64]));
    ui->d68->setText(QString::number(timingDataBuf[68]));
}
