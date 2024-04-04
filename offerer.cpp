#include "client.h"
#include "offerer.h"
#include "rtc/global.hpp"
#include <rtc/configuration.hpp>
#include <rtc/peerconnection.hpp>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

// #include <rtc/rtc.hpp>

offerer::offerer(std::string name,std::string role, QObject *parent)
    : QObject{parent}
{
    myclient = new TcpClient(name,role);
    connect(myclient, &TcpClient::set_remote, this, &offerer::set_remote);
    myclient->runClient2();

}

void offerer::runAnswerer(std::string name){
    role = "answerer";
    std::cout << "\nAnswerer 1";
    rtc::InitLogger(rtc::LogLevel::Warning);
    initialize_peer_connection();

    std::cout << "\nAnswerer 2";
    pc->onDataChannel([&](std::shared_ptr<rtc::DataChannel> _dc) {
        std::cout << "[Got a DataChannel with label: " << _dc->label() << "]" << std::endl;
        dc = _dc;

        dc->onClosed(
            [&]() { std::cout << "[DataChannel closed: " << dc->label() << "]" << std::endl; });

        dc->onMessage([](auto data) {
            if (std::holds_alternative<std::string>(data)) {
                std::cout << "[Received message: " << std::get<std::string>(data) << "]"
                          << std::endl;
            }
        });
    });
    QThread::sleep(1);

    //send message in slot function !

}

QJsonDocument offerer::prepare_sdp_and_candidate_message(){
    QJsonObject sdp_candidate_object;
    sdp_candidate_object["name"] = "Ahmad";
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

void offerer::runOfferer(std::string answerer_name){
    role = "offerer";
    rtc::InitLogger(rtc::LogLevel::Warning);
    std::cout << "\n Offere 1";
    initialize_peer_connection();

    std::cout << "\n Offere 2";
    make_datachannel();

    QThread::sleep(1);
    std::cout << "\n Offere 3";
    QJsonDocument json_message = prepare_sdp_and_candidate_message();

    std::cout << "\n Offere 4";
    myclient->sendMessage(json_message.toJson().toStdString());
}

void offerer::set_remote(QString json_message){
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json_message.toUtf8());
    QJsonObject jsonObject = jsonDocument.object();
    pc->setRemoteDescription(rtc::Description(jsonObject["set_remote"].toString().toStdString()));
    QJsonArray candidate_array = jsonDocument.array();
    for (int i = 0; i < candidate_array.size(); ++i) {
        pc->addRemoteCandidate(rtc::Candidate(candidate_array.at(i).toString().toStdString()));
        qDebug() << "Element" << i << ":" << candidate_array.at(i).toString();
        std::cout << "Element" << i << ":" << candidate_array.at(i).toString().toStdString();
    }
    if (role == "answerer"){
        QJsonDocument json_message = prepare_sdp_and_candidate_message();
        myclient->sendMessage(json_message.toJson().toStdString());
    }
}

void offerer::make_datachannel(){
    dc = pc->createDataChannel("test"); // this is the offerer, so create a data channel

    dc->onOpen([&]() { std::cout << "[DataChannel open: " << dc->label() << "]" << std::endl; });

    dc->onClosed(
        [&]() { std::cout << "[DataChannel closed: " << dc->label() << "]" << std::endl; });

    dc->onMessage([](auto data) {
        if (std::holds_alternative<std::string>(data)) {
            std::cout << "[Received: " << std::get<std::string>(data) << "]" << std::endl;
        }
    });
}


void offerer::initialize_peer_connection(){
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
