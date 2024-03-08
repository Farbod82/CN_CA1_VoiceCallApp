#include "audiorecorder.h"
#include "mainwindow.h"

#include <QApplication>
#include <QAudioFormat>
#include <QAudioInput>
#include <QThread>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AudioRecorder ar;
    ar.record();
    MainWindow w;
    w.show();
    return a.exec();
}
