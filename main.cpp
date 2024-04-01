// #include "listner.h"
#include "mainwindow.h"
#include "audio_capture.h"

#include <QApplication>
#include<QtNetwork>


#include "signaling_server.h"
#include "client.h"
#include <rtc/rtc.hpp>



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
void process_buff(const QByteArray& data){
    for(int i =0; i <data.size(); i++){
        qDebug() <<data.at(i);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // QThread serverThread;
    // QThread clientThread;
    // QThread clientThread2;
    // // AudioRecorder ar;
    // // ar.record();
    // QObject::connect(&serverThread, &QThread::started, [](){
    //     runServer2();
    // });
    // QObject::connect(&clientThread, &QThread::started, [](){
    //     runClient2();
    // });
    // QObject::connect(&clientThread2, &QThread::started, [](){
    //     runClient3();
    // });

    // serverThread.start();
    // clientThread.start();
    // QThread::msleep(1000);
    // clientThread2.start();
    AudioCapture ac;
    QObject::connect(&ac,&AudioCapture::bufferReady,process_buff);
    MainWindow w;
    w.show();
    return a.exec();

}
