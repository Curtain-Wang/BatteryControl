#ifndef TFORM5_H
#define TFORM5_H

#include <QWidget>
class MainWindow;
namespace Ui {
class TForm5;
}

class TForm5 : public QWidget
{
    Q_OBJECT

public:
    explicit TForm5(QWidget *parent = nullptr);
    ~TForm5();
    void refresh();
    // void connectAll();
// private slots:
//     void onEditingFinished();
//     void on_lineEditAHigh_editingFinished();

private:
    Ui::TForm5 *ui;
};

#endif // TFORM5_H
