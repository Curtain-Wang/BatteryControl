#include "tform5.h"
#include "ui_tform5.h"
#include "globalparam.h"
#include <QMessageBox>
TForm5::TForm5(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TForm5)
{
    ui->setupUi(this);
    //设置窗口标志，确保有边框和标题栏、最小化、关闭，最大化
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    setWindowIcon(QIcon(":/icons/images/battery_control_icon.ico"));
    refresh();
}

TForm5::~TForm5()
{
    delete ui;
}

void TForm5::refresh()
{
    QVector<int> indices = {14, 15, 16, 17, 18, 19, 25, 26, 27, 28};
    QVector<QLineEdit*> lineEdits = {ui->d14, ui->d15, ui->d16, ui->d17, ui->d18, ui->d19, ui->d25, ui->d26, ui->d27, ui->d28};
    for (int i = 0; i < indices.size(); ++i) {
        if(i < 4)
        {
            lineEdits[i]->setText(QString::number(static_cast<float>(timingDataBuf[indices[i]]) / 10, 'f', 1));
        }
        else
        {
            qint16 value = timingDataBuf[indices[i]];
            if(deviceType == 0 || deviceType == 1)
            {
                lineEdits[i]->setText(QString::number(static_cast<float>(value) / 100, 'f', 2));
            }
            else
            {
                lineEdits[i]->setText(QString::number(static_cast<float>(value) / 10, 'f', 1));
            }
        }
    }
    //刷新翻转电压
    ui->lineEditAHigh->setText(QString::number(static_cast<float>(ATurnHighV) / 10, 'f', 1));
    ui->lineEditBHigh->setText(QString::number(static_cast<float>(BTurnHighV) / 10, 'f', 1));
    ui->lineEditALow->setText(QString::number(static_cast<float>(ATurnLowV) / 10, 'f', 1));
    ui->lineEditBLow->setText(QString::number(static_cast<float>(BTurnLowV) / 10, 'f', 1));
}
