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

void AudioPlayer::playData(std::string message){
    QString str = QString::fromStdString(message);
    QByteArray data = str.toUtf8();
    int len = data.size();
    if(len)
        io->write(data.constData(),len);
}
