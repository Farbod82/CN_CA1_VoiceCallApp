// #include "listner.h"
#include "mainwindow.h"
#include "audio_capture.h"
#include "offerer.h"
#include "signaling_server.h"
#include "answerer.h"

#include <QApplication>
#include <QtNetwork>
#include <QObject>

#include "client.h"
#include <rtc/rtc.hpp>



// void runClient2() {
//     // TcpClient* c = new TcpClient("CONNECT Ali");
//     // c->runClient2();
//     // c->sendMessage("CONNECT Ali");
//     AudioPlayer* ap = new AudioPlayer();
//     AudioCapture* ac = new AudioCapture();
//     ap->startPlaying();
//     answerer ans("Ahmad","192.168.1.106",ap,ac);
//     ans.run_answerer();

//     // std::cout << "#########################################\n";
//     // ans.send_message("Hello Farbod!");
// }
// void runClient3() {
//     // TcpClient* c = new TcpClient("CALL Farbod");
//     // c->runClient2();
//     AudioPlayer* ap = new AudioPlayer();
//     AudioCapture* ac = new AudioCapture();
//     offerer of("Farbod","Ahmad", "192.168.1.106", ap, ac);
//     of.run_offerer();
// }


// void run_server2()
// {
//     TcpServer *s = new TcpServer();
//     s->run_server2();
// }
// void process_buff(const QByteArray& data){
//     for(int i =0; i <data.size(); i++){
//         qDebug() <<data.at(i);
//     }
// }

int main(int argc, char *argv[])
{
    //offerer
    QApplication a(argc, argv);
    // QThread serverThread;
    // QThread clientThread;
    // QThread clientThread2;
    // AudioRecorder ar;
    // ar.record();
    // QObject::connect(&serverThread, &QThread::started, []() { run_server2(); });
    // QObject::connect(&clientThread, &QThread::started, []() { runClient2(); });
    // QObject::connect(&clientThread2, &QThread::started, []() { runClient3(); });

    // serverThread.start();
    // clientThread.start();
    // QThread::msleep(5000);
    // clientThread2.start();
    // QThread::msleep(1000);
    // AudioCapture* ac = new AudioCapture();
    // AudioPlayer* ap = new AudioPlayer();
    // ap->startPlaying();
    // ac->startRecord();
    // offerer of("Farbod","Ahmad","192.168.1.106", ap, ac);
    // AudioCapture ac;
    // of.run_offerer();
    MainWindow w;
    w.show();
    return a.exec();

}
