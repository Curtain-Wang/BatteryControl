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
    setWindowIcon(QIcon(":/icons/images/battery_control_icon.ico"));
    refresh();
}

TForm4::~TForm4()
{
    delete ui;
}

void TForm4::refresh()
{
    ui->d5->setText(QString::number(timingDataBuf[5], 16).toUpper());
    qDebug() << "d20: " << timingDataBuf[20];
    ui->d20_1->setText(QString::number(static_cast<float>(timingDataBuf[20]) / 10, 'f', 1));
    ui->d21_1->setText(QString::number(static_cast<float>(timingDataBuf[21]) / 10, 'f', 1));
    qint16 value = timingDataBuf[22];
    if(deviceType == 2)
    {
        ui->d22_1->setText(QString::number(static_cast<float>(value) / 10, 'f', 1));
        value = timingDataBuf[23];
        ui->d23_1->setText(QString::number(static_cast<float>(value) / 10, 'f', 1));
    }
    else
    {
        ui->d22_1->setText(QString::number(static_cast<float>(value) / 100, 'f', 2));
        value = timingDataBuf[23];
        ui->d23_1->setText(QString::number(static_cast<float>(value) / 100, 'f', 2));
    }
    value = timingDataBuf[34];
    ui->d34_1->setText(QString::number(static_cast<float>(value) / 100, 'f', 2));
    value = timingDataBuf[35];
    ui->d35_1->setText(QString::number(static_cast<float>(value) / 10, 'f', 1));
    qint32 value32 = timingDataBuf[38] + timingDataBuf[39] * 256;
    ui->d38_1->setText(QString::number(static_cast<float>(value32) / 10, 'f', 1));
    value32 = timingDataBuf[40] + timingDataBuf[41] * 256;
    ui->d40_1->setText(QString::number(static_cast<float>(value32) / 10, 'f', 1));
    ui->d24_1->setText(QString::number(static_cast<float>(timingDataBuf[24]) / 10, 'f', 1));
}
