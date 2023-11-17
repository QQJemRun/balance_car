#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QThread>
#include "myudpsocket.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    receiveudpsocket *recv_sock = new receiveudpsocket;
    sendudpsocket *send_sock = new sendudpsocket;
    QThread *recv_thread = new QThread;
    QThread *send_thread = new QThread;
    recv_sock->moveToThread(recv_thread);
    send_sock->moveToThread(send_thread);
    connect(this,&Widget::setwhoreceive,send_sock,&sendudpsocket::whoreceive);
    connect(this,&Widget::sendingdata,send_sock,&sendudpsocket::sending);
    connect(this,&Widget::setrecvport,recv_sock,&receiveudpsocket::setrecvport);
    connect(recv_sock,&receiveudpsocket::received,this,&Widget::received_data);
    connect(send_sock,&sendudpsocket::send,this,&Widget::send_data);
    connect(ui->rocker,&rocker_bar::new_direction,this,&Widget::send_direction);
    recv_thread->start();
    send_thread->start();
    send_message_num = 0;
    recv_message_num = 0;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::received_data(QString message)
{
    ui->recivied_Edit->append(QString::number(++recv_message_num)+">>"+message+"\n");
}

void Widget::send_data(QString message)
{
    ui->send_Edit->append(QString::number(++send_message_num)+">>"+message+"\n");
}

void Widget::send_direction(QPointF direction)
{
    emit this->sendingdata("new_direction("+QString::number(direction.x())+","+QString::number(direction.y())+")");
}

void Widget::on_connect_Button_clicked()
{
    if(ui->ip_input->text().isEmpty())
    {
        QMessageBox box(this);
        box.warning(this,"no_ip","input the opposite ip,please");
        return ;
    }
    if(ui->port_input->text().isEmpty())
    {
        QMessageBox box(this);
        box.warning(this,"no_port","input the opposite port,please");
        return ;
    }
    emit setwhoreceive(ui->ip_input->text(),ui->port_input->text().toInt());
    emit setrecvport(ui->self_recv_port_input->text().toInt());
    emit sendingdata("connect");
}

void Widget::on_send_Button_clicked()
{
    if(ui->send_message_edit->text().isEmpty())
        return;
    emit sendingdata(ui->send_message_edit->text());
    ui->send_message_edit->clear();
}


void Widget::on_forward_Button_clicked()
{
    emit this->sendingdata("forword");
}

void Widget::on_left_Button_clicked()
{
    emit this->sendingdata("turn left");
}

void Widget::on_right_Button_clicked()
{
    emit this->sendingdata("turn right");
}


void Widget::on_back_Button_clicked()
{
    emit this->sendingdata("back");
}

