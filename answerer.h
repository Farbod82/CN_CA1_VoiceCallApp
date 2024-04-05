#ifndef ANSWERER_H
#define ANSWERER_H


#include <QObject>
#include <iostream>
#include "client.h"
#include "rtc/rtc.hpp"
#include <QJsonDocument>

class answerer : public QObject
{
    Q_OBJECT
public:
    explicit answerer(std::string name, QObject *parent=nullptr);
    void runAnswerer();
    void set_remote(QString message);
    void send_message(std::string message);
    void close_connection();
signals:
private:
    void make_datachannel();
    void initialize_peer_connection();
    QJsonDocument prepare_sdp_and_candidate_message();
    std::vector<std::string> local_candidates;
    std::string description;
    std::shared_ptr<rtc::PeerConnection> pc;
    std::shared_ptr<rtc::DataChannel> dc;
    QTcpSocket* socket;
    std::string _name;


public slots:
    void recieveResponse();
    void connected();
};



#endif // ANSWERER_H
