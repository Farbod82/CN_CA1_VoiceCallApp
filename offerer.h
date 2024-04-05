#ifndef OFFERER_H
#define OFFERER_H

#include <QObject>
#include <iostream>
#include "audio_capture.h"
#include "client.h"
#include "rtc/rtc.hpp"
#include <QJsonDocument>

class offerer : public QObject
{
    Q_OBJECT
public:
    // explicit offerer(std::string name, QObject *parent = nullptr);
    void runOfferer(std::string answerer_name);
    void runAnswerer(std::string name);
    void send_message(std::string message);
    void close_connection();
    explicit offerer(std::string name, std::string role, AudioCapture *ac, QObject *parent=nullptr);
    void set_remote(QString message);
    void startPhoneCall();
    void test();
signals:
private:
    AudioCapture *ac;
    std::vector<std::string> local_candidates;
    std::string description;
    TcpClient* myclient;
    std::shared_ptr<rtc::PeerConnection> pc;
    std::shared_ptr<rtc::DataChannel> dc;
    void make_datachannel();
    void initialize_peer_connection();
    QJsonDocument prepare_sdp_and_candidate_message();
    std::string offerer_name;
    std::string answerer_name;
    QTcpSocket* socket;
    bool phone_connected;


public slots:
    void sendToDataChannel(const QByteArray& data);
    void recieveResponse();
    void connected();
};


#endif // OFFERER_H
