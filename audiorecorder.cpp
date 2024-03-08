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

AudioRecorder::AudioRecorder(QObject *parent)
    : QObject{parent}
{

}




void AudioRecorder::record(){
    QMediaCaptureSession m_captureSession;
    QAudioDevice defaultMicrophone;
    for (const QAudioDevice& device : QMediaDevices::audioInputs()) {
        if (device.isDefault() && device.mode() == QAudioDevice::Input) {
            defaultMicrophone = device;
            break;
        }
    }
    QAudioInput * ai = new QAudioInput();
    ai->setDevice(defaultMicrophone);
    m_captureSession.setAudioInput(ai);
    QMediaRecorder* recorder = new QMediaRecorder();
    // m_audioRecorder->setQuality(QMediaRecorder::Quality(ui->qualitySlider->value()));
    m_captureSession.setRecorder(recorder);
    recorder->setMediaFormat(QMediaFormat::Wave);
    recorder->setOutputLocation(QUrl("C:/Users/farbo/Desktop/university-s6/CN/ca1/ca1/test.wav"));
    recorder->setAudioSampleRate(48000);
    recorder->setAudioChannelCount(1);
    recorder->record();
    QThread::msleep(32500);
    recorder->stop();



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
