#include "answerer.h"
#include "audio_player.h"
#include "rtc/global.hpp"
#include <rtc/configuration.hpp>
#include <rtc/peerconnection.hpp>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

using std::shared_ptr;

answerer::answerer(std::string name, QString server_ip, AudioPlayer* _ap,AudioCapture* _ac, QObject *parent)
    : QObject{parent},socket(new QTcpSocket())
{
    ap = _ap;
    ac = _ac;
    _name = name;
    phone_connected = false;
    mic_connected = false;
    connect(socket, &QTcpSocket::connected, this, &answerer::connected);
    connect(socket,&QTcpSocket::readyRead,this, &answerer::recieve_response);
    socket->connectToHost(server_ip, 8080, QIODevice::ReadWrite);
    while (!socket->waitForConnected(30000));
}

void answerer::connected(){
    qDebug() << "connected";
    socket->write(("CONNECT " + _name).c_str());
}

void answerer::recieve_response(){
    qDebug() << "Im fking here";
    QTcpSocket* _socket = qobject_cast<QTcpSocket*>(sender());
    std::string message = _socket->readAll().data();
    QString q_string_message = QString::fromStdString(message);
    // std::cout << "response from server: " << message;

    QJsonDocument jsonDocument = QJsonDocument::fromJson(q_string_message.toUtf8());
    if (!jsonDocument.isNull()) {
        if (jsonDocument.isObject()) {
            QJsonObject jsonObject = jsonDocument.object();
            if (jsonObject["type"] == "set_remote"){
                set_remote(q_string_message);
            }
        }
    }
}

void answerer::send_to_datachannel(const QByteArray& data){
    if (phone_connected){
        // dc->sendBuffer(data);
        // std::string d = QString::fromUtf8(data).toStdString();
        // dc->send();
        // const char* charBuffer = data.constData();
        // dc->send(data.constData());
        // std::vector<std::byte> d(data.begin(), data.end());
        // qDebug() << "im sending as answerer";
        std::vector<char> d(data.begin(), data.end());
        std::vector<std::byte> bytes;
        for (char &c : d) {
            bytes.push_back(static_cast<std::byte>(c));
        }

        dc->sendBuffer(bytes);

    }
}
void answerer::run_answerer(){
    std::cout << "\nAnswerer 1";
    rtc::InitLogger(rtc::LogLevel::Warning);
    initialize_peer_connection();    
    std::cout << "\nAnswerer 2";

    pc->onDataChannel([&](shared_ptr<rtc::DataChannel> _dc) {
        std::cout << "[Got a DataChannel with label: " << _dc->label() << "]" << std::endl;
        dc = _dc;


        dc->onClosed(
            [&]() { std::cout << "[DataChannel closed: " << dc->label() << "]" << std::endl; });

        dc->onMessage([this](auto data) {
            // qDebug() << "got you";
            if (std::holds_alternative<std::vector<std::byte>>(data)) {
                // auto d = std::get<std::vector<std::byte>>(data);
                // QByteArray byteArray(reinterpret_cast<const char*>(d.data()), d.size());
                audio_message = std::get<std::vector<std::byte>>(data);
                phone_connected =true;
            }
            // ap->playData(byteArray.constData());
        });
    });
    // QThread::sleep(1);
    socket->waitForReadyRead();
    QJsonDocument json_message = prepare_sdp_and_candidate_message();
    // QThread::sleep(1);
    socket->write(json_message.toJson().toStdString().c_str());
    socket->waitForBytesWritten();
    // socket->waitForReadyRead();
    mic_connected = true;

    ac->startRecord();
    while(1){
        connect(ac,&AudioCapture::bufferReady,this,&answerer::send_to_datachannel);
        while( !phone_connected){
            continue;
        }
        ap->playData(audio_message);
        QByteArray mic_audio = ac->readAny();
        send_to_datachannel(mic_audio);
        phone_connected = false;
    }



    //send message in slot function !

}

QJsonDocument answerer::prepare_sdp_and_candidate_message(){
    QJsonObject sdp_candidate_object;
    sdp_candidate_object["reciever"] = offerer_name;
    sdp_candidate_object["sender"] = "NoNeed";
    sdp_candidate_object["type"] = "set_remote";
    sdp_candidate_object["sdp"] = QString::fromStdString(description);

    QJsonArray candidates;
    for (auto cand : local_candidates){
        candidates.append(QString::fromStdString(cand));
    }
    sdp_candidate_object["candidates"] = candidates;
    QJsonDocument json_message(sdp_candidate_object);
    return json_message;
}

void answerer::set_remote(QString message){
    QJsonDocument jsonDocument = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jsonObject = jsonDocument.object();
    pc->setRemoteDescription(rtc::Description(jsonObject["sdp"].toString().toStdString()));
    QJsonArray candidate_array = jsonObject["candidates"].toArray();
    for (int i = 0; i < candidate_array.size(); ++i) {
        pc->addRemoteCandidate(rtc::Candidate(candidate_array.at(i).toString().toStdString()));
    }
    offerer_name = jsonObject["sender"].toString();
    qDebug() << "set answerer done";
}


void answerer::initialize_peer_connection(){
    rtc::Configuration config;
    config.iceServers.emplace_back("stun.l.google.com:19302");

    pc = std::make_shared<rtc::PeerConnection>(config);

    pc->onLocalDescription([this](rtc::Description _description) {
        description = std::string(_description);
        qDebug() << "hey";
    });

    pc->onLocalCandidate([this](rtc::Candidate _candidate) {
        local_candidates.push_back(std::string(_candidate));
        qDebug() << "hoy";
    });


    pc->onStateChange([](rtc::PeerConnection::State state) {
        std::cout << "[State: " << state << "]" << std::endl;
    });

    pc->onGatheringStateChange([](rtc::PeerConnection::GatheringState state) {
        std::cout << "[Gathering State: " << state << "]" << std::endl;
    });
}

void answerer::send_message(std::string message){
    if (!dc || !dc->isOpen()) {
        std::cout << "** Channel is not Open ** ";
        return;
    }
    dc->send(message);
}

void answerer::close_connection(){
    if (dc)
        dc->close();

    if (pc)
        pc->close();
}
