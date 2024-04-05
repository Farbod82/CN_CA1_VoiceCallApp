#include "audio_capture.h"

#include <QAudioSource>
#include <QMediaDevices>
#include <QaudioFormat>
#include <QDebug>

#include <rtc/datachannel.hpp>



using std::shared_ptr;




void AudioCapture::audioSourceData(QIODevice * device, QAudioSource* src){
    qint64 len = src->bytesAvailable();

    const int BufferSize = 4096;
    // if (len > BufferSize)
    len = BufferSize;

    QByteArray buffer(len, 0);
    qint64 l = device->read(buffer.data(), len);
    QByteArray buff = QByteArray::fromRawData(buffer,l);
    // for (int i =0; i <buff.size();i++){
    //     qDebug() << buff.at(i);
    // }
    // bool isAllZero = (buff.count(0) == buff.size());
    // if(!isAllZero)
    // std::string data = QString::fromUtf8(buff).toStdString();
    // qDebug() << data.size();
    emit bufferReady(buff);
}



AudioCapture::AudioCapture(QObject *parent)
    : QObject{parent}
{
    // device = new QMediaDevices();
    // audio_src = new QAudioSource();
}

void AudioCapture::startRecord(){
    device = new QMediaDevices{this};
    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);
    // shared_ptr<rtc::DataChannel> _dc;
    audio_src = new QAudioSource{device->defaultAudioInput(),
                                                 format,nullptr};

    audio_src->reset();
    IO = audio_src->start();
    if(!IO->open(QIODevice::ReadOnly))
    {
        qDebug() << "Couldn't open AudioSource!";
    }
    else
    {
        connect(IO,&QIODevice::readyRead,[&]()
                {
                    audioSourceData(IO,audio_src);
                });
    }
}


QByteArray AudioCapture::readAny(){
    qint64 len = audio_src->bytesAvailable();

    const int BufferSize = 4096;
    // if (len > BufferSize)
    len = BufferSize;

    QByteArray buffer(len, 0);
    qint64 l = IO->read(buffer.data(), len);
    QByteArray buff = QByteArray::fromRawData(buffer,l);
    return buff;
}
