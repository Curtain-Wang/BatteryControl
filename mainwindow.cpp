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
    , companyNameLabel(new QLabel(this))
    , companyImageLabel(new QLabel(this))
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
    //发送数据
    sendTimer = new QTimer(this);
    connect(sendTimer, &QTimer::timeout, this, &MainWindow::onSendTimerTimeout);
    sendTimer->setInterval(500);
    sendTimer->start();
    //接收数据
    receiveTimer = new QTimer(this);
    connect(receiveTimer, &QTimer::timeout, this, &MainWindow::onReceiveTimerTimeout);
    receiveTimer->setInterval(100);
    receiveTimer->start();
    refreshPort();
    //状态栏
    connectStatusLabel->setMinimumWidth(150);
    ui->statusBar->addWidget(connectStatusLabel);
    connectStatusLabel->setText(connStatus.arg("未连接"));
    connectStatusLabel->setStyleSheet("QLabel { background-color : red; color : white; }");
    ui->statusBar->addWidget(connectStatusLabel);
    runningStatusLabel->setMinimumWidth(150);
    ui->statusBar->addWidget(runningStatusLabel);
    QPixmap pixmap(":/icons/images/companyName.png");  // 加载图片
    companyImageLabel->setPixmap(pixmap);
    companyImageLabel->setFixedSize(80, 20);
    companyImageLabel->setScaledContents(true);
    ui->statusBar->addPermanentWidget(companyImageLabel);
    companyNameLabel->setText(companyName);
    companyNameLabel->setMinimumWidth(350);
    ui->statusBar->addPermanentWidget(companyNameLabel);

    //加载配置文件
    loadConfig();
    // 创建定时器，用于在一段时间后重置按键计数
    resetTimer = new QTimer(this);
    resetTimer->setInterval(2000);  // 2秒内按两次空格才算有效
    connect(resetTimer, &QTimer::timeout, this, &MainWindow::resetKeyPressCount);

    //初始化模式
    timingDataBuf[3] = -1;
    //充电时间计数
    chargeTimeCountTimer = new QTimer(this);
    chargeTimeCountTimer->setInterval(1000);
    connect(chargeTimeCountTimer, &QTimer::timeout, this, &MainWindow::chargeTimeCountAdd);
    refresh();
    staticCanDataInit();
    ui->pushButton_2->setVisible(false);
}

void MainWindow::refreshPort()
{
    // //清空combox中已经有的串口名
    // ui->comboBox_2->clear();
    // // 获取系统中所有可用串口
    // QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();

    // // 按串口名升序排序
    // std::sort(portList.begin(), portList.end(), [](const QSerialPortInfo &a, const QSerialPortInfo &b) {
    //     return a.portName() < b.portName();
    // });

    // // 遍历可用串口，将串口名添加到 comboBox中
    // for (const QSerialPortInfo &portInfo : portList) {
    //     ui->comboBox_2->addItem(portInfo.portName());
    // }
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
    refreshRunningStatusInfo();
    //翻转电压初始化
    if(ATurnHighV == 0 && timingDataBuf[14] != 0)
    {
        ATurnHighV = timingDataBuf[14] * 0.9;
        ATurnLowV = timingDataBuf[16] * 1.1;
        BTurnHighV = timingDataBuf[15] * 0.9;
        BTurnLowV = timingDataBuf[17] * 1.1;
    }
    //工作模式
    quint8 workMode = (timingDataBuf[2] & 0x1f);
    if(workMode != ui->cbx_workingmode->currentIndex() * 2 + 1)
    {
        // 禁用信号触发
        ui->cbx_workingmode->blockSignals(true);

        // 修改 currentIndex 时不会触发信号
        ui->cbx_workingmode->setCurrentIndex(workMode / 2);

        // 重新启用信号触发
        ui->cbx_workingmode->blockSignals(false);
    }

    //实时电压
    ui->d20->setText(QString::number(static_cast<float>(timingDataBuf[20]) / 10, 'f', 1));
    ui->d21->setText(QString::number(static_cast<float>(timingDataBuf[21]) / 10, 'f', 1));
    //实时电流
    if(deviceType == 0 || deviceType == 1)
    {
        qint16 value = timingDataBuf[22];
        ui->d22->setText(QString::number(static_cast<float>(value) / 100, 'f', 2));
        value = timingDataBuf[23];
        ui->d23->setText(QString::number(static_cast<float>(value) / 100, 'f', 2));
    }
    else
    {
        qint16 value = timingDataBuf[22];
        ui->d22->setText(QString::number(static_cast<float>(value) / 10, 'f', 1));
        value = timingDataBuf[23];
        ui->d23->setText(QString::number(static_cast<float>(value) / 10, 'f', 1));
    }
    qint16 ac = timingDataBuf[22];
    //手动/自动模式
    if(timingDataBuf[3] == 0 || timingDataBuf[3] == 4)
    {
        if(ac < 0)
        {
            QPixmap pixmapArrow(":/icons/images/right.png");  // 加载图片
            if(!chargeTimeCountTimer->isActive())
            {
                chargeTimeCountTimer->start();
            }
        }
        else if(ac > 0)
        {
            QPixmap pixmapArrow(":/icons/images/left.png");  // 加载图片
            if(!chargeTimeCountTimer->isActive())
            {
                chargeTimeCountTimer->start();
            }
        }
        else
        {
            QPixmap pixmapArrow(":/icons/images/x.png");  // 加载图片
            if(chargeTimeCountTimer->isActive())
            {
                chargeTimeCountTimer->stop();
            }
        }
    }
    else
    {
        QPixmap pixmapArrow(":/icons/images/x.png");  // 加载图片
        if(chargeTimeCountTimer->isActive())
        {
            chargeTimeCountTimer->stop();
        }
    }
    // //A->B, A实时电压小于翻转低压或者B大于翻转高压
    // if(ac < 0 && (timingDataBuf[20] <= ATurnLowV || timingDataBuf[21] >= BTurnHighV))
    // {
    //     //翻转充电时间重置
    //     chargeTime = 0;
    //     //自动模式，下发B->A
    //     if(timingDataBuf[3] == 0)
    //     {
    //         setBtoA();
    //         cycleNum++;
    //     }
    //     //手动模式，手动停止
    //     if(timingDataBuf[3] == 4)
    //     {
    //         manualWriteOneCMDBuild(static_cast<char>(0x00), 0x03, static_cast<char>(0x00), 0x05);
    //     }
    // }
    // //B->A，B实时电压小于翻转低压或者A大于翻转高压
    // if(ac > 0 && (timingDataBuf[21] <= BTurnLowV || timingDataBuf[20] >= ATurnHighV))
    // {
    //     //翻转充电时间重置
    //     chargeTime = 0;
    //     //自动模式，下发A->B
    //     if(timingDataBuf[3] == 0)
    //     {
    //         setAtoB();
    //         cycleNum++;
    //     }
    //     //手动模式，手动停止
    //     if(timingDataBuf[3] == 4)
    //     {
    //         manualWriteOneCMDBuild(static_cast<char>(0x00), 0x03, static_cast<char>(0x00), 0x05);
    //     }
    // }
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

void MainWindow::refreshRunningStatusInfo()
{
    if(timingDataBuf[3] == 0)
    {
        runningStatusLabel->setText(runningStatus.arg("自动模式"));
        runningStatusLabel->setStyleSheet("QLabel { background-color : #4CAF50; color : white; }");
        ui->label_8->setStyleSheet(    "QLabel {"
                                   "    border: none;"
                                   "    border-radius: 15px;"
                                   "    background-color: #4CAF50;"
                                   "}");
        ui->label_9->setStyleSheet(    "QLabel {"
                                   "    border: none;"
                                   "    border-radius: 15px;"
                                   "    background-color: gray;"
                                   "}");
        ui->label_11->setStyleSheet(    "QLabel {"
                                    "    border: none;"
                                    "    border-radius: 15px;"
                                    "    background-color: gray;"
                                    "}");
    }
    else if(timingDataBuf[3] == 4)
    {
        runningStatusLabel->setText(runningStatus.arg("手动模式"));
        runningStatusLabel->setStyleSheet("QLabel { background-color : #FFA500; color : white; }");
        ui->label_8->setStyleSheet(    "QLabel {"
                                   "    border: none;"
                                   "    border-radius: 15px;"
                                   "    background-color: gray;"
                                   "}");
        ui->label_9->setStyleSheet(    "QLabel {"
                                   "    border: none;"
                                   "    border-radius: 15px;"
                                   "    background-color: #FFA500;"
                                   "}");
        ui->label_11->setStyleSheet(    "QLabel {"
                                    "    border: none;"
                                    "    border-radius: 15px;"
                                    "    background-color: gray;"
                                    "}");



    }
    else if(timingDataBuf[3] == 1 || timingDataBuf[3] == 5)
    {
        runningStatusLabel->setText(runningStatus.arg("关机"));
        runningStatusLabel->setStyleSheet("QLabel { background-color : #F44336; color : white; }");
        ui->label_8->setStyleSheet(    "QLabel {"
                                   "    border: none;"
                                   "    border-radius: 15px;"
                                   "    background-color: gray;"
                                   "}");
        ui->label_9->setStyleSheet(    "QLabel {"
                                   "    border: none;"
                                   "    border-radius: 15px;"
                                   "    background-color: gray;"
                                   "}");
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
        ui->label_8->setStyleSheet(    "QLabel {"
                                   "    border: none;"
                                   "    border-radius: 15px;"
                                   "    background-color: gray;"
                                   "}");
        ui->label_9->setStyleSheet(    "QLabel {"
                                   "    border: none;"
                                   "    border-radius: 15px;"
                                   "    background-color: gray;"
                                   "}");
        ui->label_11->setStyleSheet(    "QLabel {"
                                    "    border: none;"
                                    "    border-radius: 15px;"
                                    "    background-color: gray;"
                                    "}");
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
    //加载配置
    if(configFile.exists())
    {
        cycleNum = settings.value("CYCLE_NUM").toInt();
        ATurnHighV = settings.value("A_TURN_HIGH_V").toInt();
        ATurnLowV = settings.value("A_TURN_LOW_V").toInt();
        BTurnHighV = settings.value("B_TURN_HIGH_V").toInt();
        BTurnLowV = settings.value("B_TURN_LOW_V").toInt();
    }
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
    //设置A的输出电压翻转低压,B的输出电压为翻转高压
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("先建立连接!"));
        return;
    }
    // QByteArray buf;
    //     //起始地址
    // buf.append(static_cast<char>(0x00));
    // buf.append(0x0C);
    // //字数
    // buf.append(static_cast<char>(0x00));
    // buf.append(0x02);
    // //字节数
    // buf.append(0x04);
    // //写入值A
    // buf.append(static_cast<char>(ATurnLowV >> 8));
    // buf.append(static_cast<char>(ATurnLowV & 0xFF));
    // //写入值B
    // buf.append(static_cast<char>(BTurnHighV >> 8));
    // buf.append(static_cast<char>(BTurnHighV & 0xFF));
    // manualWriteMultipleCMDBuild(buf);
    //设置低压侧输出电压、高压侧输出电压
    writeCanData(0x0c, ATurnLowV & 0xFF, ATurnLowV >> 8);
    writeCanData(0x0d, BTurnHighV & 0xFF, BTurnHighV >> 8);
}

void MainWindow::setBtoA()
{
    //设置A翻转高压,B翻转低压
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("先建立连接!"));
        return;
    }
    // QByteArray buf;
    //     //起始地址
    // buf.append(static_cast<char>(0x00));
    // buf.append(0x0C);
    // //字数
    // buf.append(static_cast<char>(0x00));
    // buf.append(0x02);
    // //字节数
    // buf.append(0x04);
    // //写入值A
    // buf.append(static_cast<char>(ATurnHighV >> 8));
    // buf.append(static_cast<char>(ATurnHighV & 0xFF));
    // //写入值B
    // buf.append(static_cast<char>(BTurnLowV >> 8));
    // buf.append(static_cast<char>(BTurnLowV & 0xFF));
    // manualWriteMultipleCMDBuild(buf);
    writeCanData(0x0c, ATurnHighV & 0xFF, ATurnHighV >> 8);
    writeCanData(0x0d, BTurnLowV & 0xFF, BTurnLowV >> 8);
}

void MainWindow::secondCMDSend()
{
    // switch (secCmdType) {
    // case 1:
    //     setAtoB();
    //     break;
    // case 2:
    //     setBtoA();
    // default:
    //     break;
    // }
}

// 初始化 CAN
bool MainWindow::initCAN() {
    // 打开 USB-CAN 设备
    dhandle = ZCAN_OpenDevice(ZCAN_USBCAN2, 0, 0);
    if (INVALID_DEVICE_HANDLE == dhandle)
    {
        qDebug() << "打开设备失败，错误信息：";
        return false;
    }

    property = GetIProperty(dhandle);
    if (NULL == property)
    {
        qDebug() << "属性指针为空";
        return false;
    }

    if (ZCAN_SetValue(dhandle , "1/baud_rate", "250000") != STATUS_OK)
    {
        qDebug() << "波特率设置失败";
        return false;
    }

    ZCAN_CHANNEL_INIT_CONFIG cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.can_type = TYPE_CAN;
    cfg.can.filter = 0;
    cfg.can.mode = 0x0;
    cfg.can.acc_code = 0x0;
    cfg.can.acc_mask = 0xFFFFFFFF;

    //初始化CAN通道
    chHandle = ZCAN_InitCAN(dhandle, 0, &cfg);
    if (INVALID_CHANNEL_HANDLE == chHandle) {
        qDebug() << "初始化通道失败";
        ReleaseIProperty(property);
        ZCAN_CloseDevice(dhandle);
        return false;
    }
    // 启动 CAN 通道
    if (ZCAN_StartCAN(chHandle) != STATUS_OK) {
        qDebug() << "启动通道失败";
        ReleaseIProperty(property);
        ZCAN_CloseDevice(dhandle);
        return false;
    }
    return true;
}

void MainWindow::closeCAN()
{
    ReleaseIProperty(property);
    ZCAN_CloseDevice(dhandle);
}

// 根据给定的模块号、命令、帧序号等信息生成 CAN ID
quint32 MainWindow::generateCANId(quint8 module_id, quint8 command, quint8 frame_sequence, quint8 module_number) {
    uint32_t can_id = 0;
    can_id |= (module_id & 0xFF);                   // 0-7位模块号
    can_id |= ((command & 0xF) << 8);               // 8-11位命令
    can_id |= (0x3 << 12);                          // 12-15位固定值0x3
    can_id |= ((frame_sequence & 0xF) << 16);       // 16-19位帧序号
    can_id |= (0xE << 20);                          // 20-23位固定值0xe
    can_id |= (0x8 << 24);                          // 24-27位固定值0x8
    can_id |= (0x1 << 28);                          // 28位固定值0x1
    return can_id;
}

// 读取数据（接收数据帧）
void MainWindow::receiveCANData()
{
    // 确保缓冲区有数据
    UINT receiveNum = ZCAN_GetReceiveNum(chHandle, 0);  // 0 表示CAN类型
    if (receiveNum == 0) {
        return;
    }

    ZCAN_Receive_Data receiveData[10]; // 假设最多接收10条数据
    int actualReceive = ZCAN_Receive(chHandle, receiveData, 10, 100); // 设置等待超时100ms
    if (actualReceive <= 0) {
        qDebug() << "接收数据失败或超时";
        return;
    }

    // 遍历接收到的数据并解析
    for (int i = 0; i < actualReceive; ++i) {
        can_frame frame = receiveData[i].frame;
        decodeCANData(frame);
    }
}

// 发送数据（发送数据帧）
bool MainWindow::sendCANData(quint32 canId, uint8_t data[8]) {
    ZCAN_Transmit_Data frame;
    memset(&frame, 0, sizeof(frame));

    // 生成 CAN ID
    frame.frame.can_id = (canId & 0x1FFFFF00) | moduleId | (1 << 31);
    frame.frame.can_dlc = 8;  // 数据长度，最多 8 字节

    // 填充数据
    memcpy(frame.frame.data, data, 8);
    qDebug() << "发送数据：";
    for(qint8 i = 0; i < 8; i++)
    {
        qDebug () << QString::number(data[i], 16);
    }

    // 发送数据
    if (ZCAN_Transmit(chHandle, &frame, 1) != 1) {
        qDebug() << "发送数据失败，帧ID：0x" << QString::number(frame.frame.can_id, 16).toUpper();
        return false;
    }
    qDebug() << "发送数据成功，帧 ID: 0x" << QString::number(frame.frame.can_id, 16).toUpper();
    return true;
}

void MainWindow::decodeCANData(can_frame frame)
{
    qDebug() << "接收到数据帧，ID: " << QString::number(frame.can_id, 16);
    // 检查帧ID是否符合我们定义的格式
    moduleId = frame.can_id & 0xFF; // 0-7位为模块号
    qDebug() << "模块号: " << moduleId;
    // DCDC传过来的数据 (0x5)
    // if (command != 0x5) {
    //     qDebug() << "收到脏数据, 帧ID: 0x" << QString::number(frame.can_id, 16).toUpper();
    //     return;
    // }
    //这里只是为了与缓存的key匹配
    frame.can_id = (frame.can_id & 0x1FFFFF00) | 1;
    qDebug() << "转换后的CANID: " << QString::number(frame.can_id, 16);
    canFrameHash.insert(frame.can_id, frame);
    switch(frame.can_id)
    {
    case 0x18E03501:
        frame.data[0];//优先控制帧标志
        frame.data[1];//DC/DC启动控制
        break;
    case 0x18E13501:
        //低压侧实时电压
        timingDataBuf[20] = static_cast<quint8>(frame.data[0]) + static_cast<quint8>(frame.data[1]) * 256;
        //高压侧实时电压
        timingDataBuf[21] = static_cast<quint8>(frame.data[2]) + static_cast<quint8>(frame.data[3]) * 256;
        //低压侧实时工作电流
        timingDataBuf[22] = static_cast<quint8>(frame.data[4]) + static_cast<quint8>(frame.data[5]) * 256;
        //高压侧实时工作电流
        timingDataBuf[23] = static_cast<quint8>(frame.data[6]) + static_cast<quint8>(frame.data[7]) * 256;
        break;
    case 0x18E23501:
        //低压侧实时输出功率
        timingDataBuf[34] = static_cast<quint8>(frame.data[0]) + static_cast<quint8>(frame.data[1]) * 256;
        //高压侧实时输出功率
        timingDataBuf[35] = static_cast<quint8>(frame.data[2]) + static_cast<quint8>(frame.data[3]) * 256;
        //散热器温度
        timingDataBuf[24] = static_cast<quint8>(frame.data[4]) + static_cast<quint8>(frame.data[5]) * 256;
        //故障寄存器
        timingDataBuf[5] = static_cast<quint8>(frame.data[6]) + static_cast<quint8>(frame.data[7]) * 256;
        break;
    case 0x18E33501:
        //模块号
        timingDataBuf[0] = static_cast<quint8>(frame.data[0]);
        timingDataBuf[2] = static_cast<quint8>(frame.data[2]);
        //停止变换控制字
        timingDataBuf[3] = static_cast<quint8>(frame.data[4]);
        //严格单向选择寄存器
        timingDataBuf[7] = static_cast<quint8>(frame.data[5]);
        //反向充电限压使能
        timingDataBuf[11] = static_cast<quint8>(frame.data[3]);
        break;
    case 0x18E43501:
        //低压侧最大输入电流
        timingDataBuf[25] = static_cast<quint8>(frame.data[0]) + static_cast<quint8>(frame.data[1]) * 256;
        //低压侧最大输出电流
        timingDataBuf[27] = static_cast<quint8>(frame.data[2]) + static_cast<quint8>(frame.data[3]) * 256;
        //高压侧最大输入电流
        timingDataBuf[26] = static_cast<quint8>(frame.data[4]) + static_cast<quint8>(frame.data[5]) * 256;
        //高压侧最大输出电流
        timingDataBuf[28] = static_cast<quint8>(frame.data[6]) + static_cast<quint8>(frame.data[7]) * 256;
        break;
    case 0x18E53501:
        //低压侧输出电流设置
        timingDataBuf[18] = static_cast<quint8>(frame.data[0]) + static_cast<quint8>(frame.data[1]) * 256;
        //高压侧输出电流设置
        timingDataBuf[19] = static_cast<quint8>(frame.data[2]) + static_cast<quint8>(frame.data[3]) * 256;
        //低压侧过压保护电压设置
        timingDataBuf[14] = static_cast<quint8>(frame.data[4]) + static_cast<quint8>(frame.data[5]) * 256;
        //高压侧过压保护电压设置
        timingDataBuf[15] = static_cast<quint8>(frame.data[6]) + static_cast<quint8>(frame.data[7]) * 256;
        break;
    case 0x18E63501:
        //低压侧输出电压设置
        timingDataBuf[12] = static_cast<quint8>(frame.data[0]) + static_cast<quint8>(frame.data[1]) * 256;
        //高压侧输出电压设置
        timingDataBuf[13] = static_cast<quint8>(frame.data[2]) + static_cast<quint8>(frame.data[3]) * 256;
        //低压侧欠压保护电压设置
        timingDataBuf[16] = static_cast<quint8>(frame.data[4]) + static_cast<quint8>(frame.data[5]) * 256;
        //高压侧欠压保护电压设置
        timingDataBuf[17] = static_cast<quint8>(frame.data[6]) + static_cast<quint8>(frame.data[7]) * 256;
        break;
    case 0x18E73501:
        //低压侧导纳设置
        timingDataBuf[29] = static_cast<quint8>(frame.data[0]);
        //高压侧导纳设置
        timingDataBuf[30] = static_cast<quint8>(frame.data[1]);
        //累计上电小时数
        timingDataBuf[64] = static_cast<quint8>(frame.data[2]) + static_cast<quint8>(frame.data[3]) * 256;
        //累计变换时间，分钟部分
        timingDataBuf[33] = static_cast<quint8>(frame.data[4]);
        //累计变换时间，小时部分
        timingDataBuf[32] = static_cast<quint8>(frame.data[6]) + static_cast<quint8>(frame.data[7]) * 256;
        break;
    case 0x18EC3501:
        //低压侧输出/输入功率设置
        timingDataBuf[36] = static_cast<quint8>(frame.data[0]) + static_cast<quint8>(frame.data[1]) * 256;
        //高压侧输出/输入功率设置
        timingDataBuf[37] = static_cast<quint8>(frame.data[2]) + static_cast<quint8>(frame.data[3]) * 256;
        break;
    case 0x18E83501:
        //低压侧累计输出电能低字
        timingDataBuf[38] = static_cast<quint8>(frame.data[0]) + static_cast<quint8>(frame.data[1]) * 256;
        //低压侧累计输出电能高字
        timingDataBuf[39] = static_cast<quint8>(frame.data[2]) + static_cast<quint8>(frame.data[3]) * 256;
        //高压侧累计输出电能，低字
        timingDataBuf[40] = static_cast<quint8>(frame.data[4]) + static_cast<quint8>(frame.data[5]) * 256;
        //高压侧累计输出电能，高字
        timingDataBuf[41] = static_cast<quint8>(frame.data[6]) + static_cast<quint8>(frame.data[7]) * 256;
        break;
    case 0x18E93501:
        //低压侧电压校正系数C
        timingDataBuf[49] = static_cast<quint8>(frame.data[0]) + static_cast<quint8>(frame.data[1]) * 256;
        //低压侧电压校正系数B
        timingDataBuf[48] = static_cast<quint8>(frame.data[2]) + static_cast<quint8>(frame.data[3]) * 256;
        //高压侧电压校正系数C
        timingDataBuf[53] = static_cast<quint8>(frame.data[4]) + static_cast<quint8>(frame.data[5]) * 256;
        //高压侧电压校正系数B
        timingDataBuf[52] = static_cast<quint8>(frame.data[6]) + static_cast<quint8>(frame.data[7]) * 256;
        break;
    case 0x18EA3501:
        //低压侧电流校正系数C
        timingDataBuf[57] = static_cast<quint8>(frame.data[0]) + static_cast<quint8>(frame.data[1]) * 256;
        //低压侧电流校正系数B
        timingDataBuf[56] = static_cast<quint8>(frame.data[2]) + static_cast<quint8>(frame.data[3]) * 256;
        //高压侧电流校正系数C
        timingDataBuf[61] = static_cast<quint8>(frame.data[4]) + static_cast<quint8>(frame.data[5]) * 256;
        //高压侧电流校正系数B
        timingDataBuf[60] = static_cast<quint8>(frame.data[6]) + static_cast<quint8>(frame.data[7]) * 256;
        break;
    }
}

void MainWindow::writeCanData(quint8 addr, char valueLow, char valueHigh)
{
    quint32 canId = canidHash.value(addr);
    if(canId == 0)
    {
        QMessageBox::information(this, tr("提示"), QString("暂时不支持修改地址为%1的寄存器").arg(addr));
        return;
    }
    can_frame frame = canFrameHash.value(canId);
    if(frame.can_id != canId)
    {
        QMessageBox::information(this, tr("提示"), QString("没有找到canid为%1的can_frame").arg(QString::number(canId, 16)));
        return;
    }
    frame.data[lowByteIndexHash.value(addr)] = valueLow;
    //不一定有两个字节
    if(highByteIndexHash.value(addr) != -1)
    {
        frame.data[highByteIndexHash.value(addr)] = valueHigh;
    }
    quint32 writeCanId = writeCanidHash.value(addr);
    sendCANData(writeCanId, frame.data);
}

void MainWindow::staticCanDataInit()
{
    //A侧(低压侧)最大输入电流
    canidHash.insert(25, 0x18E43501);
    lowByteIndexHash.insert(25, 0);
    highByteIndexHash.insert(25, 1);
    writeCanidHash.insert(25, 0x18E23A01);
    //B侧(高压侧)最大输入电流
    canidHash.insert(26, 0x18E43501);
    lowByteIndexHash.insert(26, 4);
    highByteIndexHash.insert(26, 5);
    writeCanidHash.insert(26, 0x18E23A01);
    //A侧(低压侧)最大输出电流
    canidHash.insert(27, 0x18E43501);
    lowByteIndexHash.insert(27, 2);
    highByteIndexHash.insert(27, 3);
    writeCanidHash.insert(27, 0x18E23A01);
    //B侧(高压侧)最大输出电流
    canidHash.insert(28, 0x18E43501);
    lowByteIndexHash.insert(28, 6);
    highByteIndexHash.insert(28, 7);
    writeCanidHash.insert(28, 0x18E23A01);
    //A 设定输出电流
    canidHash.insert(18, 0x18E53501);
    lowByteIndexHash.insert(18, 0);
    highByteIndexHash.insert(18, 1);
    writeCanidHash.insert(18, 0x18E33A01);
    //B 设定输出电流
    canidHash.insert(19, 0x18E53501);
    lowByteIndexHash.insert(19, 2);
    highByteIndexHash.insert(19, 3);
    writeCanidHash.insert(19, 0x18E33A01);
    //A侧过压保护电压
    canidHash.insert(14, 0x18E53501);
    lowByteIndexHash.insert(14, 4);
    highByteIndexHash.insert(14, 5);
    writeCanidHash.insert(14, 0x18E33A01);
    //B侧过压保护电压
    canidHash.insert(15, 0x18E53501);
    lowByteIndexHash.insert(15, 6);
    highByteIndexHash.insert(15, 7);
    writeCanidHash.insert(15, 0x18E33A01);
    //A侧欠压保护电压
    canidHash.insert(16, 0x18E63501);
    lowByteIndexHash.insert(16, 4);
    highByteIndexHash.insert(16, 5);
    writeCanidHash.insert(16, 0x18E43A01);
    //B侧欠压保护电压
    canidHash.insert(17, 0x18E63501);
    lowByteIndexHash.insert(17, 6);
    highByteIndexHash.insert(17, 7);
    writeCanidHash.insert(17, 0x18E43A01);
    //停止变换控制字
    canidHash.insert(3, 0x18E33501);
    lowByteIndexHash.insert(3, 4);
    highByteIndexHash.insert(3, -1);
    writeCanidHash.insert(3, 0x18E13A01);
    //A侧输出电压
    canidHash.insert(0x0c, 0x18E63501);
    lowByteIndexHash.insert(0x0c, 0);
    highByteIndexHash.insert(0x0c, 1);
    writeCanidHash.insert(0x0c, 0x18E43A01);
    //模式
    canidHash.insert(2, 0x18E33501);
    lowByteIndexHash.insert(2, 2);
    highByteIndexHash.insert(2, -1);
    writeCanidHash.insert(2, 0x18E13A01);
    //模块号
    canidHash.insert(0, 0x18E33501);
    lowByteIndexHash.insert(0, 0);
    highByteIndexHash.insert(0, -1);
    writeCanidHash.insert(0, 0x18E13A01);
    //波特率选择
    canidHash.insert(1, 0x18E33501);
    lowByteIndexHash.insert(1, 1);
    highByteIndexHash.insert(1, -1);
    writeCanidHash.insert(1, 0x18E13A01);
    //B侧输出电压
    canidHash.insert(0x0d, 0x18E63501);
    lowByteIndexHash.insert(0x0d, 2);
    highByteIndexHash.insert(0x0d, 3);
    writeCanidHash.insert(0x0d, 0x18E43A01);
    //A 导纳
    canidHash.insert(29, 0x18E73501);
    lowByteIndexHash.insert(29, 0);
    highByteIndexHash.insert(29, -1);
    writeCanidHash.insert(29, 0x18E53A01);
    //B 导纳
    canidHash.insert(30, 0x18E73501);
    lowByteIndexHash.insert(30, 1);
    highByteIndexHash.insert(30, -1);
    writeCanidHash.insert(30, 0x18E53A01);
    //风扇启动温度
    canidHash.insert(9, 0x18E33501);
    lowByteIndexHash.insert(9, 7);
    highByteIndexHash.insert(9, -1);
    writeCanidHash.insert(9, 0x18E13A01);
    //反向充电使能
    canidHash.insert(11, 0x18E33501);
    lowByteIndexHash.insert(11, 3);
    highByteIndexHash.insert(11, -1);
    writeCanidHash.insert(11, 0x18E13A01);
    //A 侧输出功率
    canidHash.insert(36, 0x18EC3501);
    lowByteIndexHash.insert(36, 0);
    highByteIndexHash.insert(36, 1);
    writeCanidHash.insert(36, 0x18EB3A01);
    //B 侧输出功率
    canidHash.insert(37, 0x18EC3501);
    lowByteIndexHash.insert(37, 2);
    highByteIndexHash.insert(37, 3);
    writeCanidHash.insert(37, 0x18EB3A01);
    //单双向选择
    canidHash.insert(7, 0x18E33501);
    lowByteIndexHash.insert(7, 5);
    highByteIndexHash.insert(7, -1);
    writeCanidHash.insert(7, 0x18E13A01);
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
    refresh();
    for(int i = 1; i < 7; i++)
    {
        refresh(i);
    }
    // if(waitMessageRemaingTime > 0)
    // {
    //     waitMessageRemaingTime--;
    // }
    // if(dataRefreshRemaingTime > 0)
    // {
    //     dataRefreshRemaingTime--;
    // }

    // //说明串口空闲，看看有没有手动的命令要下发
    // if(waitMessageRemaingTime == 0)
    // {
    //     //说明有二次手动命令
    //     if(manualFlag == 2)
    //     {
    //         sendPortData();
    //         manualFlag = 0;
    //         secondCMDSend();
    //     }
    //     //说明有手动命令要下发
    //     else if(manualFlag == 1)
    //     {
    //         //手动命令下发
    //         sendPortData();
    //         manualFlag = 0;
    //     }
    //     //说明没有手动命令要下发，就判断是否到了刷新时间
    //     else if(dataRefreshRemaingTime <= 0)
    //     {
    //         sendGetAllDataCMD();
    //     }
    // }
}

void MainWindow::onReceiveTimerTimeout()
{

    if(connFlag == 0)
    {
        return;
    }
    receiveCANData();
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
    return crcResultArray[0] == data[data.size() - 2] && crcResultArray[1] == data[data.size() - 1];
}

void MainWindow::dealMessage(const QByteArray &data)
{
    //查询所有的命令
    if(data[1] == READ_CMD)
    {
        //清空等待时间
        waitMessageRemaingTime = 0;
        //更新缓存
        QByteArray dataBuf = data.mid(3, data.size() - 5);
        for(int i = 0; i < dataBuf.size(); i = i + 2)
        {
            timingDataBuf[i / 2] = static_cast<quint8>(dataBuf.at(i)) * 256 + static_cast<quint8>(dataBuf.at(i + 1));
        }
        refreshAll();
    }
    else if(data[1] == WRITE_ONE_CMD)
    {
        //写入命令返回，立刻回显
        sendGetAllDataCMD();

    }
    else if(data[1] == WRITE_MULTIPLE_CMD)
    {
        //多个写入命令返回，暂时没有处理逻辑、
        waitMessageRemaingTime = 0;
    }
}


void MainWindow::on_connBtn_clicked()
{
    if(ui->connBtn->text() == "建立连接")
    {
        //连接成功
        if(initCAN())
        {
            connFlag = 1;
            connectStatusLabel->setText(connStatus.arg("已连接"));
            connectStatusLabel->setStyleSheet("QLabel { background-color : green; color : white; }");
            ui->connBtn->setText("断开连接");
        }
    }
    else if(ui->connBtn->text() == "断开连接")
    {
        closeCAN();
        connFlag = 0;
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
    writeCanData(3, 0, 0);
}

//手动模式
void MainWindow::on_pushButton_7_clicked()
{
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("请先建立连接!"));
        return;
    }
    if(timingDataBuf[3] == 4)
    {
        QMessageBox::information(this, tr("提示"), tr("当前已经是手动模式!"));
        return;
    }
    // //手动模式，
    // manualWriteOneCMDBuild(static_cast<char>(0x00), static_cast<char>(0x03), static_cast<char>(0x00), static_cast<char>(0x04));
    //手动模式，设置停止变换控制字为4
    writeCanData(3, 4, 0);
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
    //manualWriteOneCMDBuild(static_cast<char>(0x00), 0x03, static_cast<char>(0x00), 0x05);
    //手动关闭，设置停止变换控制字为5
    writeCanData(3, 5, 0);
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

void MainWindow::closeEvent(QCloseEvent *event)
{
    //刷新配置
    QString configFileName = "config.ini";
    QString iniFilePath = QDir::currentPath() + "/" + configFileName;
    QFile configFile(iniFilePath);
    QSettings settings(iniFilePath, QSettings::IniFormat);
    settings.setValue("A_TURN_HIGH_V", ATurnHighV);
    settings.setValue("A_TURN_LOW_V", ATurnLowV);
    settings.setValue("B_TURN_HIGH_V", BTurnHighV);
    settings.setValue("B_TURN_LOW_V", BTurnLowV);
    settings.setValue("CYCLE_NUM", cycleNum);
}

void MainWindow::on_comboBox_3_currentIndexChanged(int index)
{
    deviceType = index;
}

void MainWindow::chargeTimeCountAdd()
{
    chargeTime++;
}


void MainWindow::on_pushButton_10_clicked()
{
    cycleNum = 0;
}


void MainWindow::on_cbx_workingmode_currentIndexChanged(int index)
{
    quint16 value = index * 2 + 1 + (timingDataBuf[2] & 0xE0);
    writeCanData(2, value & 0xFF, value >> 8);
}

