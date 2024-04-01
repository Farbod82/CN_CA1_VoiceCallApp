#include "signaling_server.h"
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <sstream>


struct Client;


TcpServer::TcpServer() : QObject(), server(new QTcpServer()) {}



std::string TcpServer::extractCommandInfo(std::string data){
    std::istringstream iss(data);
    std::vector<std::string> words;

    std::string word;
    while (std::getline(iss, word, ' ')) {
        words.push_back(word);
    }
    return words[1];
}


Client* TcpServer::findUserByName(std::string name){
    for (auto c: clients){
        if(c->name.compare(name) == 0){
            return c;
        }
    }
    return nullptr;

}
void TcpServer::sendResponse(std::string response,QTcpSocket* socket){
    socket->write(response.c_str());
}

void TcpServer::handleRequests(std::string message,QTcpSocket* socket){
    if(strncmp(message.c_str(),"CONNECT",7) ==0){
        std::string name = extractCommandInfo(message);
        std::string address = socket->peerAddress().toString().toStdString();
        int port = socket->peerPort();
        Client* new_client = new Client;
        new_client->port = port;
        new_client->name = name;
        new_client->address = address;
        TcpServer::clients.push_back(new_client);
    }
    else if(strncmp(message.c_str(),"CALL",4)==0){
        int response_port;
        std::string name = extractCommandInfo(message);
        Client* reciever = findUserByName(name);
        std::string response = "RESPONSE " + reciever->address + " " + std::to_string(reciever->port);
        sendResponse(response,socket);
    }
    return;
}

void TcpServer::receiveMessage() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    std::string message = socket->readAll().data();
    qDebug() << message;
    handleRequests(message,socket);
}

void TcpServer::newConnection() {
    QTcpSocket* socket = server->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead,this,&TcpServer::receiveMessage);
}

void TcpServer::runServer2() {
    connect(server, &QTcpServer::newConnection,this, &TcpServer::newConnection);
    server->listen(QHostAddress::LocalHost, 8080);
}
