#include "tform2.h"
#include "ui_tform2.h"
#include <QMessageBox>
#include "globalparam.h"
#include <QRegularExpression>
TForm2::TForm2(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TForm2)
{
    ui->setupUi(this);
    //设置窗口标志，确保有边框和标题栏、最小化、关闭，最大化
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    setWindowIcon(QIcon(":/icons/images/battery_control_icon.ico"));
    refresh();
}

TForm2::~TForm2()
{
    delete ui;
}

void TForm2::refresh()
{

}

void TForm2::on_lineEdit_returnPressed()
{
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("请先建立连接!"));
        return;
    }
    QString input = ui->lineEdit->text().trimmed();  // 获取并去掉空白字符

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
            buf.append(0x44);
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


void TForm2::on_lineEdit_2_returnPressed()
{
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("请先建立连接!"));
        return;
    }
    QString input = ui->lineEdit_2->text().trimmed();  // 获取并去掉空白字符

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
            buf.append(0x46);
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

