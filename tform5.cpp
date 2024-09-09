#include "tform5.h"
#include "ui_tform5.h"
#include "globalparam.h"
#include "mainwindow.h"

TForm5::TForm5(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TForm5)
{
    ui->setupUi(this);
    mainwindow = qobject_cast<MainWindow*>(parent);
    //设置窗口标志，确保有边框和标题栏、最小化、关闭，最大化
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    connectAll();
    // qDebug() << "parent 的类型是：" << parent->metaObject()->className();
}

TForm5::~TForm5()
{
    delete ui;
}

void TForm5::refresh()
{
    QVector<int> indices = {12, 13, 14, 15, 16, 17, 18, 19, 25, 26, 27, 28};
    QVector<QLineEdit*> lineEdits = {ui->d12, ui->d13, ui->d14, ui->d15, ui->d16, ui->d17, ui->d18, ui->d19, ui->d25, ui->d26, ui->d27, ui->d28};
    for (int i = 0; i < indices.size(); ++i) {
        lineEdits[i]->setText(QString::number(static_cast<float>(timingDataBuf[indices[i]]) / 10, 'f', 1));
    }
}

void TForm5::connectAll()
{
    QVector<QLineEdit*> lineEdits = {ui->d12, ui->d13, ui->d14, ui->d15, ui->d16, ui->d17, ui->d18, ui->d19, ui->d25, ui->d26, ui->d27, ui->d28};
    for (int i = 0; i < lineEdits.size(); ++i) {
        // connect(lineEdits[i], &QLineEdit::editingFinished, this, &TForm5::onEditingFinished);
        connect(lineEdits[i], &QLineEdit::returnPressed, this, &TForm5::onEditingFinished);
    }
}

void TForm5::onEditingFinished()
{
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
