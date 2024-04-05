#include "signaling_server.h"
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <sstream>
#include <QJsonDocument>
#include <QJsonObject>


struct Client;

TcpServer::TcpServer() : QObject(), server(new QTcpServer()) {}

std::vector<std::string> TcpServer::split(std::string str, char sep){
    std::istringstream iss(str);
    std::vector<std::string> words;
    std::string word;
    while (std::getline(iss, word, sep)) {
        words.push_back(word);
    }
    return words;
}

Client* TcpServer::find_user_by_name(std::string name){
    for (auto c: clients){
        if(c->name.compare(name) == 0){
            return c;
        }
    }
    return nullptr;
}

void TcpServer::send_response(std::string response,QTcpSocket* socket){
    socket->write(response.c_str());
}

void TcpServer::handle_requests(std::string message,QTcpSocket* socket){
    if(strncmp(message.c_str(),"CONNECT",7) ==0){
        std::vector<std::string> words = split(message,' ');
        std::string address = socket->peerAddress().toString().toStdString();
        int port = socket->peerPort();
        Client* new_client = new Client;
        new_client->port = port;
        new_client->name = words[1];
        new_client->address = address;
        new_client->socket = socket;
        clients.push_back(new_client);
    }
    else{
        QString q_string_message = QString::fromStdString(message);
        QJsonDocument jsonDocument = QJsonDocument::fromJson(q_string_message.toUtf8());
        QJsonObject jsonObject = jsonDocument.object();
        QString reciever_name = jsonObject["reciever"].toString();
        QString offerer_name = jsonObject["sender"].toString();
        Client* reciever = find_user_by_name(reciever_name.toStdString());
        if (offerer_name.compare("NoNeed") != 0){
            Client* new_client = new Client;
            new_client->name = offerer_name.toStdString();
            new_client->socket = socket;
            clients.push_back(new_client);
        }
        else{
            qDebug() << "reached";
        }
        reciever->socket->write(message.c_str());
    }
    return;
}

void TcpServer::receive_message() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    std::string message = socket->readAll().data();
    qDebug() << "some one sent something";
    qDebug() << message;
    handle_requests(message,socket);
}

void TcpServer::new_connection() {
    QTcpSocket* socket = server->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead,this,&TcpServer::receive_message);
}

void TcpServer::run_server2() {
    connect(server, &QTcpServer::newConnection,this, &TcpServer::new_connection);
    server->listen(QHostAddress::Any, 8080);
}

