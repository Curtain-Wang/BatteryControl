#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H

#include <QComboBox>
class MyComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit MyComboBox(QWidget *parent = nullptr);

protected:
    // 重写 showPopup 事件处理
    void showPopup() override;
    void refreshPort();
};

#endif // MYCOMBOBOX_H
