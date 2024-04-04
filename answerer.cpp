#include "answerer.h"
#include "rtc/global.hpp"
#include "client.h"
#include <rtc/configuration.hpp>
#include <rtc/peerconnection.hpp>
#include <Qvector>
#include <sstream>


std::vector<std::string> split(std::string message, std::string sep){
    std::stringstream test("this_is_a_test_string");
    std::string segment;
    std::vector<std::string> splited;

    while(std::getline(test, segment, '_'))
    {
        splited.push_back(segment);
    }

    return splited;
}

answerer::answerer(QObject *parent)
    : QObject{parent}
{}

using std::shared_ptr;


void answerer::runAnswerer(std::string name){
    rtc::InitLogger(rtc::LogLevel::Warning);

    rtc::Configuration config;
    config.iceServers.emplace_back("stun.l.google.com:19302");
    TcpClient *client = new TcpClient("ANSWERER " +name);
    auto pc = std::make_shared<rtc::PeerConnection>(config);

    pc->onLocalDescription([](rtc::Description description) {
        std::cout << "Local Description (Paste this to the other peer):" << std::endl;
        std::cout << std::string(description) << std::endl;
    });

    pc->onLocalCandidate([](rtc::Candidate candidate) {
        std::cout << "Local Candidate (Paste this to the other peer after the local description):"
                  << std::endl;
        std::cout << std::string(candidate) << std::endl << std::endl;
    });

    pc->onStateChange([](rtc::PeerConnection::State state) {
        std::cout << "[State: " << state << "]" << std::endl;
    });
    pc->onGatheringStateChange([](rtc::PeerConnection::GatheringState state) {
        std::cout << "[Gathering State: " << state << "]" << std::endl;
    });

    // std::string message = client->receiveResponse();
    // auto splited_message = split(message, ",");
    // if (splited_message[0] == "CALL"){
    //     // emit
    // }

    shared_ptr<rtc::DataChannel> dc;
    pc->onDataChannel([&](shared_ptr<rtc::DataChannel> _dc) {
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

    bool exit = false;
    while (!exit) {
        std::cout
            << std::endl
            << "**********************************************************************************"
               "*****"
            << std::endl
            << "* 0: Exit /"
            << " 1: Enter remote description /"
            << " 2: Enter remote candidate /"
            << " 3: Send message /"
            << " 4: Print Connection Info *" << std::endl
            << "[Command]: ";

        int command = -1;
        std::cin >> command;
        std::cin.ignore();

        switch (command) {
        case 0: {
            exit = true;
            break;
        }
        case 1: {
            // Parse Description
            std::cout << "[Description]: ";
            std::string sdp, line;
            while (getline(std::cin, line) && !line.empty()) {
                sdp += line;
                sdp += "\r\n";
            }
            std::cout << sdp;
            pc->setRemoteDescription(sdp);
            break;
        }
        case 2: {
            // Parse Candidate
            std::cout << "[Candidate]: ";
            std::string candidate;
            getline(std::cin, candidate);
            pc->addRemoteCandidate(candidate);
            break;
        }
        case 3: {
            // Send Message
            if (!dc || !dc->isOpen()) {
                std::cout << "** Channel is not Open ** ";
                break;
            }
            std::cout << "[Message]: ";
            std::string message;
            getline(std::cin, message);
            dc->send(message);
            break;
        }
        case 4: {
            // Connection Info
            if (!dc || !dc->isOpen()) {
                std::cout << "** Channel is not Open ** ";
                break;
            }
            rtc::Candidate local, remote;
            std::optional<std::chrono::milliseconds> rtt = pc->rtt();
            if (pc->getSelectedCandidatePair(&local, &remote)) {
                std::cout << "Local: " << local << std::endl;
                std::cout << "Remote: " << remote << std::endl;
                std::cout << "Bytes Sent:" << pc->bytesSent()
                          << " / Bytes Received:" << pc->bytesReceived() << " / Round-Trip Time:";
                if (rtt.has_value())
                    std::cout << rtt.value().count();
                else
                    std::cout << "null";
                std::cout << " ms";
            } else {
                std::cout << "Could not get Candidate Pair Info" << std::endl;
            }
            break;
        }
        default: {
            std::cout << "** Invalid Command ** " << std::endl;
            break;
        }
        }
    }

    if (dc)
        dc->close();

    if (pc)
        pc->close();
}
