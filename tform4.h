#ifndef TFORM4_H
#define TFORM4_H

#include <QWidget>

namespace Ui {
class TForm4;
}

class TForm4 : public QWidget
{
    Q_OBJECT

public:
    explicit TForm4(QWidget *parent = nullptr);
    ~TForm4();
    void refresh();

private:
    Ui::TForm4 *ui;
};

#endif // TFORM4_H
