#include "tform3.h"
#include "ui_tform3.h"
#include <QTimer>
#include <QRegularExpression>
#include <mainwindow.h>
#include <QMessageBox>
#include "globalparam.h"
TForm3::TForm3(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TForm3)
{
    ui->setupUi(this);
    //设置窗口标志，确保有边框和标题栏、最小化、关闭，最大化
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    setWindowIcon(QIcon(":/icons/images/battery_control_icon.ico"));
    refresh();
    connectAll();
}

TForm3::~TForm3()
{
    delete ui;
}

void TForm3::refresh()
{
    lineEdits = {ui->d48, ui->d49, ui->d52, ui->d53, ui->d56, ui->d57, ui->d60, ui->d61};
    //K
    ui->d53->setText(QString::number(static_cast<float>(timingDataBuf[53]) / 10000, 'f', 4));
    ui->d61->setText(QString::number(static_cast<float>(timingDataBuf[61]) / 10000, 'f', 4));
    ui->d49->setText(QString::number(static_cast<float>(timingDataBuf[49]) / 10000, 'f', 4));
    ui->d57->setText(QString::number(static_cast<float>(timingDataBuf[57]) / 10000, 'f', 4));
    //B
    ui->d52->setText(QString::number(static_cast<float>(timingDataBuf[52]) / 1000, 'f', 3));
    ui->d60->setText(QString::number(static_cast<float>(timingDataBuf[60]) / 1000, 'f', 3));
    ui->d48->setText(QString::number(static_cast<float>(timingDataBuf[48]) / 1000, 'f', 3));
    ui->d56->setText(QString::number(static_cast<float>(timingDataBuf[56]) / 1000, 'f', 3));

    ui->d21->setText(QString::number(static_cast<float>(timingDataBuf[21]) / 10, 'f', 1));
    ui->d74->setText(QString::number(static_cast<float>(timingDataBuf[74]) / 10, 'f', 1));
    ui->d20->setText(QString::number(static_cast<float>(timingDataBuf[20]) / 10, 'f', 1));
    ui->d73->setText(QString::number(static_cast<float>(timingDataBuf[73]) / 10, 'f', 1));

}

void TForm3::connectAll()
{
    for(int i = 0; i < 8; i++)
    {
        connect(lineEdits[i], &QLineEdit::returnPressed, this, &TForm3::onEditingFinished);
    }
}

void TForm3::on_lineEdit_13_returnPressed()
{
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("请先建立连接!"));
        return;
    }
    QString input = ui->lineEdit_13->text().trimmed();  // 获取并去掉空白字符

    // 定义16进制校验的正则表达式，要求8个16进制字符（0-9, a-f, A-F）
    QRegularExpression hexPattern("^[0-9a-fA-F]{8}$");

    // 校验输入是否匹配
    if (hexPattern.match(input).hasMatch()) {
        // 输入合法，继续处理
        // 将8个字符分成两个部分
        QString highStr = input.mid(0, 4);  // 前4个字符
        QString lowStr = input.mid(4, 4);  // 后4个字符

        // 转换成数字（使用16进制）
        bool ok1, ok2;
        int high = highStr.toInt(&ok1, 16);  // 将第一部分转成整数
        int low = lowStr.toInt(&ok2, 16);  // 将第二部分转成整数

        if (ok1 && ok2) {
            // 转换成功，可以进行进一步处理
            QByteArray buf;
            buf.append(static_cast<char>(0x00));
            buf.append(0x42);
            buf.append(static_cast<char>(0x00));
            buf.append(0x02);
            buf.append(0x04);
            buf.append(low >> 8);
            buf.append(low & 0xFF);
            buf.append(high >> 8);
            buf.append(high & 0xFF);
            mainwindow->manualWriteMultipleCMDBuild(buf);
        } else {
            QMessageBox::warning(this, "错误", "输入转换失败");
        }
    } else {
        // 输入不合法，弹出错误提示
        QMessageBox::warning(this, "错误", "请输入8个合法的16进制字符（0-9, a-f, A-F）");
    }
}

void TForm3::onEditingFinished()
{
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("请先建立连接!"));
        return;
    }
    // 获取触发此信号的对象
    QLineEdit* senderLineEdit = qobject_cast<QLineEdit*>(sender());
    if (senderLineEdit) {
        // 打印对象名称
        qDebug() << "当前的 QLineEdit 对象名称是：" << senderLineEdit->objectName();
        quint8 addrLow = senderLineEdit->objectName().mid(1).toInt();
        quint16 value = 0;
        //K
        if(senderLineEdit->objectName() == "d53" ||
            senderLineEdit->objectName() == "d57" ||
            senderLineEdit->objectName() == "d49" ||
            senderLineEdit->objectName() == "d61")
        {
            value = senderLineEdit->text().toFloat() * 10000;
        }
        //B
        if(senderLineEdit->objectName() == "d52" ||
            senderLineEdit->objectName() == "d48" ||
            senderLineEdit->objectName() == "d56" ||
            senderLineEdit->objectName() == "d60")
        {
            value = senderLineEdit->text().toFloat() * 1000;
        }

        mainwindow->manualWriteOneCMDBuild(static_cast<char>(0), addrLow, value >> 8, value & 0xFF);
    }
}

