#include "audio_player.h"

#include <QAudioFormat>
#include <QAudioSink>
#include <QMediaDevices>
#include <QObject>

AudioPlayer::AudioPlayer(QObject *parent)
    : QObject{parent} {

}




void::AudioPlayer::startPlaying(){
    devices = new QMediaDevices();
    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    QAudioSink* audioSink = new QAudioSink{devices->defaultAudioOutput(),
                                 format,nullptr};
    audioSink->reset();

    io = audioSink->start();
}

void AudioPlayer::playData(std::vector<std::byte> message){
    // QString str = QString::fromStdString(message);
    QByteArray* data = new QByteArray(reinterpret_cast<const char*>(message.data()), message.size());
    int len = message.size();
    if(len)
        io->write(*data,len);
}
