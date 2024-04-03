#ifndef OFFERER_H
#define OFFERER_H

#include <QObject>

#include <iostream>


class offerer : public QObject
{
    Q_OBJECT
public:
    explicit offerer(QObject *parent = nullptr);

    void runOfferer(std::string name, std::string answerer_name);
signals:
private:
    std::string candidate;
    std::string description;
};


#endif // OFFERER_H
