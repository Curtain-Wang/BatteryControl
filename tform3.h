#ifndef TFORM3_H
#define TFORM3_H

#include <QWidget>

namespace Ui {
class TForm3;
}

class TForm3 : public QWidget
{
    Q_OBJECT

public:
    explicit TForm3(QWidget *parent = nullptr);
    ~TForm3();

private:
    Ui::TForm3 *ui;
};

#endif // TFORM3_H
