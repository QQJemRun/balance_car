#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
signals:
    void setrecvport(int recv_port);
    void setwhoreceive(QString receiver_ip,int receiver_port);
    void sendingdata(QString);

private slots:
    void received_data(QString message);

    void send_data(QString message);

    void send_direction(QPointF direction);

    void on_connect_Button_clicked();

    void on_send_Button_clicked();

    void on_forward_Button_clicked();

    void on_left_Button_clicked();

    void on_right_Button_clicked();

    void on_back_Button_clicked();

private:
    Ui::Widget *ui;
    int send_message_num;
    int recv_message_num;
};
#endif // WIDGET_H
