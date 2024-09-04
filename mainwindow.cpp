#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tform1.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tform1 = new TForm1(this);
    tform1->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::isCreator(int num)
{
    switch (num) {
    case 1:
        return tform1 != nullptr;
        break;
    default:
        return false;
    }
}
