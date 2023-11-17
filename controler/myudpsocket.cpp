#include "myudpsocket.h"

receiveudpsocket::receiveudpsocket(QObject *parent)
{
    connect(this,&receiveudpsocket::readyRead,this,&receiveudpsocket::receiving);
}

void receiveudpsocket::receiving()
{
    char buf[1024]={0};
    QHostAddress sender_ip;
    quint16 sender_port;
    while(this->hasPendingDatagrams())
    {
        this->readDatagram(buf,1024,&sender_ip,&sender_port);
        emit this->received("["+sender_ip.toString()+":"+QString::number(sender_port)+"]"+QString::fromLocal8Bit(buf));
        memset(buf,0,1024);
    }
}

void receiveudpsocket::setrecvport(int recv_port)
{
    if(!bind_if)
    {
        this->bind(QHostAddress::Any,recv_port,ReuseAddressHint);
        bind_if = true;
    }
}


sendudpsocket::sendudpsocket(QObject *parent)
{
    this->bind(QHostAddress::Any,5555,ReuseAddressHint);
}

void sendudpsocket::whoreceive(QString ip, int port)
{
    this->receiver_ip = QHostAddress(ip);
    this->receiver_port = port;
}

void sendudpsocket::sending(QString message)
{
    this->writeDatagram(message.toUtf8(),this->receiver_ip,this->receiver_port);
    emit this->send(message.toUtf8());
}
