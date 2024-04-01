#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include "audiorecorder.h"

class connection : public QObject
{
public:
    explicit connection(QObject* parent = nullptr);
    void init();
signals:
    void audioReceived(const QByteArray& audioData);
private slots:
    void handleAudioData(const QByteArray& audioData);
private:
    rtc::Configuration config;
    rtc::PeerConnection pc;
    AudioRecorder audio_recorder;
};

#endif // CONNECTION_H
