#include "audiorecorder.h"
#include "mainwindow.h"

#include <QApplication>
#include <QAudioFormat>
#include <QAudioInput>
#include <QThread>
#include <rtc/rtc.h>
#include <boost/asio.hpp>

// #include "stunserver.cpp"
// #include "stun_client.cpp"
#include "listner.h"
#include "signaling_server.cpp"
#include "test_client.cpp"


void runServer2(){
    boost::asio::io_context io_context;
    // we'll just use an arbitrary port here
    Listner s(io_context, 8080);
    // and we run until our server is alive
    io_context.run();
}
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QThread serverThread;
    QThread clientThread;
    QThread clientThread2;

    QObject::connect(&serverThread, &QThread::started, [](){
        runServer2();
    });

    QObject::connect(&clientThread, &QThread::started, [](){
        runClient2();
    });
    QObject::connect(&clientThread2, &QThread::started, [](){
        runClient2();
    });

    serverThread.start();
    clientThread.start();

    // QThread::msleep(10000);
    clientThread2.start();

    // AudioRecorder ar;
    // ar.record();


    MainWindow w;
    w.show();
    return a.exec();
}
