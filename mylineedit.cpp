#include "MyLineEdit.h"
#include <QDebug>
#include "globalparam.h"
#include "tform7.h"
MyLineEdit::MyLineEdit(QWidget *parent) : QLineEdit(parent)
{
}

void MyLineEdit::onTFormDestroyed(QObject *obj)
{
    if(obj == tform7)
    {
        tform7 = nullptr;
    }
}

void MyLineEdit::mousePressEvent(QMouseEvent *event)
{
    // 调用父类的 mousePressEvent 方法，确保原有功能正常
    QLineEdit::mousePressEvent(event);

    // 触发自定义的 clicked 信号
    emit clicked();

    // 你可以在这里添加更多点击处理逻辑
    qDebug() << objectName()<< "LineEdit clicked!";

    if(objectName() == "lineEditAHigh" || objectName() == "lineEditALow"
        || objectName() == "lineEditBHigh" || objectName() == "lineEditBLow")
    {
        editAddrLow = -1;
        lineEditName = objectName();
    }
    else
    {
        editAddrLow = objectName().mid(1).toInt();
    }
    if(tform7 == nullptr)
    {
        tform7 = new TForm7(this);
        tform7->setAttribute(Qt::WA_DeleteOnClose);
        connect(tform7, &TForm7::destroyed, this, &MyLineEdit::onTFormDestroyed);
    }
    tform7->show();
}
