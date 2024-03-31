// #include "listner.h"
#include "mainwindow.h"

#include <QApplication>
#include<QtNetwork>


#include "signaling_server.h"
#include "client.h"



void runClient2() {
    TcpClient* c = new TcpClient("CONNECT Farbod");
    c->runClient2();
}
void runClient3() {
    TcpClient* c = new TcpClient("CALL Farbod");
    c->runClient2();
}

void runServer2(){
    TcpServer* s = new TcpServer();
    s->runServer2();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QThread serverThread;
    QThread clientThread;
    QThread clientThread2;
    // AudioRecorder ar;
    // ar.record();
    QObject::connect(&serverThread, &QThread::started, [](){
        runServer2();
    });
    QObject::connect(&clientThread, &QThread::started, [](){
        runClient2();
    });
    QObject::connect(&clientThread2, &QThread::started, [](){
        runClient3();
    });

    serverThread.start();
    clientThread.start();
    QThread::msleep(1000);
    clientThread2.start();

    MainWindow w;
    w.show();
    return a.exec();
}
