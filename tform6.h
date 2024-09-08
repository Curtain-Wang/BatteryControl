#ifndef TFORM6_H
#define TFORM6_H

#include <QWidget>

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

private:
    Ui::TForm6 *ui;
};

#endif // TFORM6_H
