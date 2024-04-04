#ifndef OFFERER_H
#define OFFERER_H

#include <QObject>
#include <iostream>
#include "client.h"
#include "rtc/rtc.hpp"

class offerer : public QObject
{
    Q_OBJECT
public:
    explicit offerer(std::string name, QObject *parent = nullptr);

    void runOfferer(std::string answerer_name);
signals:
private:
    std::vector<std::string> local_candidates;
    std::string description;
    TcpClient* myclient;
    std::shared_ptr<rtc::PeerConnection> pc;
    std::shared_ptr<rtc::DataChannel> dc;
    void make_datachannel();
    void initialize_peer_connection();


private slots:
    void set_remote(QString json_message);
};


#endif // OFFERER_H
