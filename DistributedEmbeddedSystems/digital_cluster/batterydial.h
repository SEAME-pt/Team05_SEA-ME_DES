#ifndef BATTERYDIAL_H
#define BATTERYDIAL_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QTimer>

class BatteryDial : public QWidget
{
    Q_OBJECT
public:
    BatteryDial(QWidget *parent = nullptr);
    ~BatteryDial();
    void set_current(int n);
    void paint_text(QPainter &painter);
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int current;
    const int max;
signals:
};

#endif // BATTERYDIAL_H
