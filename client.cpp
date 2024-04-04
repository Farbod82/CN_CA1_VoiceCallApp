#include "client.h"
#include <QTcpSocket>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>

TcpClient::TcpClient(std::string message,std::string role) : QObject(), socket(new QTcpSocket()) {
    _message = message;
    _role = role;
}

void TcpClient::connected() {
    if (_role.compare("answerer") == 0)
        socket->write(("CONNECT "+ _message).c_str());
    qDebug() << "connected";
}
void TcpClient::receiveResponse(){
    std::cout << "\nzzzzzzzzzzzzzzzzzzzzzz\n";
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    std::string message = socket->readAll().data();
    QString q_string_message = QString::fromStdString(message);
    qDebug() << "response from server: " << message;
    std::cout << "response from server: " << message;

    QJsonDocument jsonDocument = QJsonDocument::fromJson(q_string_message.toUtf8());
    if (!jsonDocument.isNull()) {
        if (jsonDocument.isObject()) {
            QJsonObject jsonObject = jsonDocument.object();
            if (jsonObject["type"] == "set_remote"){
                emit sdpSet(q_string_message);
            }
        }
    }
}

void TcpClient::runClient2() {
    connect(socket, &QTcpSocket::connected, this, &TcpClient::connected);
    connect(socket,&QTcpSocket::readyRead,this, &TcpClient::receiveResponse);
    socket->connectToHost(QHostAddress::LocalHost, 8080, QIODevice::ReadWrite);
    while (!socket->waitForConnected(30000));
}

void TcpClient::sendMessage(std::string message){
    socket->write(message.c_str());
}
