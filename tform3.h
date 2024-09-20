#ifndef TFORM3_H
#define TFORM3_H

#include <QWidget>
class QLineEdit;
namespace Ui {
class TForm3;
}

class TForm3 : public QWidget
{
    Q_OBJECT

public:
    explicit TForm3(QWidget *parent = nullptr);
    ~TForm3();
    void refresh();
    void connectAll();
private slots:
    void on_lineEdit_13_returnPressed();
    void onEditingFinished();

private:
    Ui::TForm3 *ui;
    QVector<QLineEdit*> lineEdits;
};

#endif // TFORM3_H
