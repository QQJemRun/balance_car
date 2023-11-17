#ifndef MYUDPSOCKET_H
#define MYUDPSOCKET_H

#include <QUdpSocket>
#include <QObject>

class receiveudpsocket:public QUdpSocket
{
    Q_OBJECT
public:
    receiveudpsocket(QObject *parent = nullptr);
    void receiving();
    void setrecvport(int recv_port);
signals:
    void received(QString message);
private:
    bool bind_if = false;
};

class sendudpsocket:public QUdpSocket
{
    Q_OBJECT

public:
    sendudpsocket(QObject *parent = nullptr);
    void whoreceive(QString ip,int port);
    void sending(QString message);
signals:
    void send(QString message);
private:
    QHostAddress receiver_ip;
    int receiver_port;

};

#endif
