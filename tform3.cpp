#include "tform3.h"
#include "ui_tform3.h"

TForm3::TForm3(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TForm3)
{
    ui->setupUi(this);
    //设置窗口标志，确保有边框和标题栏、最小化、关闭，最大化
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
}

TForm3::~TForm3()
{
    delete ui;
}
