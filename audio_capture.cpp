#include "audio_capture.h"

#include <QAudioSource>
#include <QMediaDevices>
#include <QaudioFormat>
#include <QDebug>







void AudioCapture::audioSourceData(QIODevice * device, QAudioSource* src){
    qint64 len = src->bytesAvailable();

    const int BufferSize = 1024;
    // if (len > BufferSize)
    len = BufferSize;

    QByteArray buffer(len, 0);
    qint64 l = device->read(buffer.data(), len);
    QByteArray buff = QByteArray::fromRawData(buffer,l);
    emit bufferReady(buff);
}



AudioCapture::AudioCapture(QObject *parent)
    : QObject{parent}
{
    QMediaDevices* devices = new QMediaDevices{this};
    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    QAudioSource* audioSource = new QAudioSource{devices->defaultAudioInput(),
                                     format,nullptr};
    audioSource->reset();
    auto io = audioSource->start();
    if(!io->open(QIODevice::ReadOnly))
    {
        qDebug() << "Couldn't open AudioSource!";
    }
    else
    {
        connect(io,&QIODevice::readyRead,[&,audioSource,io]()
                {
                    audioSourceData(io,audioSource);
                });
    }
}
