#ifndef LISTNER_H
#define LISTNER_H


#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <vector>

struct Client{
    std::string name;
    std::string address;
    QTcpSocket* socket;
    int port;
};

class TcpServer : public QObject {
    Q_OBJECT

public:
    TcpServer();
    // void connectServer();
    void new_connection();
    void run_server2();
    void handle_requests(std::string message, QTcpSocket *socket);
    Client* find_user_by_name(std::string name);
    void send_response(std::string response, QTcpSocket *socket);
    std::vector<std::string> extract_command_info(std::string data, std::string message);
private slots:
    void receive_message();
    // void connected();
private:
    QTcpServer* server;
    std::vector<Client*> clients;
};

#endif


