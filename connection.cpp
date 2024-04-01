#include "connection.h"
#include "rtc/rtc.h"
#include "signaling_server.h"
#include "audiorecorder.h"


connection::connection(QObject* parent) : QObject(parent), pc(config) {
    config.iceServers.emplace_back("stun:stun.l.google.com:19302");

    pc.onLocalDescription([](rtc::Description sdp) {
        // Send the SDP to the remote peer
        MY_SEND_DESCRIPTION_TO_REMOTE(std::string(sdp));
    });

    pc.onLocalCandidate([](rtc::Candidate candidate) {
        // Send the candidate to the remote peer
        MY_SEND_CANDIDATE_TO_REMOTE(candidate.candidate(), candidate.mid());
    });

    pc.onDataChannel([](rtc::DataChannel channel) {
        channel.onMessage([](const rtc::Message& message) {
            // Handle incoming audio data
            // Emit signal to pass audio data to the application
            emit audioReceived(QByteArray(message.data(), message.size()));
        });
    });
}

void connection::init() {
    rtc::DataChannelInit();
    audio_recorder.record();
    // Initialize WebRTC library
    // For example, rtc::Initialize();
    // Start audio capture and rendering
}

void connection::handleAudioData(const QByteArray& audioData) {
    // Process and play audio data
}


void signal_peer_connection(){
    rtc::Configuration config;
    config.iceServers.emplace_back("mystunserver.org:3478");

    rtc::PeerConnection pc(config);

    pc.onLocalDescription([](rtc::Description sdp) {
        // Send the SDP to the remote peer
        MY_SEND_DESCRIPTION_TO_REMOTE(std::string(sdp));
    });

    pc.onLocalCandidate([](rtc::Candidate candidate) {
        // Send the candidate to the remote peer
        MY_SEND_CANDIDATE_TO_REMOTE(candidate.candidate(), candidate.mid());
    });

    MY_ON_RECV_DESCRIPTION_FROM_REMOTE([&pc](std::string sdp) {
        pc.setRemoteDescription(rtc::Description(sdp));
    });

    MY_ON_RECV_CANDIDATE_FROM_REMOTE([&pc](std::string candidate, std::string mid) {
        pc.addRemoteCandidate(rtc::Candidate(candidate, mid));
    });
}

void make_data_channel(){
    auto dc = pc.createDataChannel("test");

    dc->onOpen([]() {
        std::cout << "Open" << std::endl;
    });

    dc->onMessage([](std::variant<rtc::binary, rtc::string> message) {
        if (std::holds_alternative<rtc::string>(message)) {
            std::cout << "Received: " << get<rtc::string>(message) << std::endl;
        }
    });
}
