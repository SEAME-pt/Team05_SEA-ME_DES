#include "batterydial.h"
#include <QtMath>
#include <QStyleOptionSlider>
#include <QStylePainter>
#include <QTimer>
#include <QSettings>
#include <QLinearGradient>
#include <iostream>

BatteryDial::BatteryDial(QWidget *parent)
    : QWidget(parent), current(0), max(100)
{
    setStyleSheet("background-color: rgb(2, 1, 30);");
    setFixedSize(400, 470);
}

BatteryDial::~BatteryDial() {
    std::cout << "Remove custom dial" << std::endl;
}

//fading
void BatteryDial::paintEvent(QPaintEvent *event) {
    QPainter painter(this); 
    painter.setRenderHint(QPainter::Antialiasing, true);
    int radius = qMin(width(), height()) / 2 - 10;  
    int segments = 300; 
    float segment_angle = 270.0f / segments; 
    for (int i = 0; i < segments; ++i) {
        float t = static_cast<float>(i) / (segments);
        int alpha = static_cast<int>(120 * (1 - std::abs(2 * t - 1))); 
        QColor color(0, 52, 50, alpha); 
        QPen pen(color, 20);
        painter.setPen(pen);
        float overlap = 1.1f; 
        painter.drawArc(10, 10, radius * 2, radius * 2, (180 - i * segment_angle) * 16, -segment_angle * 16 * overlap);
    }
    float angle_progress = (static_cast<float>(current) * 270.0f) / max;
    segment_angle = angle_progress / segments;
    QColor start_color(0, 65, 74); 
    QColor end_color(0, 255, 200);
    if (current < 40) {
        start_color = QColor(0, 65, 74); 
        end_color = QColor(0, 140, 150, 255); 
    }
    for (int i = 0; i < segments; ++i) {
        float t = static_cast<float>(i) / segments; //factor (0 to 1)
        QColor color = QColor::fromRgbF(
            (1 - t) * start_color.redF() + t * end_color.redF(),  
            (1 - t) * start_color.greenF() + t * end_color.greenF(), 
            (1 - t) * start_color.blueF() + t * end_color.blueF()  
        );
        int alpha = static_cast<int>(255 * (1 - std::abs(2 * t - 1)));
        color.setAlpha(alpha);
        QPen pen(color, 20);
        painter.setPen(pen);
        painter.drawArc(10, 10, radius * 2, radius * 2, (270 + i * segment_angle) * 16, segment_angle * 16);
    }
    paint_text(painter);
}

void BatteryDial::paint_text(QPainter &painter) {
    painter.setPen(QPen(QColor(0, 250, 195)));
    painter.setFont(QFont("Digital-7", 80, QFont::Bold));
    QRect textRect = rect();
    textRect.translate(0, -20); 
    painter.drawText(textRect, Qt::AlignCenter, QString::number(current));

    painter.setPen(QPen(QColor(0, 250, 195, 150)));
    QFont font("Digital-7", 30);
    painter.setFont(font);
    QRect percentage = rect();
    percentage.setWidth(percentage.width() - 80);
    painter.drawText(percentage, Qt::AlignCenter | Qt::AlignRight, "%");

    painter.setPen(QPen(QColor(0, 255, 160, 150)));
    painter.setFont(QFont("Calculator", 25));
    painter.drawText(rect(), Qt::AlignBottom | Qt::AlignCenter, "Power");
}

void BatteryDial::set_current(int n) {
    current = n;
    update();
}
