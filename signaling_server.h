// #ifndef LISTNER_H
// #define LISTNER_H


// #include <QObject>
// #include <QTcpServer>


// class Listner : public QObject {
//     Q_OBJECT

// public:
//     Listner(qint16 port = 8080);

// private slots:
//     void handleNewConnection();
//     void handleClientData();
//     void handleClientDisconnect();

// private:
//     QTcpServer* server;
// };

// #endif // LISTNER_H

#ifndef LISTNER_H
#define LISTNER_H


#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <vector>

struct Client{
    std::string name;
    std::string address;
    int port;
};

class TcpServer : public QObject {
    Q_OBJECT

public:
    TcpServer();
    // void connectServer();
    void newConnection();
    void runServer2();
    void handleRequests(std::string message, QTcpSocket *socket);
    std::string extractCommandInfo(std::string data);
    Client* findUserByName(std::string name);
    void sendResponse(std::string response, QTcpSocket *socket);
private slots:
    void receiveMessage();
    // void connected();
private:
    QTcpServer* server;
    QTcpSocket* incomingSocket;
    std::vector<Client*> clients;
};

#endif
