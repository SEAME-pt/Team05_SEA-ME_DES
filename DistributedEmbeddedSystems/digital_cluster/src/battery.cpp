#include "../include/battery.h"
#include <iostream>
#include <QPixmap>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>

Battery::Battery(QWidget *parent)
    : QWidget(parent), current(20), max(100), test_painter(nullptr)
{
    setStyleSheet("background-color: rgb(2, 1, 30);");
    if (parent) {
        setMinimumSize(parent->width() * 0.5, parent->height() * 0.7); 
        setMaximumSize(parent->width() * 0.5, parent->height() * 0.7);
    }
}

Battery::~Battery()
{
    std::cout << "Remove battery" << std::endl;
}

void Battery::set_current(int n)
{
    current = n;
    update();
}

int Battery::get_current()
{
    return current;
}

void Battery::paintEvent(QPaintEvent *event)
{
    TestPainter* painter = test_painter;
    QPainter real_painter(this);
    if (!painter) {
        painter = new QPainterCaller(&real_painter);
    }
    if (test_painter) {
        painter->begin(this);
    }
    painter->setRenderHint(QPainter::Antialiasing, true);
    draw_arcs(painter);
    draw_pixmap(painter);
}


//fading arcs
void Battery::draw_arcs(TestPainter *painter)
{
    int radius = qMin(width(), height()) / 2.5;  
    int segments = 300; 
    int centerX = width() / 2;
    int centerY = height() / 2;
    float segment_angle = 270.0f / segments; 
    for (int i = 0; i < segments; ++i) {
        float t = static_cast<float>(i) / (segments);
        int alpha = static_cast<int>(120 * (1 - std::abs(2 * t - 1))); 
        QColor color(0, 52, 50, alpha); 
        QPen pen(color, width() / 20);
        painter->setPen(pen);
        float overlap = 1.1f; 
        painter->drawArc(centerX - radius, centerY - radius, radius * 2, radius * 2, (180 - i * segment_angle) * 16, -segment_angle * 16 * overlap);
    }
    float angle_progress = (static_cast<float>(current) * 270.0f) / max;
    segment_angle = angle_progress / segments;
    QColor start_color(0, 65, 74); 
    QColor end_color(0, 255, 200);
    if (current < 40) {
        segments = 200;
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
        QPen pen(color, width() / 20);
        painter->setPen(pen);
        painter->drawArc(centerX - radius, centerY - radius, radius * 2, radius * 2, (270 + i * segment_angle) * 16, segment_angle * 16);
    }
}


void Battery::draw_pixmap(TestPainter *painter)
{
    painter->setPen(QPen(QColor(0, 250, 195)));
    painter->setFont(QFont("Digital-7", width() / 5, QFont::Bold));
    QRect currentTextRect = painter->boundingRect(rect(), Qt::AlignCenter, QString::number(current));
    painter->drawText(currentTextRect, Qt::AlignCenter, QString::number(current));
    
    QString path = QCoreApplication::applicationDirPath();
    QString digital_path = QDir(path).filePath("../fonts_icon/charging-station.png"); //change this dir, take out the ../ when sending to jetson
    digital_path = QDir::cleanPath(digital_path);
    QPixmap pixmap(digital_path);  
    QImage img = pixmap.toImage();
    for (int x = 0; x < img.width(); ++x) {
        for (int y = 0; y < img.height(); ++y) {
            QColor color = img.pixelColor(x, y);
            if (color == QColor(0, 0, 0)) {   //changing colors of icon
                color.setRgb(0, 120, 100); 
                img.setPixelColor(x, y, color);
            }
        }
    }
    pixmap.convertFromImage(img);
    pixmap = pixmap.scaled(width() / 10, width() / 10, Qt::KeepAspectRatio);
    QRect rectBottom = this->rect();
    int xIcon = (width() - pixmap.width()) / 2;  
    QRect bottomRect = QRect(xIcon, currentTextRect.bottom() + 10, pixmap.width(), pixmap.height());
    painter->drawPixmap(bottomRect, pixmap);
    draw_text(painter, bottomRect);
}


void Battery::draw_text(TestPainter *painter, QRect bottomRect)
{
    QFont font("Calculator", width() / 16);
    painter->setFont(font);
    painter->setPen(QPen(QColor(0, 120, 100)));
    int yPos = bottomRect.bottom();  
    int xPos = bottomRect.right() + 5; 
    QRectF textRect(bottomRect.right() + 5, bottomRect.bottom() - 20, 30, 30);  // Adjust size as needed
    int flags = Qt::AlignLeft | Qt::AlignVCenter;  // Adjust alignment flags as needed
    QString text = "%";
    painter->drawText(textRect, flags, text);
}