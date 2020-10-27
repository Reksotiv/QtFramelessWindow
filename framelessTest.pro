QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    framelesswindow.cpp \
    main.cpp \
    mainwindow.cpp \
    titlebar.cpp

HEADERS += \
    framelesswindow.h \
    mainwindow.h \
    titlebar.h

FORMS += \
    framelesswindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    framelesswindow.qrc \
    styles/qdarkstyle/style.qrc \
    styles/white/qss.qrc
