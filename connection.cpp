#include "connection.h"
#include "rtc/rtc.hpp"
#include "signaling_server.h"
#include "audiorecorder.h"


connection::connection(QObject* parent) : QObject(parent), pc(config) {
    rtc::InitLogger(rtc::LogLevel::Debug);
    config.iceServers.emplace_back("stun:stun.l.google.com:19302");

    pc->onStateChange(
    [](rtc::PeerConnection::State state) { std::cout << "State: " << state << std::endl; });

    pc->onLocalCandidate([&local_candidates](rtc::Candidate candidate) {
        std::cout << "candidate added" << std::endl;
        local_candidates.push_back( std::string(candidate) );
    });

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

    pc.onDataChannel([](rtc::DataChannel channel) {
        channel.onMessage([](const rtc::Message& message) {
            // Handle incoming audio data
            // Emit signal to pass audio data to the application
            emit audioReceived(QByteArray(message.data(), message.size()));
        });
    });

    make_data_channel();
}

void connection::init() {
    rtc::DataChannelInit();
    audio_recorder.record();
    // Initialize WebRTC library
    // For example, rtc::Initialize();
    // Start audio capture and rendering
}

void connection::handleAudioData(const QByteArray& audioData) {
    std::cout<<static_cast<QString>(audioData);
}


void offere(){
    try{
     //    pc->onGatheringStateChange([pc](rtc::PeerConnection::GatheringState state) {
        // std::cout << "Gathering State: " << state << std::endl;
        // if (state == rtc::PeerConnection::GatheringState::Complete) {
        //     auto description = pc->localDescription();
        //     json message = {{"type", description->typeString()},
        // 		    {"sdp", std::string(description.value())}};
        //     local_sdp = message.dump();
        //     std::cout << message << std::endl;
        // }
     //    });

        // int sock = socket(AF_INET, SOCK_DGRAM, 0);
        // struct sockaddr_in addr = {};
        // addr.sin_family = AF_INET;
        // addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        // addr.sin_port = htons(6000);

        // if (bind(sock, reinterpret_cast<const sockaddr *>(&addr), sizeof(addr)) < 0)
        //     throw std::runtime_error("Failed to bind UDP socket on 127.0.0.1:6000");

        // int rcvBufSize = 212992;
        // setsockopt(sock, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char *>(&rcvBufSize),
        //             sizeof(rcvBufSize));

        // const rtc::SSRC ssrc = 42;
        // rtc::Description::Video media("video", rtc::Description::Direction::SendOnly);
        // media.addH264Codec(96); // Must match the payload type of the external h264 RTP stream
        // media.addSSRC(ssrc, "video-send");
        // auto track = pc->addTrack(media);

        pc->setLocalDescription(rtc::Description::Type::Offer);



        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
        std::cerr << "Error creating sdp change socket.\n";
        return 1;
        }
        std::cout << "sockfd created\n";
        sockaddr_in serv_addr;
        std::memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr("192.168.43.19");
        serv_addr.sin_port = htons(PORT);

        if (bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Error binding sdp socket.\n";
        return 1;
        }

        std::cout << "binded\n";

        if (listen(sockfd, 1) < 0) {
        std::cerr << "Error listening on sdp socket.\n";
        return 1;
        }
        std::cout << "listening\n";

        int connfd = accept(sockfd, NULL, NULL);
        if (connfd < 0) {
        std::cerr << "Error accepting connection.\n";
        return 1;
        }
        std::cout << "accepted\n";

        const char* sdp_to_send= local_sdp.c_str();
        int n;
        send(connfd, sdp_to_send, std::strlen(sdp_to_send), 0);

        char buffer[BUFFER_SIZE] = {0};
        std::memset(buffer, 0, BUFFER_SIZE);
        std::cout << "sent\n";

        n = recv(connfd, buffer, BUFFER_SIZE, 0);
        if (n < 0) {
        std::cerr << "Error receiving message.\n";
        } else if (n == 0) {
        std::cerr << "Connection closed by client.\n";

        }
        std::cout << "SDP Received \n" << buffer << std::endl;


        try {
        json j = json::parse(buffer);
        rtc::Description answer(j["sdp"].get<std::string>(), j["type"].get<std::string>());
        pc->setRemoteDescription(answer);
        } catch (const json::parse_error &e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        }


        int candidate_number = local_candidates.size();
        send(connfd, &candidate_number, 4,0); //send number of canditate to send

        for(int i = 0; i < candidate_number; i++){
        send(connfd, local_candidates[i].c_str(), strlen(local_candidates[i].c_str()), 0 );
        }
        std::vector<std::string> remote_candidates_vector;
        //wait for receiver candidate count
        int remote_cand_count=0;
        recv(connfd,&remote_cand_count,4,0);

        if(remote_cand_count<=0) {
        std::cout<<"Error getting remote cand count"<<std::endl;
        }
        std::cout << "remote cand count " << remote_cand_count << std::endl;

        // wait for receiver candiates

        // for(int i=0;i<remote_cand_count-1;i++) {
        //     char remote_candidates[BUFFER_SIZE]={0};
        //     recv(connfd, remote_candidates, BUFFER_SIZE, 0);
        //     remote_candidates_vector.push_back(std::string(remote_candidates));
        //     std::cout << "candidate " << i << " " << remote_candidates << std::endl;
        // }

        char remote_candidates[BUFFER_SIZE]={0};
        recv(connfd, remote_candidates, BUFFER_SIZE, 0);
        std::cout << remote_candidates << std::endl;
        pc->addRemoteCandidate(rtc::Candidate(std::string(remote_candidates)));

        // for(auto const& cand:remote_candidates_vector ) {
        //     std::cout<<"Remote candidates:\n"<<cand<<std::endl;
        // }
        // for(int i = 0 ; i < remote_cand_count; i++){
        //     std::cout << "\ni >>> " << i << std::endl;
        //     pc->addRemoteCandidate((rtc::Candidate(remote_candidates_vector[i])));
        // }

        std::cout << "[Message]: ";
        std::string message;
        getline(std::cin, message);
        dc->send(message);
        close(connfd);
        close(sockfd);

}

catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
}


void connection::make_data_channel(){

    dc = pc.createDataChannel("test");

    dc->onOpen([&]() { std::cout << "[DataChannel open: " << dc->label() << "]" << std::endl; });

    dc->onClosed(
    [&]() { std::cout << "[DataChannel closed: " << dc->label() << "]" << std::endl; });

    dc->onMessage([](auto data) {
    if (std::holds_alternative<std::string>(data)) {
        std::cout << "[Received: " << std::get<std::string>(data) << "]" << std::endl;
    }
    });
}

connection::~connection(){
    dc.close();
}
