#include "mainwindow.h"
#include "audiorecorder.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AudioRecorder ar;
    ar.record();
    MainWindow w;
    w.show();
    return a.exec();
}
