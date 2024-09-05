#include "tform2.h"
#include "ui_tform2.h"

TForm2::TForm2(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TForm2)
{
    ui->setupUi(this);
    //设置窗口标志，确保有边框和标题栏、最小化、关闭，最大化
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
}

TForm2::~TForm2()
{
    delete ui;
}
