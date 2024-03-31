#include "audiorecorder.h"
#include <QAudioInput>
#include <QMediaCaptureSession>
#include <QMediaRecorder>
#include <QAudioDevice>
#include <QMediaDevices>
#include <QAudioOutput>
#include <QUrl>
#include <QMediaFormat>
#include <QThread>
#include <QAudioDecoder>
#include <iostream>
#include <QDebug>

AudioRecorder::AudioRecorder(QObject *parent)
    : QObject{parent}
{

}




void AudioRecorder::record(){
    QMediaCaptureSession* session = new QMediaCaptureSession();
    QAudioInput* audioInput = new QAudioInput();
    QMediaRecorder* recorder = new QMediaRecorder();
    session->setAudioInput(audioInput);
    session->setRecorder(recorder);
    recorder->setMediaFormat(QMediaFormat::Wave);
    recorder->setOutputLocation(QUrl::fromLocalFile("G:/Documents/GitHub/test.wav"));
    recorder->setAudioSampleRate(48000);
    recorder->setAudioChannelCount(1);
    recorder->record();
    QThread::msleep(3250);
    recorder->stop();

    // READ
    QAudioFormat desiredFormat;
    desiredFormat.setChannelCount(2);
    desiredFormat.setSampleFormat(QAudioFormat::Int16);
    desiredFormat.setSampleRate(48000);
    decoder = new QAudioDecoder();
    decoder->setAudioFormat(desiredFormat);
    decoder->setSource(QUrl::fromLocalFile("C:/Users/farbo/Desktop/university-s6/CN/ca1/ca1/test.wav"));
    connect(decoder, SIGNAL(bufferReady()), this, SLOT(readBuffer()));
    decoder->start();
}


void AudioRecorder::readBuffer(){
    qDebug() << "Debug Message";
}

// void AudioRecorder::handleAudioData(){
//     while (true) {
//         QByteArray audioData = audioBuffer.readAll();
//         if (audioData.size() > 0) {
//             std::cout<< "help\n";
//             std::cout <<audioData.data()<<std::endl;
//         }
//     }
// }


// void AudioRecorder::stop(){
//     m_audioRecorder->stop();
// }
