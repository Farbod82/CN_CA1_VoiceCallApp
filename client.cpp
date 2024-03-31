#include "client.h"
#include <QTcpSocket>
#include <QObject>


TcpClient::TcpClient(std::string message) : QObject(), socket(new QTcpSocket()) {
    _message = message;
}

void TcpClient::connected() {
    socket->write(_message.c_str());
}
void TcpClient::receiveResponse(){
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    std::string message = socket->readAll().data();
    qDebug() << "response from server: " << message;
}

void TcpClient::runClient2() {
    connect(socket, &QTcpSocket::connected, this, &TcpClient::connected);
    connect(socket,&QTcpSocket::readyRead,this, &TcpClient::receiveResponse);
    socket->connectToHost(QHostAddress::LocalHost, 8080, QIODevice::ReadWrite);
    while (!socket->waitForConnected(30000));
}



