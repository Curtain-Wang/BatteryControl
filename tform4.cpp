#include "tform4.h"
#include "ui_tform4.h"
#include "globalparam.h"
TForm4::TForm4(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TForm4)
{
    ui->setupUi(this);
    //设置窗口标志，确保有边框和标题栏、最小化、关闭，最大化
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
}

TForm4::~TForm4()
{
    delete ui;
}

void TForm4::refresh()
{
    ui->d5->setText(QString::number(timingDataBuf[5]));
    ui->d38->setText(QString::number(timingDataBuf[38]));
    ui->d20_1->setText(QString::number(static_cast<float>(timingDataBuf[20]) / 10, 'f', 1));
    ui->d21_1->setText(QString::number(static_cast<float>(timingDataBuf[21]) / 10, 'f', 1));
    ui->d22_1->setText(QString::number(static_cast<float>(timingDataBuf[22]) / 10, 'f', 1));
    ui->d23_1->setText(QString::number(static_cast<float>(timingDataBuf[23]) / 10, 'f', 1));
    ui->d24_1->setText(QString::number(static_cast<float>(timingDataBuf[24]) / 10, 'f', 1));
    ui->d34_1->setText(QString::number(static_cast<float>(timingDataBuf[34]) / 10, 'f', 1));
    ui->d35_1->setText(QString::number(static_cast<float>(timingDataBuf[35]) / 10, 'f', 1));
    ui->d40_1->setText(QString::number(static_cast<float>(timingDataBuf[40]) / 10, 'f', 1));
}
