#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tform1.h"
#include "tform2.h"
#include "tform3.h"
#include "tform4.h"
#include "tform5.h"
#include "tform6.h"
#include <QDateTime>
#include <QTimer>
#include "globalparam.h"
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>
#include <QSettings>
#include <QDir>
#include <QKeyEvent>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serialPort(new QSerialPort(this))
    , connectStatusLabel(new QLabel(this))
    , runningStatusLabel(new QLabel(this))
{
    ui->setupUi(this);
    mainwindow = this;
    setWindowIcon(QIcon(":/icons/images/battery_control_icon.ico"));
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
    case 4:
        return tform4 != nullptr;
    case 5:
        return tform5 != nullptr;
    case 6:
        return tform6 != nullptr;
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
    connect(sendTimer, &QTimer::timeout, this, &MainWindow::onSendTimerTimeout);
    sendTimer->setInterval(100);
    sendTimer->start();
    //接收数据
    receiveTimer = new QTimer(this);
    connect(receiveTimer, &QTimer::timeout, this, &MainWindow::onReceiveTimerTimeout);
    receiveTimer->setInterval(10);
    receiveTimer->start();
    refreshPort();
    //状态栏
    connectStatusLabel->setMinimumWidth(150);
    ui->statusBar->addWidget(connectStatusLabel);
    connectStatusLabel->setText(connStatus.arg("未连接"));
    connectStatusLabel->setStyleSheet("QLabel { background-color : red; color : white; }");

    runningStatusLabel->setMinimumWidth(150);
    ui->statusBar->addWidget(runningStatusLabel);

    //加载配置文件
    loadConfig();
    // 创建定时器，用于在一段时间后重置按键计数
    resetTimer = new QTimer(this);
    resetTimer->setInterval(2000);  // 0.5秒内按两次空格才算有效
    connect(resetTimer, &QTimer::timeout, this, &MainWindow::resetKeyPressCount);
\
    //初始化模式
    timingDataBuf[3] = -1;
    refresh();
}

void MainWindow::refreshPort()
{
    //清空combox中已经有的串口名
    ui->comboBox_2->clear();
    // 获取系统中所有可用串口
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();

    // 按串口名升序排序
    std::sort(portList.begin(), portList.end(), [](const QSerialPortInfo &a, const QSerialPortInfo &b) {
        return a.portName() < b.portName();
    });

    // 遍历可用串口，将串口名添加到 comboBox中
    for (const QSerialPortInfo &portInfo : portList) {
        ui->comboBox_2->addItem(portInfo.portName());
    }
}

void MainWindow::sendGetAllDataCMD()
{
    QByteArray buf;
    buf.append(MODULE);
    buf.append(READ_CMD);

    //起始地址
    buf.append(static_cast<char>(0x00));
    buf.append(static_cast<char>(0x00));
    //个数
    buf.append(static_cast<char>(0x00));
    buf.append(static_cast<char>(0x41));
    QByteArray crcArray = calculateCRCArray(buf, 6);
    //crC
    buf.append(crcArray[0]);
    buf.append(crcArray[1]);
    sendPortData(buf);
    dataRefreshRemaingTime = DATA_REFRESH_CYCLE;
}

void MainWindow::manualReadCMDBuild(char startHigh, char startLow, char numHigh, char numLow)
{
    if(manualFlag == 1)
    {
        QMessageBox::information(this, "冲突", "当前有其他手动命令在发送, 请稍后再试!");
        return;
    }
    manualSendDataBuf.clear();
    manualFlag = 1;
    manualSendDataBuf.append(MODULE);
    manualSendDataBuf.append(READ_CMD);
    manualSendDataBuf.append(startHigh);
    manualSendDataBuf.append(startLow);
    manualSendDataBuf.append(numHigh);
    manualSendDataBuf.append(numLow);
    QByteArray crcArray = calculateCRCArray(manualSendDataBuf, 6);
    //crC
    manualSendDataBuf.append(crcArray[0]);
    manualSendDataBuf.append(crcArray[1]);
}

void MainWindow::refresh()
{
     qint16 ac = timingDataBuf[22];
    if(timingDataBuf[3] == 0)
    {
        runningStatusLabel->setText(runningStatus.arg("自动模式"));
        runningStatusLabel->setStyleSheet("QLabel { background-color : #4CAF50; color : white; }");
        ui->label_8->setStyleSheet(    "QLabel {"
                                   "    border: none;"
                                   "    border-radius: 15px;"
                                   "    background-color: #4CAF50;"
                                   "}");
    }
    else if(timingDataBuf[3] == 4)
    {
        runningStatusLabel->setText(runningStatus.arg("手动模式"));
        runningStatusLabel->setStyleSheet("QLabel { background-color : #FFA500; color : white; }");
        if(ac < 0)
        {
            ui->label_9->setStyleSheet(    "QLabel {"
                                       "    border: none;"
                                       "    border-radius: 15px;"
                                       "    background-color: #FFA500;"
                                       "}");
        }
        if(ac > 0)
        {
            ui->label_10->setStyleSheet(    "QLabel {"
                                       "    border: none;"
                                       "    border-radius: 15px;"
                                       "    background-color: #FFA500;"
                                       "}");
        }



    }
    else if(timingDataBuf[3] == 1 || timingDataBuf[3] == 5)
    {
        runningStatusLabel->setText(runningStatus.arg("关闭"));
        runningStatusLabel->setStyleSheet("QLabel { background-color : #F44336; color : white; }");
        ui->label_11->setStyleSheet(    "QLabel {"
                                    "    border: none;"
                                    "    border-radius: 15px;"
                                    "    background-color: #F44336;"
                                    "}");
    }
    else
    {
        runningStatusLabel->setText(runningStatus.arg("未知"));
        runningStatusLabel->setStyleSheet("QLabel { background-color : #696969; color : white; }");
    }
    //A->B, A欠压或者B过压
    if(timingDataBuf[2] == 7 && (timingDataBuf[16] >= timingDataBuf[20] || timingDataBuf[15] <= timingDataBuf[21]))
    {
        //自动模式，下发B->A
        if(timingDataBuf[3] == 0)
        {
            setBtoA();
            cycleNum++;
        }
        //手动模式，手动停止
        if(timingDataBuf[3] == 4)
        {
            manualWriteOneCMDBuild(static_cast<char>(0x00), 0x03, static_cast<char>(0x00), 0x05);
        }
    }
    //B->A，B欠压或者A过压
    if(timingDataBuf[2] == 5 && (timingDataBuf[17] >= timingDataBuf[21] || timingDataBuf[14] <= timingDataBuf[20]))
    {
        //自动模式，下发A->B
        if(timingDataBuf[3] == 0)
        {
            setAtoB();
            cycleNum++;
        }
        //手动模式，手动停止
        if(timingDataBuf[3] == 4)
        {
            manualWriteOneCMDBuild(static_cast<char>(0x00), 0x03, static_cast<char>(0x00), 0x05);
        }
    }
    //实时电压
    ui->d20->setText(QString::number(static_cast<float>(timingDataBuf[20]) / 10, 'f', 1));
    ui->d21->setText(QString::number(static_cast<float>(timingDataBuf[21]) / 10, 'f', 1));
    ui->d14->setText(QString::number(static_cast<float>(timingDataBuf[14]) / 10, 'f', 1));
    ui->d15->setText(QString::number(static_cast<float>(timingDataBuf[15]) / 10, 'f', 1));
    ui->d16->setText(QString::number(static_cast<float>(timingDataBuf[16]) / 10, 'f', 1));
    ui->d17->setText(QString::number(static_cast<float>(timingDataBuf[17]) / 10, 'f', 1));
    //电流方向，循环次数刷新
    if(ac < 0)
    {
        ui->label_24->setText(displayInfo.arg("A->B").arg(cycleNum));
    }
    else if(ac > 0)
    {
        ui->label_24->setText(displayInfo.arg("B->A").arg(cycleNum));
    }
    else
    {
        ui->label_24->setText(displayInfo.arg("无").arg(cycleNum));
    }
}

void MainWindow::refresh(int num)
{
    switch(num)
    {
    case 1:
        if(isCreated(1))
        {
            tform1->refresh();
        }
        break;
    case 2:
        if(isCreated(2))
        {
            tform2->refresh();
        }
        break;
    case 3:
        if(isCreated(3))
        {
            tform3->refresh();
        }
        break;
    case 4:
        if(isCreated(4))
        {
            tform4->refresh();
        }
        break;
    case 5:
        if(isCreated(5))
        {
            tform5->refresh();
        }
        break;
    case 6:
        if(isCreated(6))
        {
            tform6->refresh();
        }
        break;
    }
}

void MainWindow::refreshAll()
{
    refresh();
    for(int i = 1; i <= 6; i++)
    {
        refresh(i);
    }
}

void MainWindow::loadConfig()
{
    QString configFileName = "config.ini";
    QString iniFilePath = QDir::currentPath() + "/" + configFileName;
    QFile configFile(iniFilePath);
    QSettings settings(iniFilePath, QSettings::IniFormat);
    settings.beginGroup("CONFIG");
    //不存在则初始化
    if(!configFile.exists())
    {
        settings.setValue("CYCLE_NUM", 0);
    }
    //加载配置
    else
    {
        cycleNum = settings.value("CYCLE_NUM").toInt();
    }
    ui->label_24->setText(displayInfo.arg("无").arg(cycleNum));
}

void MainWindow::manualWriteOneCMDBuild(char startHigh, char startLow, char valueHigh, char valueLow, quint8 secFlag)
{
    if(manualFlag == 1)
    {
        QMessageBox::information(this, "冲突", "当前有其他手动命令在发送, 请稍后再试!");
        return;
    }
    manualSendDataBuf.clear();
    if(secFlag == 0)
    {
       manualFlag = 1;
    }
    else
    {
        manualFlag = 2;
    }
    manualSendDataBuf.append(MODULE);
    manualSendDataBuf.append(WRITE_ONE_CMD);
    manualSendDataBuf.append(startHigh);
    manualSendDataBuf.append(startLow);
    manualSendDataBuf.append(valueHigh);
    manualSendDataBuf.append(valueLow);
    QByteArray crcArray = calculateCRCArray(manualSendDataBuf, 6);
    //crC
    manualSendDataBuf.append(crcArray[0]);
    manualSendDataBuf.append(crcArray[1]);
}

void MainWindow::manualWriteMultipleCMDBuild(QByteArray buf, quint8 secFlag)
{

    if(manualFlag == 1)
    {
        QMessageBox::information(this, "冲突", "当前有其他手动命令在发送, 请稍后再试!");
        return;
    }
    manualSendDataBuf.clear();
    if(secFlag == 0)
    {
        manualFlag = 1;
    }
    else
    {
        manualFlag = 2;
    }
    manualSendDataBuf.append(MODULE);
    manualSendDataBuf.append(WRITE_MULTIPLE_CMD);
    manualSendDataBuf.append(buf);
    QByteArray crcArray = calculateCRCArray(manualSendDataBuf, 2 + buf.size());
    //crC
    manualSendDataBuf.append(crcArray[0]);
    manualSendDataBuf.append(crcArray[1]);
}

void MainWindow::setAtoB()
{
    //设置A的输出电压为欠压的0.9,B的输出电压为过压的1.1
    int aoutputv = timingDataBuf[16] * 0.9;
    int boutputv = timingDataBuf[15] * 1.1;
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("先建立连接!"));
        return;
    }
    QByteArray buf;
        //起始地址
    buf.append(static_cast<char>(0x00));
    buf.append(0x0C);
    //字数
    buf.append(static_cast<char>(0x00));
    buf.append(0x02);
    //字节数
    buf.append(0x04);
    //写入值A
    buf.append(static_cast<char>(aoutputv >> 8));
    buf.append(static_cast<char>(aoutputv & 0xFF));
    //写入值B
    buf.append(static_cast<char>(boutputv >> 8));
    buf.append(static_cast<char>(boutputv & 0xFF));
    manualWriteMultipleCMDBuild(buf);
}

void MainWindow::setBtoA()
{
    //设置A的输出电压为过压的1.1,B的输出电压为欠压的0.9
    int aoutputv = timingDataBuf[14] * 1.1;
    int boutputv = timingDataBuf[17] * 0.9;
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("先建立连接!"));
        return;
    }
    QByteArray buf;
        //起始地址
    buf.append(static_cast<char>(0x00));
    buf.append(0x0C);
    //字数
    buf.append(static_cast<char>(0x00));
    buf.append(0x02);
    //字节数
    buf.append(0x04);
    //写入值A
    buf.append(static_cast<char>(aoutputv >> 8));
    buf.append(static_cast<char>(aoutputv & 0xFF));
    //写入值B
    buf.append(static_cast<char>(boutputv >> 8));
    buf.append(static_cast<char>(boutputv & 0xFF));
    manualWriteMultipleCMDBuild(buf);
}

void MainWindow::secondCMDSend()
{
    switch (secCmdType) {
    case 1:
        setAtoB();
        break;
    case 2:
        setBtoA();
    default:
        break;
    }
}

void MainWindow::sendPortData(QByteArray data)
{
    if(data == nullptr)
    {
        sendSerialData(manualSendDataBuf);
    }
    else
    {
        sendSerialData(data);
    }
    //设置等待时间
    waitMessageRemaingTime = 20;
}

void MainWindow::onSendTimerTimeout()
{
    if(connFlag == 0)
    {
        return;
    }
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
        //说明有二次手动命令
        if(manualFlag == 2)
        {
            sendPortData();
            manualFlag = 0;
            secondCMDSend();
        }
        //说明有手动命令要下发
        else if(manualFlag == 1)
        {
            //手动命令下发
            sendPortData();
            manualFlag = 0;
        }
        //说明没有手动命令要下发，就判断是否到了刷新时间
        else if(dataRefreshRemaingTime <= 0)
        {
            sendGetAllDataCMD();
        }
    }
}

void MainWindow::onReceiveTimerTimeout()
{

    if(connFlag == 0)
    {
        return;
    }

    cacheReceiveData();
    //当缓冲区的消息长度大于messageSize，那说明可能存在一条完整的响应
    while ((receiveEndIndex + 500 - receiveStartIndex) % 500 >= 6) {
        int module = static_cast<uint8_t>(receiveDataBuf[receiveStartIndex]);
        int cmd = static_cast<uint8_t>(receiveDataBuf[(receiveStartIndex + 1) % 500]);
        //没有匹配到开始
        if(module != MODULE || (cmd != 3 && cmd != 6 && cmd != 16))
        {
            //更新开始点
            receiveStartIndex = (receiveStartIndex + 1) % 500;
            continue;
        }
        //匹配到开始,再匹配下长度是否符合
        int messageSize = 0;
        if(cmd == 3)
        {
            messageSize = static_cast<uint8_t>(receiveDataBuf[(receiveStartIndex + 2) % 500]) + 5;
        }
        if(cmd == 6)
        {
            messageSize = 8;
        }
        if(cmd == 16)
        {
            messageSize = 16;
        }
        if((receiveEndIndex + 500 - receiveStartIndex) % 500 < messageSize){
            //消息还没接收完整，等下一次定时去接,不更新开始点
            break;
        }
        //构建消息
        QByteArray buf;
        for (int var = 0; var < messageSize; var++) {
            buf.append(receiveDataBuf[(receiveStartIndex + var) % 500]);
        }
        //判断是否是一个完整的消息
        if(receiveDataCRCCheck(buf))
        {
            //首先更新接收缓冲区的开始坐标
            if(isCreated(1))
            {
                tform1->displayInfo("串口上传上来且验证通过的一条消息：" + buf.toHex());
            }
            receiveStartIndex = (receiveStartIndex + messageSize) % 500;
            dealMessage(buf);
            break;
        }
        //crc校验失败
        else
        {
            //更新开始点
            receiveStartIndex = (receiveStartIndex + 1) % 500;
            continue;
        }
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    if(!isCreated(5))
    {
        tform5 = new TForm5(this);
        tform5->setAttribute(Qt::WA_DeleteOnClose);
        connect(tform5, &TForm5::destroyed, this, &MainWindow::onTFormDestroyed);
    }
    tform5->show();
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
    if(obj == tform4)
    {
        tform4 = nullptr;
    }
    if(obj == tform5)
    {
        tform5 = nullptr;
    }
    if(obj == tform6)
    {
        tform6 = nullptr;
    }
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

void MainWindow::sendSerialData(const QByteArray &data)
{
    //串口未开启
    if(!serialPort->isOpen())
    {
        QMessageBox::critical(this, "错误", "串口未开启!");
        return;
    }
    if(isCreated(1))
    {
        tform1->displayInfo("上位机发送的串口数据：" + data.toHex());
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
        for (auto byte : data) {
            receiveDataBuf[receiveEndIndex] = byte;
            receiveEndIndex = (receiveEndIndex + 1) % 500;
        }
    }
}

bool MainWindow::receiveDataCRCCheck(const QByteArray &data)
{
    QByteArray crcResultArray = calculateCRCArray(data, data.length() - 2);
    if(isCreated(1))
    {
        tform1->displayInfo("待校验的下位机数据：" + data.toHex());
    }
    qDebug() << "cal crc low: " << crcResultArray[0];
    qDebug() << "cal crc high: " << crcResultArray[1];
    qDebug() << "data[data.size() - 2]: " << data[data.size() - 2];
    qDebug() << "data[data.size() - 1]: " << data[data.size() - 1];
    return crcResultArray[0] == data[data.size() - 2] && crcResultArray[1] == data[data.size() - 1];
}

void MainWindow::dealMessage(const QByteArray &data)
{
    //查询所有的命令
    if(data[1] == READ_CMD)
    {
        //更新缓存
        QByteArray dataBuf = data.mid(3, data.size() - 5);
        qDebug() << "dataBuf: " << dataBuf.toHex();
        for(int i = 0; i < dataBuf.size(); i = i + 2)
        {
            timingDataBuf[i / 2] = static_cast<quint8>(dataBuf.at(i)) * 256 + static_cast<quint8>(dataBuf.at(i + 1));
            if(i == 40)
            {
                qDebug() << "40 : " << static_cast<quint8>(dataBuf.at(i));
                qDebug() << "41 : " << static_cast<quint8>(dataBuf.at(i + 1));
                qDebug() << "第二十： " << timingDataBuf[i / 2];
            }
        }
        refreshAll();
    }
    if(data[1] == WRITE_ONE_CMD)
    {
        //写入命令返回，暂时没有处理逻辑
    }
    if(data[1] == WRITE_MULTIPLE_CMD)
    {
        //多个写入命令返回，暂时没有处理逻辑
    }
}


void MainWindow::on_connBtn_clicked()
{
    if(ui->connBtn->text() == "建立连接")
    {
        if(ui->comboBox_2->currentIndex() == -1)
        {
            QMessageBox::information(this, tr("提示"),
                                     tr("请选择串口!"));
            return;
        }
        serialPort->setBaudRate(BR);
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
            connectStatusLabel->setText(connStatus.arg("未连接"));
            connectStatusLabel->setStyleSheet("QLabel { background-color : red; color : white; }");
            return;
        }
        //连接成功
        else
        {
            connFlag = 1;
            ui->comboBox_2->setEnabled(false);
            connectStatusLabel->setText(connStatus.arg("已连接"));
            connectStatusLabel->setStyleSheet("QLabel { background-color : green; color : white; }");
            ui->connBtn->setText("断开连接");
        }
    }
    else if(ui->connBtn->text() == "断开连接")
    {
        if(!serialPort->isOpen())
        {
            return;
        }
        serialPort->close();
        connFlag = 0;
        ui->comboBox_2->setEnabled(true);
        connectStatusLabel->setText(connStatus.arg("未连接"));
        connectStatusLabel->setStyleSheet("QLabel { background-color : red; color : white; }");
        ui->connBtn->setText("建立连接");
    }
}


void MainWindow::on_pushButton_4_clicked()
{
    if(!isCreated(4))
    {
        tform4 = new TForm4(this);
        tform4->setAttribute(Qt::WA_DeleteOnClose);
        connect(tform4, &TForm4::destroyed, this, &MainWindow::onTFormDestroyed);
    }
    tform4->show();
}


void MainWindow::on_pushButton_6_clicked()
{
    if(!isCreated(6))
    {
        tform6 = new TForm6(this);
        tform6->setAttribute(Qt::WA_DeleteOnClose);
        connect(tform6, &TForm6::destroyed, this, &MainWindow::onTFormDestroyed);
    }
    tform6->show();
}


//自动模式
void MainWindow::on_pushButton_clicked()
{
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("先建立连接!"));
        return;
    }
    if(timingDataBuf[3] == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("当前已经是自动模式!"));
        return;
    }
    //判断初始化流向
    if((timingDataBuf[20] - timingDataBuf[16]) * (timingDataBuf[15] - timingDataBuf[17]) > (timingDataBuf[21] - timingDataBuf[17]) * (timingDataBuf[14] - timingDataBuf[16]))
    {
        //A更接近过压
        secCmdType = 1;
    }
    else
    {
        //B更接近过压
        secCmdType = 2;
    }
    //先发自动模式，
    manualWriteOneCMDBuild(static_cast<char>(0x00), static_cast<char>(0x03), static_cast<char>(0x00), static_cast<char>(0x00), 1);

}

//手动模式A->B
void MainWindow::on_pushButton_7_clicked()
{
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("请先建立连接!"));
        return;
    }
    if(timingDataBuf[3] == 4 && timingDataBuf[2] == 7)
    {
        QMessageBox::information(this, tr("提示"), tr("当前已经是手动模式A->B!"));
        return;
    }
    if(timingDataBuf[16] >= timingDataBuf[20])
    {
        QMessageBox::information(this, tr("提示"), tr("A侧电流欠压，不满足下发条件"));
        return;
    }
    if(timingDataBuf[15] <= timingDataBuf[21])
    {
        QMessageBox::information(this, tr("提示"), tr("B侧电流过压，不满足下发条件"));
        return;
    }
    secCmdType = 1;
    //手动模式，
    manualWriteOneCMDBuild(static_cast<char>(0x00), static_cast<char>(0x03), static_cast<char>(0x00), static_cast<char>(0x04), 1);
}

//手动B->A
void MainWindow::on_pushButton_8_clicked()
{
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("请先建立连接!"));
        return;
    }
    if(timingDataBuf[3] == 4 && timingDataBuf[2] == 7)
    {
        QMessageBox::information(this, tr("提示"), tr("当前已经是手动模式A->B!"));
        return;
    }
    if(timingDataBuf[16] >= timingDataBuf[20])
    {
        QMessageBox::information(this, tr("提示"), tr("A侧电流欠压，不满足下发条件"));
        return;
    }
    if(timingDataBuf[15] <= timingDataBuf[21])
    {
        QMessageBox::information(this, tr("提示"), tr("B侧电流过压，不满足下发条件"));
        return;
    }
    secCmdType = 2;
    //手动模式，
     manualWriteOneCMDBuild(static_cast<char>(0x00), static_cast<char>(0x03), static_cast<char>(0x00), static_cast<char>(0x04), 1);
}

//手动关闭
void MainWindow::on_pushButton_9_clicked()
{
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("请先建立连接!"));
        return;
    }
    if(timingDataBuf[3] == 5)
    {
        QMessageBox::information(this, tr("提示"), tr("当前已经是关闭状态!"));
        return;
    }
    manualWriteOneCMDBuild(static_cast<char>(0x00), 0x03, static_cast<char>(0x00), 0x05);
}

void MainWindow::resetKeyPressCount()
{
    spaceKeyPressCount = 0;
    resetTimer->stop();  // 停止定时器
}

void MainWindow::onEditingFinished()
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
        quint16 value = senderLineEdit->text().toFloat() * 10;
        mainwindow->manualWriteOneCMDBuild(static_cast<char>(0), addrLow, value >> 8, value & 0xFF);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return) {
        spaceKeyPressCount++;

        if (spaceKeyPressCount == 1) {
            // 第一次按空格，启动定时器
            resetTimer->start();
        } else if (spaceKeyPressCount == 2) {
            // 第二次按空格，弹出隐藏界面
            if(!isCreated(1))
            {
                tform1 = new TForm1(this);
                tform1->setAttribute(Qt::WA_DeleteOnClose);
                connect(tform1, &TForm1::destroyed, this, &MainWindow::onTFormDestroyed);
            }
            tform1->show();
            resetTimer->stop();
            spaceKeyPressCount = 0;  // 重置计数
        }
    } else {
        // 非空格键时重置计数
        spaceKeyPressCount = 0;
    }

    QMainWindow::keyPressEvent(event);  // 保留默认的按键处理
}
