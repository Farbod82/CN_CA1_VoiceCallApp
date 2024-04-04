// #ifndef CONNECTION_H
// #define CONNECTION_H

// #include <QObject>
// #include "rtc/rtc.hpp"

// class connection : public QObject
// {
//     Q_OBJECT
// public:
//     explicit connection(QObject* parent = nullptr);
//     ~connection();
//     void init();
//     void make_data_channel();
// signals:
//     void audioReceived(const QByteArray& audioData);
// private slots:
//     // void handleAudioData(const QByteArray& audioData);
// private:
//     rtc::Configuration config;
//     rtc::PeerConnection pc;
//     rtc::DataChannel dc;
//     std::vector<std::string> local_candidates;
// };

// #endif // CONNECTION_H
