#include "../include/speed.h"
#include <QtMath>
#include <QStyleOptionSlider>
#include <QStylePainter>
#include <QTimer>
#include <QCoreApplication>
#include <QDir>
#include <QSettings>
#include <QTimer>
#include <iostream>
#include <QDebug>

Speed::Speed(QWidget *parent)
    : QWidget(parent), max(11), current_angle(0), target_angle(0), current(0)
{
    setStyleSheet("background-color: rgb(2, 1, 30);");
    setFixedSize(400, 470);
    QString path = QCoreApplication::applicationDirPath();
    QString digital_path = QDir(path).filePath("../fonts_icon/digital-7.ttf"); //change this dir, take out the ../ when sending to jetson
    digital_path = QDir::cleanPath(digital_path);
    QString calculator_path = QDir(path).filePath("../fonts_icon/Calculator.ttf");
    calculator_path = QDir::cleanPath(calculator_path);
    font_id = font.addApplicationFont(digital_path);
    font_id2 = font2.addApplicationFont(calculator_path);
    is_animating = false;
}

Speed::~Speed() {
    QFontDatabase::removeApplicationFont(font_id);
    QFontDatabase::removeApplicationFont(font_id2);
    std::cout << "Remove Speed" << std::endl;
}

//fading
void Speed::paintEvent(QPaintEvent *event) {
    QPainter painter(this); 
    painter.setRenderHint(QPainter::Antialiasing);
    int radius = qMin(width(), height()) / 2 - 10; 
    int segments = 400; 
    float segment_angle = 270.0f / segments; 
    for (int i = 0; i < segments; ++i) {
        float t = static_cast<float>(i) / (segments);
        int alpha = static_cast<int>(120 * (1 - std::abs(2 * t - 1))); 
        QColor color(0, 40, 60, alpha); 
        QPen pen(color, 20);
        painter.setPen(pen);
        float overlap = 1.1f; 
        painter.drawArc(10, 10, radius * 2, radius * 2, (270 - i * segment_angle) * 16, -segment_angle * 16 * overlap);
    }

    QColor start_color(0, 70, 90);  
    QColor end_color(0, 255, 255);  
    int active_segments = static_cast<int>((current_angle / 270.0f) * segments);
    for (int i = 0; i < active_segments; ++i) {
        float t = static_cast<float>(i) / segments; 
        QColor color = QColor::fromRgbF(
            (1 - t) * start_color.redF() + t * end_color.redF(),  
            (1 - t) * start_color.greenF() + t * end_color.greenF(), 
            (1 - t) * start_color.blueF() + t * end_color.blueF()  
        );
        int alpha = static_cast<int>(255 * (1 - std::abs(2 * t - 1)));
        color.setAlpha(alpha);
        QPen pen(color, 20);
        painter.setPen(pen);
        painter.drawArc(10, 10, radius * 2, radius * 2, (270 - i * segment_angle) * 16, -segment_angle * 16);
    }
    paint_text(painter);
}

void Speed::paint_text(QPainter &painter) {
    painter.setPen(QPen(Qt::cyan));
    painter.setFont(QFont("Digital-7", width() / 4, QFont::Bold));
    QRect textRect = rect();
    textRect.translate(0, -20); 
    painter.drawText(textRect, Qt::AlignCenter, QString::number(current));
    painter.setPen(QPen(Qt::darkCyan));
    painter.setFont(QFont("Calculator", width() / 18));
    painter.drawText(rect(), Qt::AlignBottom | Qt::AlignCenter, "km/h");
}

void Speed::set_current(float n) {
    current = n * 3.6;
    float new_target = (current * 270.0f) / max;
    new_target = std::min(new_target, 270.0f);
    if (std::abs(new_target - target_angle) > 2.0f) {  
        target_angle = new_target;
        if (is_animating == false)
            animation();
    }
}

void Speed::animation() {
    is_animating = true;
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this, timer]() {
        if (current_angle < target_angle) {
            current_angle += 5.0f; 
            if (current_angle > target_angle) current_angle = target_angle;
        } else if (current_angle > target_angle) {
            current_angle -= 5.0f;
            if (current_angle < target_angle) current_angle = target_angle;
        } else {
            timer->stop();
            timer->deleteLater();
            is_animating = false;
        }
        update();
    });
    timer->start(1);
}

bool Speed::get_is_animating() {
    return is_animating;
}

float Speed::get_current() {
    return current;
}

float Speed::get_current_angle() {
    return current_angle;
}

float Speed::get_target_angle() {
    return target_angle;
}

int Speed::get_max() {
    return max;
}