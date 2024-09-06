#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tform1.h"
#include "tform2.h"
#include "tform3.h"
#include <QDateTime>
#include <QTimer>
#include "globalparam.h"
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serialPort(new QSerialPort(this))
{
    ui->setupUi(this);
    init();
}

static const uint16_t crc16Table[] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::isCreated(int num)
{
    switch (num) {
    case 1:
        return tform1 != nullptr;
    case 2:
        return tform2 != nullptr;
    case 3:
        return tform3 != nullptr;
    default:
        return false;
    }
}

void MainWindow::timerUpDate()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
}

void MainWindow::init()
{
    //时间显示
    // timer1 = new QTimer(this);
    // connect(timer1, &QTimer::timeout, this, &MainWindow::timerUpDate);
    // timer1->setInterval(1000);
    // timer1->start();
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

void MainWindow::sendPortData()
{
    QByteArray buf(sendDataBuf, sendLength);
    sendSerialData(buf);
    //设置等待时间
    waitMessageRemaingTime = 20;
}

void MainWindow::on_sendTimer_timeout()
{
    if(waitMessageRemaingTime > 0)
    {
        waitMessageRemaingTime--;
    }
    if(dataRefreshRemaingTime > 0)
    {
        dataRefreshRemaingTime--;
    }

    //说明串口空闲，看看有没有手动的命令要下发
    if(waitMessageRemaingTime == 0)
    {
        //说明有手动命令要下发
        if(manualFlag == 1)
        {
            //手动命令下发
            sendPortData();
        }
        //说明没有手动命令要下发，就判断是否到了刷新时间
        else if(dataRefreshRemaingTime <= 0)
        {
            switch (dataRefreshStep) {
            case 1:
                //TODO 数据刷新
                //最后一步要重置刷新时间
                dataRefreshRemaingTime = 20;
                break;
            default:
                break;
            }
        }
    }
}

void MainWindow::on_receiveTimer_timeout()
{
    cacheReceiveData();
    //当缓冲区的消息长度大于messageSize，那说明可能存在一条完整的响应
    while (messageSize > 0 && (receiveEndIndex + 500 - receiveStartIndex) % 500 >= messageSize) {
        //没有匹配到开始
        if(receiveDataBuf[receiveStartIndex] != module || receiveDataBuf[(receiveStartIndex + 1) % 500] != cmd)
        {
            receiveStartIndex = (receiveStartIndex + 1) % 500;
            continue;
        }
        //构建消息
        QByteArray buf(messageSize, 0);
        for (int var = 0; var < messageSize; var++) {
            buf.append(receiveDataBuf[(receiveStartIndex + var) % 500]);
        }
        //判断是否是一个完整的消息
        if(receiveDataCRCCheck(buf))
        {
            dealMessage(buf);
            break;
        }
        //crc校验失败
        else
        {
            receiveStartIndex = (receiveStartIndex + 1) % 500;
            continue;
        }
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    if(!isCreated(1))
    {
        tform1 = new TForm1(this);
        tform1->setAttribute(Qt::WA_DeleteOnClose);
        connect(tform1, &TForm1::destroyed, this, &MainWindow::onTFormDestroyed);
    }
    tform1->show();
}

void MainWindow::onTFormDestroyed(QObject *obj)
{
    if(obj == tform1)
    {
        tform1 = nullptr;
    }
    if(obj == tform2)
    {
        tform2 = nullptr;
    }
    if(obj == tform3)
    {
        tform3 = nullptr;
    }
    // if(obj == tform4)
    // {
    //     tform4 = nullptr;
    // }
    // if(obj == tform5)
    // {
    //     tform5 = nullptr;
    // }
    // if(obj == tform6)
    // {
    //     tform6 = nullptr;
    // }
    // if(obj == tform7)
    // {
    //     tform7 = nullptr;
    // }
    // if(obj == tform8)
    // {
    //     tform8 = nullptr;
    // }

    // if(obj == tform9)
    // {
    //     tform9 = nullptr;
    // }
    // if(obj == tform10)
    // {
    //     tform10 = nullptr;
    // }
    // if(obj == tform11)
    // {
    //     tform11 = nullptr;
    // }
    // if(obj == tform12)
    // {
    //     tform12 = nullptr;
    // }
    // if(obj == tform13)
    // {
    //     tform13 = nullptr;
    // }
    // if(obj == tpcsetting)
    // {
    //     tpcsetting = nullptr;
    // }
}

void MainWindow::on_pushButton_2_clicked()
{
    if(!isCreated(2))
    {
        tform2 = new TForm2(this);
        tform2->setAttribute(Qt::WA_DeleteOnClose);
        connect(tform2, &TForm2::destroyed, this, &MainWindow::onTFormDestroyed);
    }
    tform2->show();
}


void MainWindow::on_pushButton_3_clicked()
{
    if(!isCreated(3))
    {
        tform3 = new TForm3(this);
        tform3->setAttribute(Qt::WA_DeleteOnClose);
        connect(tform3, &TForm3::destroyed, this, &MainWindow::onTFormDestroyed);
    }
    tform3->show();
}


void MainWindow::on_pushButton_6_clicked()
{
    if(ui->comboBox_2->currentIndex() == -1)
    {
        QMessageBox::information(this, tr("提示"),
                                 tr("请选择串口!"));
        return;
    }
    serialPort->setBaudRate(ui->comboBox_3->currentText().toInt());
    serialPort->setPortName(ui->comboBox_2->currentText());
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setParity(QSerialPort::EvenParity);
    //连接失败
    if(!serialPort->open(QIODevice::ReadWrite))
    {
        QMessageBox::information(this, tr("错误"),
                                 tr("无法启动串口通讯！！！"));
        connFlag = 0;
        return;
    }
    //连接成功
    else
    {
        connFlag = 1;
        ui->pushButton_6->setEnabled(false);
        ui->comboBox_2->setEnabled(false);
        ui->comboBox_3->setEnabled(false);
    }
}

void MainWindow::sendSerialData(const QByteArray &data)
{
    //串口未开启
    if(!serialPort->isOpen())
    {
        QMessageBox::critical(this, "错误", "串口未开启!");
        return;
    }
    serialPort->write(data);
}

// 计算Modbus-RTU CRC16的方法，返回高低字节的QByteArray
QByteArray MainWindow::calculateCRCArray(const QByteArray &data, int length) {
    uint16_t crc = 0xFFFF; // 初始化CRC为0xFFFF

    for (int i = 0; i < length; i++) {
        crc ^= static_cast<uint8_t>(data[i]); // 将当前字节异或到CRC低位

        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001; // 如果最低位是1，右移后与0xA001异或
            } else {
                crc = crc >> 1; // 否则只右移
            }
        }
    }
    QByteArray crcArray;
    crcArray.append(static_cast<char>(crc & 0xFF));      // CRC低字节
    crcArray.append(static_cast<char>((crc >> 8) & 0xFF)); // CRC高字节
    return crcArray;
}

void MainWindow::cacheReceiveData()
{
    if(serialPort->isOpen())
    {
        QByteArray data = serialPort->readAll();
        if(data.length() > 0)
        {
            qDebug() << "receive data: " << data;
        }
        for (auto byte : data) {
            receiveDataBuf[receiveEndIndex] = byte;
            receiveEndIndex = (receiveEndIndex + 1) % 500;
        }
    }
}

bool MainWindow::receiveDataCRCCheck(const QByteArray &data)
{
    QByteArray crcResultArray = calculateCRCArray(data, data.length() - 2);
    return crcResultArray[0] == data[data.size() - 2] && crcResultArray[1] == data[data.size() - 1];
}

void MainWindow::dealMessage(const QByteArray &data)
{

}

