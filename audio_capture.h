#ifndef AUDIO_CAPTURE_H
#define AUDIO_CAPTURE_H

#include <QAudioSource>
#include <QObject>

class AudioCapture : public QObject
{
    Q_OBJECT
public:
    explicit AudioCapture(QObject *parent = nullptr);

    void audioSourceData(QIODevice * device, QAudioSource* src);

    void startRecord();
signals:
    void bufferReady(const QByteArray& buffer);
};

#endif // AUDIO_CAPTURE_H
