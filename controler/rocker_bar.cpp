#include "rocker_bar.h"
#include <QPen>
#include <QPainter>
#include <QBrush>
#include <QDebug>
#include <QMouseEvent>

rocker_bar::rocker_bar(QWidget *parent)
{
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setFixedSize(200,200);
    rocker_center = QPointF(this->width()/2,this->height()/2);
    r_f = 20;
    r_b = this->height()/2-10;
    if(this->width()<this->height())
        r_b = this->width()/2-10;
    press_if = false;
}

void rocker_bar::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    //p.drawPixmap(0,0,width(),height(),QPixmap("../images/bg.jpg"));
    //p.drawPixmap(rect(),QPixmap("../images/bg.jpg"));
    //定义画笔
    QPen pen;
    pen.setWidth(5);
    pen.setColor(Qt::black);
    pen.setStyle(Qt::SolidLine);
    p.setPen(pen);
    QBrush brush_1;
    brush_1.setColor(QColor(255, 163,72));
    brush_1.setStyle(Qt::SolidPattern);
    p.setBrush(brush_1);
    //画圆形
    p.drawEllipse(QPoint(this->width()/2,this->height()/2),r_b,r_b);
    QBrush brush_2;
    brush_2.setColor(QColor(248, 228, 92));
    brush_2.setStyle(Qt::SolidPattern);
    p.setBrush(brush_2);

    p.drawEllipse(this->rocker_center,r_f,r_f);
}

void rocker_bar::mousePressEvent(QMouseEvent *event)
{
    if(qPow(event->position().x()-this->width()/2,2)+qPow(event->position().y()-this->height()/2,2)< qPow(r_b-15,2))
    {
        rocker_center = event->position();
        press_if = true;
        this->repaint();
    }
}

void rocker_bar::mouseMoveEvent(QMouseEvent *event)
{
    QPointF d_p(0,0);
    double r_n = qSqrt(qPow(event->position().x()-this->width()/2,2)+qPow(event->position().y()-this->height()/2,2));
    if(press_if)
    {
        if(r_n > r_b-15)
        {
            float angle = qAtan2(event->position().x() - this->width()/2,this->height()/2-event->position().y());
            d_p.setX(int((r_b-15)*qSin(angle)));
            d_p.setY(int((r_b-15)*qCos(angle)));
            this->rocker_center = QPointF(int(this->width()/2+d_p.x()),int(this->height()/2-d_p.y()));
        }
        else
        {
            d_p.setX(event->position().x() - this->width()/2);
            if(event->position().y() > this->height()/2)
                d_p.setY(this->height()/2-event->position().y());
            else
                d_p.setY(this->height()/2-event->position().y());
            this->rocker_center = event->position();
        }
        emit new_direction(d_p);

        this->repaint();
    }
}

void rocker_bar::mouseReleaseEvent(QMouseEvent *event)
{
    rocker_center = QPointF(this->width()/2,this->height()/2);
    press_if = false;
    this->repaint();
}
