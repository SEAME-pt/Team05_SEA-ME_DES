#include "customdial.h"
#include <QtMath>
#include <QStyleOptionSlider>
#include <QStylePainter>
#include <QTimer>
#include <QCoreApplication>
#include <QDir>
#include <QSettings>
#include <iostream>

CustomDial::CustomDial(QWidget *parent)
    : QWidget(parent), current(8), max(11)
{
    setStyleSheet("background-color: rgb(2, 1, 30);");
    setFixedSize(400, 470);
    QString path = QCoreApplication::applicationDirPath();
    QString digital_path = QDir(path).filePath("../digital-7.ttf"); //change this dir, take out the ../ when sending to jetson
    digital_path = QDir::cleanPath(digital_path);
    QString calculator_path = QDir(path).filePath("../Calculator.ttf");
    calculator_path = QDir::cleanPath(calculator_path);

    font_id = font.addApplicationFont(digital_path);
    font_id2 = font2.addApplicationFont(calculator_path);
}

CustomDial::~CustomDial() {
    QFontDatabase::removeApplicationFont(font_id);
    QFontDatabase::removeApplicationFont(font_id2);
    std::cout << "Remove custom dial" << std::endl;
}

//fading
void CustomDial::paintEvent(QPaintEvent *event) {
    QPainter painter(this); 
    painter.setRenderHint(QPainter::Antialiasing);
    int radius = qMin(width(), height()) / 2 - 10; 
    int segments = 300; 
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

    float angle_progress = (static_cast<float>(current) * 270.0f) / max;
    angle_progress = std::min(angle_progress, 270.0f);
    QColor start_color(0, 70, 90);  
    QColor end_color(0, 255, 255);  
    segment_angle = angle_progress / segments;
    for (int i = 0; i < segments; ++i) {
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

void CustomDial::paint_text(QPainter &painter) {
    painter.setPen(QPen(Qt::cyan));
    painter.setFont(QFont("Digital-7", 80, QFont::Bold));
    QRect textRect = rect();
    textRect.translate(0, -20); 
    painter.drawText(textRect, Qt::AlignCenter, QString::number(current));
    painter.setPen(QPen(Qt::darkCyan));
    painter.setFont(QFont("Calculator", 25));
    painter.drawText(rect(), Qt::AlignBottom | Qt::AlignCenter, "km/h");
}

void CustomDial::set_current(float n) {
    current = n * 3.6;
    update();
}
