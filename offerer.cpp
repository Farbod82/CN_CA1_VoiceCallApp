#include "client.h"
#include "offerer.h"
#include "rtc/global.hpp"
#include <rtc/configuration.hpp>
#include <rtc/peerconnection.hpp>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

// #include <rtc/rtc.hpp>

offerer::offerer(QObject *parent)
    : QObject{parent}
{

}

void offerer::runOfferer(std::string name,std::string answerer_name){
    TcpClient* myclient = new TcpClient("CONNECT "+name);
    myclient->runClient2();
    rtc::InitLogger(rtc::LogLevel::Warning);
    rtc::Configuration config;
    config.iceServers.emplace_back("stun.l.google.com:19302");

    auto pc = std::make_shared<rtc::PeerConnection>(config);

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

    auto dc = pc->createDataChannel("test"); // this is the offerer, so create a data channel

    dc->onOpen([&]() { std::cout << "[DataChannel open: " << dc->label() << "]" << std::endl; });

    dc->onClosed(
        [&]() { std::cout << "[DataChannel closed: " << dc->label() << "]" << std::endl; });

    dc->onMessage([](auto data) {
        if (std::holds_alternative<std::string>(data)) {
            std::cout << "[Received: " << std::get<std::string>(data) << "]" << std::endl;
        }
    });

    QThread::sleep(1);

    QJsonObject sdp_candidate_object;
    sdp_candidate_object["sdp"] = QString::fromStdString(description);

    QJsonArray candidates;
    for (auto cand : local_candidates){
        candidates.append(QString::fromStdString(cand));
    }
    sdp_candidate_object["candidates"] = candidates;
    QJsonDocument json_message(sdp_candidate_object);
    myclient->sendMessage(json_message.toJson().toStdString());
}
