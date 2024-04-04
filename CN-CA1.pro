QT = core

CONFIG += c++17 cmdline

QT += multimedia
QT += widgets

# QT += application
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        answerer.cpp \
        audio_capture.cpp \
        client.cpp \
        connection.cpp \
        main.cpp \
        mainwindow.cpp \
        offerer.cpp \
        signaling_server.cpp \
        stun_client.cpp \
        stunserver.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += G:\Documents\libdatachannel\include
LIBS += -LG:\Documents\libdatachannel\build -ldatachannel.dll
LIBS += -LG:\software\Qt\Tools\OpenSSLv3\Win_x64\bin -lcrypto-3-x64 -lssl-3-x64

FORMS += \
    mainwindow.ui

DISTFILES += \
    CN-CA1.pro.user \
    README.md

HEADERS += \
    answerer.h \
    audio_capture.h \
    audiorecorder.h \
    client.h \
    connection.h \
    mainwindow.h \
    offerer.h \
    signaling_server.h
