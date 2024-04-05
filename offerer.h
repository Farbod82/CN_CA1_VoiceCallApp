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
    explicit offerer(std::string _offerer_name, std::string name, QString server_ip, QObject *parent=nullptr);
    void run_offerer();
    void send_message(std::string message);
    void set_remote(QString message);
    void start_phone_call();
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
    void send_to_datachannel(const QByteArray &data);
    void recieve_response();
    void connected();
};


#endif // OFFERER_H
