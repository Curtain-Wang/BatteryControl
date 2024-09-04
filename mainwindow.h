#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class TForm1;
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool isCreator(int num);
private:
    Ui::MainWindow *ui;
    TForm1* tform1;
};
#endif // MAINWINDOW_H
