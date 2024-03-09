#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QObject>
#include<QMediaRecorder>
#include <QAudioInput>
#include <QMediaCaptureSession>
#include <QAudioDecoder>

class AudioRecorder : public QObject
{
    Q_OBJECT
private:
    QAudioInput* inputDevice;
    QAudioDecoder *decoder;
    QMediaCaptureSession* capturer;
public:
    explicit AudioRecorder(QObject *parent = nullptr);
public slots:
    void record();
    void readBuffer();
    // void handleAudioData();
    // void stop();
signals:
};

#endif // AUDIORECORDER_H
