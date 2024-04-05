#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <QMediaDevices>


class AudioPlayer: public QObject
{
    Q_OBJECT
public:
    explicit AudioPlayer(QObject *parent = nullptr);
    void startPlaying();
    void playData(std::vector<std::byte> message);
private:
    QMediaDevices* devices;
    QIODevice* io;

signals:
    void bufferReady();
};

#endif // AUDIO_PLAYER_H
