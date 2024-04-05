#ifndef OFFERER_H
#define OFFERER_H

#include <QObject>
#include <iostream>
#include "client.h"
#include "rtc/rtc.hpp"
#include <QJsonDocument>

class offerer : public QObject
{
    Q_OBJECT
public:
    explicit offerer(std::string _offerer_name, std::string name, QObject *parent=nullptr);
    void runOfferer();
    void send_message(std::string message);
    void set_remote(QString message);
    void startPhoneCall();
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
    std::string answerer_name;
    QTcpSocket* socket;
    std::string offerer_name;


public slots:
    void sendToDataChannel(const QByteArray &data);
    void recieveResponse();
    void connected();
};


#endif // OFFERER_H
