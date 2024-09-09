#ifndef TFORM6_H
#define TFORM6_H

#include <QWidget>
class MainWindow;
namespace Ui {
class TForm6;
}

class TForm6 : public QWidget
{
    Q_OBJECT

public:
    explicit TForm6(QWidget *parent = nullptr);
    ~TForm6();
    void refresh();
    void connectAll();
private slots:
    void onEditingFinished();
private:
    Ui::TForm6 *ui;
    MainWindow* mainwindow;
};

#endif // TFORM6_H
