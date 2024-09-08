#include "tform5.h"
#include "ui_tform5.h"
#include "globalparam.h"

TForm5::TForm5(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TForm5)
{
    ui->setupUi(this);
    //设置窗口标志，确保有边框和标题栏、最小化、关闭，最大化
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
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
