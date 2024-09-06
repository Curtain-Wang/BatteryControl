QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    globalparam.cpp \
    main.cpp \
    mainwindow.cpp \
    tform1.cpp \
    tform2.cpp \
    tform3.cpp

HEADERS += \
    globalparam.h \
    mainwindow.h \
    tform1.h \
    tform2.h \
    tform3.h

FORMS += \
    mainwindow.ui \
    tform1.ui \
    tform2.ui \
    tform3.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
