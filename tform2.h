#ifndef TFORM2_H
#define TFORM2_H

#include <QWidget>
namespace Ui {
class TForm2;
}

class TForm2 : public QWidget
{
    Q_OBJECT

public:
    explicit TForm2(QWidget *parent = nullptr);
    ~TForm2();
    void refresh();

private slots:
    void on_lineEdit_returnPressed();

    void on_lineEdit_2_returnPressed();

private:
    Ui::TForm2 *ui;
};

#endif // TFORM2_H
