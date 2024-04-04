#ifndef CLIENT_H
#define CLIENT_H

#include <QCoreApplication>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QDate>
#include <QTime>
#include <QThread>


// #include <iostream>

class TcpClient : public QObject {
    Q_OBJECT

public:
    TcpClient(std::string message, std::string role);
    // void connectServer();
    void runClient2();
    void sendMessage(std::string message);
private slots:
    void connected();
    void receiveResponse();
private:
    QTcpSocket* socket;
    std::string _message;
    std::string _role;
signals:
    void set_remote(QString json_message);
};

#endif // CLIENT_H
