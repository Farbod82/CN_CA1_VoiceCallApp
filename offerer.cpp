#include "client.h"
#include "offerer.h"
#include "rtc/global.hpp"
#include <rtc/configuration.hpp>
#include <rtc/peerconnection.hpp>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

// #include <rtc/rtc.hpp>

offerer::offerer(std::string name, QObject *parent)
    : QObject{parent}
{
    myclient = new TcpClient("CONNECT "+name);
    connect(myclient, &TcpClient::set_remote, this, &offerer::set_remote);
    std::cout << "aaaaaaaaaaaaaa" << std::endl;

}

void offerer::runOfferer(std::string answerer_name){
    myclient->runClient2();
    rtc::InitLogger(rtc::LogLevel::Warning);

    initialize_peer_connection();

    make_datachannel();

    QThread::sleep(1);

    QJsonObject sdp_candidate_object;
    sdp_candidate_object["type"] = "set_remtote";
    sdp_candidate_object["sdp"] = QString::fromStdString(description);

    QJsonArray candidates;
    for (auto cand : local_candidates){
        candidates.append(QString::fromStdString(cand));
    }
    sdp_candidate_object["candidates"] = candidates;
    QJsonDocument json_message(sdp_candidate_object);
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
