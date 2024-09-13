#include "MyComboBox.h"
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
MyComboBox::MyComboBox(QWidget *parent) : QComboBox(parent)
{
    // 可以在这里初始化其他内容
}

void MyComboBox::showPopup()
{
    // 调用父类的 showPopup 方法，确保下拉框正常弹出
    QComboBox::showPopup();

    refreshPort();
    // 处理下拉框点击事件
    qDebug() << "QComboBox clicked!";
}

void MyComboBox::refreshPort()
{
    //清空combox中已经有的串口名
    clear();
    // 获取系统中所有可用串口
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();

    // 按串口名升序排序
    std::sort(portList.begin(), portList.end(), [](const QSerialPortInfo &a, const QSerialPortInfo &b) {
        return a.portName() < b.portName();
    });

    // 遍历可用串口，将串口名添加到 comboBox中
    for (const QSerialPortInfo &portInfo : portList) {
       addItem(portInfo.portName());
    }
}
