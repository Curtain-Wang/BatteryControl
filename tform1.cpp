#include "tform1.h"
#include "ui_tform1.h"

TForm1::TForm1(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TForm1)
{
    ui->setupUi(this);
    //设置窗口标志，确保有边框和标题栏、最小化、关闭，最大化
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
}

TForm1::~TForm1()
{
    delete ui;
}
