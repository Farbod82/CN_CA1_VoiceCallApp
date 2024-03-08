#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QObject>
#include<QMediaRecorder>
#include <QAudioInput>
#include <QMediaCaptureSession>

class AudioRecorder : public QObject
{
    Q_OBJECT
private:
    QAudioInput* inputDevice;
    QMediaCaptureSession* capturer;
public:
    explicit AudioRecorder(QObject *parent = nullptr);
public slots:
    void record();
    // void handleAudioData();
    // void stop();
signals:
};

#endif // AUDIORECORDER_H
