QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    asynclogger.cpp \
    globalparam.cpp \
    main.cpp \
    mainwindow.cpp \
    mycombobox.cpp \
    mylineedit.cpp \
    tform1.cpp \
    tform2.cpp \
    tform3.cpp \
    tform4.cpp \
    tform5.cpp \
    tform6.cpp \
    tform7.cpp

HEADERS += \
    asynclogger.h \
    globalparam.h \
    mainwindow.h \
    mycombobox.h \
    mylineedit.h \
    tform1.h \
    tform2.h \
    tform3.h \
    tform4.h \
    tform5.h \
    tform6.h \
    tform7.h

FORMS += \
    mainwindow.ui \
    tform1.ui \
    tform2.ui \
    tform3.ui \
    tform4.ui \
    tform5.ui \
    tform6.ui \
    tform7.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/include/ -ltpscaleplugin
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/include/ -ltpscaleplugind

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
