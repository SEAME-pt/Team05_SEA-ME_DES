#ifndef SPEED_H
#define SPEED_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QTimer>
#include <QFontDatabase>

class Speed : public QWidget
{
    Q_OBJECT
public:
    Speed(QWidget *parent = nullptr);
    ~Speed();
    void set_current(float n);
    void paint_text(QPainter &painter);
    void animation();
    bool get_is_animating();
    float get_current();
    int get_max();
    float get_current_angle();
    float get_target_angle();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int current;
    const int max;
    float current_angle;
    float target_angle;
    bool is_animating;
    int font_id;
    int font_id2;
    QFontDatabase font;
    QFontDatabase font2;
};

#endif // Speed_H
