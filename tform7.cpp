#include "tform7.h"
#include "ui_tform7.h"
#include "globalparam.h"
#include <QMessageBox>
#include "mainwindow.h"
#include <QDir>
#include <QSettings>
TForm7::TForm7(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TForm7)
{
    //设置窗口标志，确保有边框和标题栏、最小化、关闭，最大化
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    setWindowIcon(QIcon(":/icons/images/battery_control_icon.ico"));
    ui->setupUi(this);
}

TForm7::~TForm7()
{
    delete ui;
}

void TForm7::updateTurnV(quint8 type, quint16 value)
{
    QString configFileName = "config.ini";
    QString iniFilePath = QDir::currentPath() + "/" + configFileName;
    QFile configFile(iniFilePath);
    QSettings settings(iniFilePath, QSettings::IniFormat);
    switch (type) {
    case 1:
        ATurnHighV = value;
        settings.setValue("A_TURN_HIGH_V", ATurnHighV);
        break;

    case 2:
        BTurnHighV = value;
        settings.setValue("B_TURN_HIGH_V", BTurnHighV);
        break;
    case 3:
        ATurnLowV = value;
        settings.setValue("A_TURN_LOW_V", ATurnLowV);
        break;
    case 4:
        BTurnLowV = value;
        settings.setValue("B_TURN_LOW_V", BTurnLowV);
        break;
    default:
        break;
    }
}

// void TForm7::on_lineEdit_editingFinished()
// {
//     if(connFlag == 0)
//     {
//         QMessageBox::information(this, tr("提示"), tr("请先建立连接!"));
//         return;
//     }
//     quint16 value = ui->lineEdit->text().toFloat() * 10;
//     //说明是直接下发到下位机的
//     if(editAddrLow != -1)
//     {
//         mainwindow->manualWriteOneCMDBuild(static_cast<char>(0), editAddrLow, value >> 8, value & 0xFF);
//     }
//     else
//     {
//         QString configFileName = "config.ini";
//         QString iniFilePath = QDir::currentPath() + "/" + configFileName;
//         QFile configFile(iniFilePath);
//         QSettings settings(iniFilePath, QSettings::IniFormat);

//         if(lineEditName == "lineEditAHigh")
//         {
//             //A翻高压大于A过压的0.9，此时需要调整A过压的设置
//             if(value >= timingDataBuf[14] * 0.9)
//             {
//                 quint16 newHighV = value * 1.2;
//                 mainwindow->manualWriteOneCMDBuild(static_cast<char>(0), 14, newHighV >> 8, newHighV & 0xFF);
//             }
//             //将A翻高写入配置
//             qDebug() << "A侧翻转高压修改：" << value;
//             ATurnHighV = value;
//             settings.setValue("A_TURN_HIGH_V", ATurnHighV);
//         }
//         if(lineEditName == "lineEditBHigh")
//         {
//             //B翻高压大于B过压的0.9，此时需要调整B过压的设置
//             if(value >= timingDataBuf[15] * 0.9)
//             {
//                 quint16 newHighV = value * 1.2;
//                 mainwindow->manualWriteOneCMDBuild(static_cast<char>(0), 15, newHighV >> 8, newHighV & 0xFF);
//             }
//             //将A翻高写入配置
//             BTurnHighV = value;
//             settings.setValue("B_TURN_HIGH_V", BTurnHighV);
//         }
//         if(lineEditName == "lineEditALow")
//         {
//             //A翻低压小于A欠压的1.1，此时需要调整A欠压的设置
//             if(value <= timingDataBuf[16] * 1.1)
//             {
//                 quint16 newHighV = value * 0.9;
//                 mainwindow->manualWriteOneCMDBuild(static_cast<char>(0), 16, newHighV >> 8, newHighV & 0xFF);
//             }
//             ATurnLowV = value;
//             settings.setValue("A_TURN_LOW_V", ATurnLowV);
//         }
//         if(lineEditName == "lineEditBLow")
//         {
//             //B翻低压小于B欠压的1.1，此时需要调整B欠压的设置
//             if(value <= timingDataBuf[17] * 1.1)
//             {
//                 quint16 newHighV = value * 0.9;
//                 mainwindow->manualWriteOneCMDBuild(static_cast<char>(0), 17, newHighV >> 8, newHighV & 0xFF);
//             }
//             BTurnLowV = value;
//             settings.setValue("B_TURN_LOW_V", BTurnLowV);
//         }
//     }
//     this->close();
// }


void TForm7::on_lineEdit_returnPressed()
{
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("请先建立连接!"));
        return;
    }

    //说明是直接下发到下位机的
    if(editAddrLow != -1)
    {
        quint16 value = 0;
        if(deviceType < 2 && (editAddrLow == 18 || editAddrLow == 19 || editAddrLow == 25
             || editAddrLow == 26 || editAddrLow == 27 || editAddrLow == 28))
        {
            value = ui->lineEdit->text().toFloat() * 100;
        }
        else
        {
            value = ui->lineEdit->text().toFloat() * 10;
            //修改的是A过压
            if(editAddrLow == 14)
            {
                //如果A过压的0.9小于翻转高压，则需要降低翻转高压
                if(value * 0.9 < ATurnHighV)
                {
                    if(value * 0.9 <= ATurnLowV)
                    {
                        QMessageBox::information(this, tr("提示"), tr("过压设置的9成不能小于翻转低压!"));
                        return;
                    }
                    else
                    {
                        updateTurnV(1, value * 0.9);
                    }
                }
            }
            //修改的是B过压
            if(editAddrLow == 15)
            {
                //如果B过压的0.9小于翻转高压，则需要降低翻转高压
                if(value * 0.9 < BTurnHighV)
                {
                    if(value * 0.9 <= BTurnLowV)
                    {
                        QMessageBox::information(this, tr("提示"), tr("过压设置的9成不能小于翻转低压!"));
                        return;
                    }
                    else
                    {
                        updateTurnV(2, value * 0.9);
                    }
                }
            }
            //修改的是A欠压
            if(editAddrLow == 16)
            {
                //如果A欠压的1.1大于于翻转低压，则需要提高翻转低压
                if(value * 1.1 > ATurnLowV)
                {
                    if(value * 1.1 >= ATurnHighV)
                    {
                        QMessageBox::information(this, tr("提示"), tr("欠压设置的1.1倍不能大于翻转高压!"));
                        return;
                    }
                    else
                    {
                        updateTurnV(3, value * 1.1);
                    }
                }
            }
            //修改的是B欠压
            if(editAddrLow == 17)
            {
                //如果B欠压的1.1大于于翻转低压，则需要提高翻转低压
                if(value * 1.1 > BTurnLowV)
                {
                    if(value * 1.1 >= BTurnHighV)
                    {
                        QMessageBox::information(this, tr("提示"), tr("欠压设置的1.1倍不能大于翻转高压!"));
                        return;
                    }
                    else
                    {
                        updateTurnV(4, value * 1.1);
                    }
                }
            }
        }
        mainwindow->manualWriteOneCMDBuild(static_cast<char>(0), editAddrLow, value >> 8, value & 0xFF);
    }
    else
    {
        quint16 value = ui->lineEdit->text().toFloat() * 10;
        QString configFileName = "config.ini";
        QString iniFilePath = QDir::currentPath() + "/" + configFileName;
        QFile configFile(iniFilePath);
        QSettings settings(iniFilePath, QSettings::IniFormat);
        if(lineEditName == "lineEditAHigh")
        {
            if(value <= ATurnLowV)
            {
                QMessageBox::information(this, tr("提示"), tr("翻转高压不能低于翻转低压!"));
                return;
            }
            //A翻高压大于A过压的0.9，此时需要调整A过压的设置
            if(value >= timingDataBuf[14] * 0.9)
            {
                quint16 newHighV = value * 1.1;
                mainwindow->manualWriteOneCMDBuild(static_cast<char>(0), 14, newHighV >> 8, newHighV & 0xFF);
            }
            //将A翻高写入配置
            qDebug() << "A侧翻转高压修改：" << value;
            ATurnHighV = value;
            settings.setValue("A_TURN_HIGH_V", ATurnHighV);
        }
        if(lineEditName == "lineEditBHigh")
        {
            if(value <= BTurnLowV)
            {
                QMessageBox::information(this, tr("提示"), tr("翻转高压不能低于翻转低压!"));
                return;
            }
            //B翻高压大于B过压的0.9，此时需要调整B过压的设置
            if(value >= timingDataBuf[15] * 0.9)
            {
                quint16 newHighV = value * 1.1;
                mainwindow->manualWriteOneCMDBuild(static_cast<char>(0), 15, newHighV >> 8, newHighV & 0xFF);
            }
            //将A翻高写入配置
            BTurnHighV = value;
            settings.setValue("B_TURN_HIGH_V", BTurnHighV);
        }
        if(lineEditName == "lineEditALow")
        {
            if(value >= ATurnHighV)
            {
                QMessageBox::information(this, tr("提示"), tr("翻转低压不能高于翻转高压!"));
                return;
            }
            //A翻低压小于A欠压的1.1，此时需要调整A欠压的设置
            if(value <= timingDataBuf[16] * 1.1)
            {
                quint16 newHighV = value * 0.9;
                mainwindow->manualWriteOneCMDBuild(static_cast<char>(0), 16, newHighV >> 8, newHighV & 0xFF);
            }
            ATurnLowV = value;
            settings.setValue("A_TURN_LOW_V", ATurnLowV);
        }
        if(lineEditName == "lineEditBLow")
        {
            if(value >= BTurnHighV)
            {
                QMessageBox::information(this, tr("提示"), tr("翻转低压不能高于翻转高压!"));
                return;
            }
            //B翻低压小于B欠压的1.1，此时需要调整B欠压的设置
            if(value <= timingDataBuf[17] * 1.1)
            {
                quint16 newHighV = value * 0.9;
                mainwindow->manualWriteOneCMDBuild(static_cast<char>(0), 17, newHighV >> 8, newHighV & 0xFF);
            }
            BTurnLowV = value;
            settings.setValue("B_TURN_LOW_V", BTurnLowV);
        }
    }
    this->close();
}

