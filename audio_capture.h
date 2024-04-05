#ifndef AUDIO_CAPTURE_H
#define AUDIO_CAPTURE_H

#include <QAudioSource>
#include <QMediaDevices>
#include <QObject>

class AudioCapture : public QObject
{
    Q_OBJECT
public:
    explicit AudioCapture(QObject *parent = nullptr);

    void audioSourceData(QIODevice * device, QAudioSource* src);

    void startRecord();
    QByteArray readAny();
signals:
    void bufferReady(const QByteArray& buffer);
private:
    QMediaDevices* device;
    QAudioSource* audio_src;
    QIODevice* IO;

};

#endif // AUDIO_CAPTURE_H
