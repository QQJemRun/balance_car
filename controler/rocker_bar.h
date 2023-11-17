#ifndef ROCKER_BAR_H
#define ROCKER_BAR_H

#include <QWidget>
#include <QObject>

class rocker_bar:public QWidget
{
    Q_OBJECT
public:
    rocker_bar(QWidget *parent);
signals:
    void new_direction(QPointF);

protected:

    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QPointF rocker_center;
    int r_b;
    int r_f;
    bool press_if;

};

#endif // ROCKER_BAR_H
