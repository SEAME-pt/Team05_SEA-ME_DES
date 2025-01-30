#include "autonomybar.h"

AutonomyBar::AutonomyBar(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(300, 200); 
    main_layout = new QVBoxLayout(this);
    main_layout->setSpacing(10);
    layout = new QHBoxLayout();
    layout->setSpacing(1); 
    nb_sections = 10;
    for (int i = 0; i < nb_sections; ++i) {
        QWidget *section = new QWidget(this);
        section->setFixedSize(20, 30); //rectangular section
        layout->addWidget(section);
        sections.append(section);
    }
    main_layout->addLayout(layout);
    set_autonomy(80);
}

AutonomyBar::~AutonomyBar() {
    delete layout;
}

void AutonomyBar::set_autonomy(int aut) {
    int sections_color = static_cast<int>((aut / 120.0) * nb_sections);
    for (int i = nb_sections -1; i >= 0; i--) {
        if (i >= nb_sections - sections_color) {
            QColor section_color;
            if (aut > 60) {
                int red_value = 190 - ((nb_sections - 1 - i) * (255 / nb_sections)); 
                int blue_value = 50 + ((nb_sections - 1 - i) * (160 / nb_sections));  
                int green_value = 100 + ((nb_sections - 1 - i) * (200 / nb_sections)); 
                section_color.setRgb(red_value, green_value, blue_value);
            } else {
                int red_value = 180 - ((nb_sections - 1 - i) * (255 / nb_sections));         
                int green_value = 100 + ((nb_sections - 1 - i) * (230 / nb_sections)); 
                int blue_value = 60 + ((nb_sections - 1 - i) * (240 / nb_sections));
                section_color.setRgb(red_value, green_value, blue_value);
            } 
            sections[i]->setStyleSheet(QString("background-color: %1").arg(section_color.name()));
        } else {
            QColor inactive_color(22, 32, 90); 
            sections[i]->setStyleSheet(QString("background-color: %1").arg(inactive_color.name()));
        }
    }
    QLabel *label = new QLabel(this);
    label->setFont(QFont("Calculator", 27));
    label->setText(QString("km"));
    label->setStyleSheet("color: rgb(0, 120, 140);");
    label->setAlignment(Qt::AlignTop | Qt::AlignRight);
    label->setContentsMargins(0, 0, 10, 0);
    main_layout->addWidget(label);
}
