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
    mainwindow = qobject_cast<MainWindow*>(parent);
    //设置窗口标志，确保有边框和标题栏、最小化、关闭，最大化
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    setWindowIcon(QIcon(":/icons/images/battery_control_icon.ico"));
    refresh();
    connectAll();
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
    ui->d64->setText(QString::number(timingDataBuf[64]));
}

void TForm6::connectAll()
{
    connect(ui->d7, &QLineEdit::returnPressed, this, &TForm6::onEditingFinished);
    connect(ui->d36_1, &QLineEdit::returnPressed, this, &TForm6::onEditingFinished);
    connect(ui->d11, &QLineEdit::returnPressed, this, &TForm6::onEditingFinished);
    connect(ui->d37_1, &QLineEdit::returnPressed, this, &TForm6::onEditingFinished);
}

void TForm6::onEditingFinished()
{
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("请先建立连接!"));
        return;
    }
    QLineEdit* senderLineEdit = qobject_cast<QLineEdit*>(sender());
    if (senderLineEdit) {
        // 打印对象名称
        qDebug() << "当前的 QLineEdit 对象名称是：" << senderLineEdit->objectName();
        if(senderLineEdit->objectName() == "d36_1" || senderLineEdit->objectName() == "d37_1")
        {
            quint8 addrLow = senderLineEdit->objectName().mid(1).toInt();
            quint16 value = senderLineEdit->text().toFloat() * 10;
            mainwindow->manualWriteOneCMDBuild(static_cast<char>(0), addrLow, value >> 8, value & 0xFF);
        }
        if(senderLineEdit->objectName() == "d7" || senderLineEdit->objectName() == "d11")
        {
            quint8 addrLow = senderLineEdit->objectName().mid(1).toInt();
            quint16 value = senderLineEdit->text().toInt();
            mainwindow->manualWriteOneCMDBuild(static_cast<char>(0), addrLow, value >> 8, value & 0xFF);
        }
    }
}
