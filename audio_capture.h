#ifndef AUDIO_CAPTURE_H
#define AUDIO_CAPTURE_H

#include <QAudioSource>
#include <QObject>

#include <rtc/datachannel.hpp>

using std::shared_ptr;

class AudioCapture : public QObject
{
    Q_OBJECT
public:
    explicit AudioCapture(std::shared_ptr<rtc::DataChannel> dc, QObject *parent = nullptr);

    void audioSourceData(QIODevice * device, QAudioSource* src);

signals:
    void bufferReady(const QByteArray& buffer);
private:
    shared_ptr<rtc::DataChannel> _dc;
};

#endif // AUDIO_CAPTURE_H
