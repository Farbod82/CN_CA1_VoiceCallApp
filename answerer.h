#ifndef ANSWERER_H
#define ANSWERER_H


#include <QObject>
#include <iostream>
#include "audio_player.h"
#include "rtc/rtc.hpp"
#include <QJsonDocument>
#include "audio_capture.h"


class answerer : public QObject
{
    Q_OBJECT
public:
    explicit answerer(std::string name, QString server_ip, QObject *parent=nullptr);
    void run_answerer();
    void set_remote(QString message);
    void send_message(std::string message);
    void close_connection();
    void recieveAudioMessage();
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
    QString offerer_name;
    bool phone_connected;
    bool mic_connected;
    AudioPlayer* ap;
    AudioCapture* ac;
    std::vector<std::byte> audio_message;


public slots:
    void sendToDataChannel(const QByteArray &data);
    void connected();
    void recieve_response();
};



#endif // ANSWERER_H
