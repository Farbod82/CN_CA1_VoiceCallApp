#ifndef OFFERER_H
#define OFFERER_H

#include <QObject>
#include <iostream>
#include "audio_capture.h"
#include "audio_player.h"
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
    void startPhoneCall();
    void test();
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
    AudioCapture *ac;
    AudioPlayer *ap;
    std::vector<std::byte> audio_message;
    std::string offerer_name;
    bool audio_connected;
    bool phone_connected;


public slots:
    void sendToDataChannel(const QByteArray& data);
    void recieveResponse();
    void connected();
};


#endif // OFFERER_H
