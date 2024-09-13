#include "tform7.h"
#include "ui_tform7.h"
#include "globalparam.h"
#include <QMessageBox>
#include "mainwindow.h"
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

void TForm7::on_lineEdit_editingFinished()
{
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("请先建立连接!"));
        return;
    }
    quint16 value = ui->lineEdit->text().toFloat() * 10;
    mainwindow->manualWriteOneCMDBuild(static_cast<char>(0), editAddrLow, value >> 8, value & 0xFF);
    this->close();
}

