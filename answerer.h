#ifndef ANSWERER_H
#define ANSWERER_H


#include <QObject>
#include <iostream>
#include "audio_player.h"
#include "client.h"
#include "rtc/rtc.hpp"
#include <QJsonDocument>
#include "audio_capture.h"


class answerer : public QObject
{
    Q_OBJECT
public:
    // explicit offerer(std::string name, QObject *parent = nullptr);
    void runOfferer(std::string answerer_name);
    void runAnswerer();
    void send_message(std::string message);
    void close_connection();
    void set_remote(QString message);
    explicit answerer(std::string name, std::string role, AudioPlayer *_ap,AudioCapture *_ac, QObject *parent=nullptr);
    void recieveAudioMessage();
signals:
private:
    std::vector<std::string> local_candidates;
    std::string description;
    TcpClient* myclient;
    std::shared_ptr<rtc::PeerConnection> pc;
    std::shared_ptr<rtc::DataChannel> dc;
    void make_datachannel();
    void initialize_peer_connection();
    QJsonDocument prepare_sdp_and_candidate_message();
    std::string role;
    QTcpSocket* socket;
    std::string _name;
    bool phone_connected;
    AudioPlayer* ap;
    AudioCapture* ac;
    std::vector<std::byte> audio_message;


public slots:
    void recieveResponse();
    void connected();
};



#endif // ANSWERER_H
