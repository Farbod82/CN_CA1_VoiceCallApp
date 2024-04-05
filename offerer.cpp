#include "audio_capture.h"
#include "client.h"
#include "offerer.h"
#include "rtc/global.hpp"
#include <rtc/configuration.hpp>
#include <rtc/peerconnection.hpp>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

// #include <rtc/rtc.hpp>

offerer::offerer(std::string _offerer_name,std::string _answerer_name, QObject *parent)
    : QObject{parent},socket(new QTcpSocket())
{
    offerer_name = _offerer_name;
    answerer_name = _answerer_name;
    connect(socket, &QTcpSocket::connected, this, &offerer::connected);
    connect(socket,&QTcpSocket::readyRead,this, &offerer::recieveResponse);
    socket->connectToHost(QHostAddress::LocalHost, 8080, QIODevice::ReadWrite);
    while (!socket->waitForConnected(30000));

}

void offerer::startPhoneCall(){
    AudioCapture* ac = new AudioCapture(dc);

}

void offerer::sendToDataChannel(const QByteArray& data){
    // dc->sendBuffer(data);
    dc->send("hello");
}

void offerer::connected(){
    qDebug() << "connected";
}

void offerer::recieveResponse(){
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


QJsonDocument offerer::prepare_sdp_and_candidate_message(){
    QJsonObject sdp_candidate_object;
    sdp_candidate_object["reciever"] =  answerer_name.c_str();
    sdp_candidate_object["sender"]  = offerer_name.c_str();
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

void offerer::runOfferer(){
    rtc::InitLogger(rtc::LogLevel::Warning);
    // std::cout << "\n Offere 1";
    initialize_peer_connection();

    // std::cout << "\n Offere 2";
    make_datachannel();

    // std::cout << "\n Offere 3";
    // QThread::sleep(1);
    QJsonDocument json_message = prepare_sdp_and_candidate_message();

    // std::cout << "\n Offerer 4";
    socket->write(json_message.toJson().toStdString().c_str());
    socket->waitForReadyRead();
    // make_datachannel();
    socket->waitForReadyRead();
    // dc->send("hellloooooo");

}

void offerer::set_remote(QString message){
    // QThread::sleep(5);
    QJsonDocument jsonDocument = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jsonObject = jsonDocument.object();
    pc->setRemoteDescription(rtc::Description(jsonObject["sdp"].toString().toStdString()));
    QJsonArray candidate_array = jsonObject["candidates"].toArray();
    for (int i = 0; i < candidate_array.size(); ++i) {
        pc->addRemoteCandidate(rtc::Candidate(candidate_array.at(i).toString().toStdString()));
        // qDebug() << "Element" << i << ":" << candidate_array.at(i).toString();
        // std::cout << "Element" << i << ":" << candidate_array.at(i).toString().toStdString();
    }
    qDebug() << "set offerer done";

}

void offerer::make_datachannel(){
    dc = pc->createDataChannel("test"); // this is the offerer, so create a data channel

    dc->onOpen([&]() { std::cout << "[DataChannel open: " << dc->label() << "]" << std::endl;
        startPhoneCall();

    });

    dc->onClosed(
        [&]() { std::cout << "[DataChannel closed: " << dc->label() << "]" << std::endl; });

    dc->onMessage([](auto data) {
        if (std::holds_alternative<std::string>(data)) {
            std::cout << "[Received: " << std::get<std::string>(data);
        }
    });
}


void offerer::initialize_peer_connection(){
    rtc::Configuration config;
    config.iceServers.emplace_back("stun.l.google.com:19302");

    pc = std::make_shared<rtc::PeerConnection>(config);

    pc->onLocalDescription([this](rtc::Description _description) {
        description = std::string(_description);
        // qDebug() << "hey";
    });

    pc->onLocalCandidate([this](rtc::Candidate _candidate) {
        local_candidates.push_back(std::string(_candidate));
        // qDebug() << "hoy";
    });


    pc->onStateChange([](rtc::PeerConnection::State state) {
        std::cout << "[State: " << state << "]" << std::endl;
    });

    pc->onGatheringStateChange([](rtc::PeerConnection::GatheringState state) {
        std::cout << "[Gathering State: " << state << "]" << std::endl;
    });
}

void offerer::send_message(std::string message){
    if (!dc || !dc->isOpen()) {
        std::cout << "** Channel is not Open ** ";
        return;
    }
    dc->send(message);
}

void offerer::close_connection(){
    if (dc)
        dc->close();

    if (pc)
        pc->close();
}
