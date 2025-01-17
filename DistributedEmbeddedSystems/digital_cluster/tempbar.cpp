#include "tempbar.h"

TempBar::TempBar(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(300, 200); 
    main_layout = new QVBoxLayout(); 
    main_layout->setSpacing(18); 

    layout = new QHBoxLayout();
    layout->setSpacing(1); 
    nb_sections = 10;
    for (int i = 0; i < nb_sections; ++i) {
        QWidget *section = new QWidget(this);
        section->setFixedSize(20, 30); //rectangular section
        layout->addWidget(section);
        sections.append(section);
    }
    main_layout->addLayout(layout, 1);
    set_temperature(0);
    setLayout(main_layout);
}

TempBar::~TempBar() {
    // delete main_layout;
    // delete layout;
    // delete layout_temp;
}

void TempBar::set_text(int temp) {
    if (layout_temp) {
        QLayoutItem* item;
        while ((item = layout_temp->takeAt(0)) != nullptr) {
            delete item->widget(); 
            delete item;       
        }
        delete layout_temp;
    }
    layout_temp = new QHBoxLayout(); 
    QLabel *label = new QLabel(this);
    QFont font("Noto Sans");
    label->setFont(font);
    label->setText("🌡️");
    label->setStyleSheet("font-size: 25px; color: rgb(0, 120, 140);");
    layout_temp->addWidget(label);

    QLabel *value = new QLabel(this);
    QFont font1("Digital-7");
    value->setFont(font1);
    value->setText(QString::number(temp));
    value->setStyleSheet("font-size: 25px; color: rgb(0, 120, 140);");
    layout_temp->addWidget(value);

    QLabel *unit = new QLabel(this);
    QFont font2("Calculator");
    unit->setFont(font2);
    unit->setText("\u00B0 C");
    unit->setStyleSheet("font-size: 23px; color: rgb(0, 120, 140);");
    layout_temp->addWidget(unit);
    layout_temp->setContentsMargins(10, 0, 0, 0);
    layout_temp->setAlignment(Qt::AlignTop |Qt::AlignLeft);
    main_layout->addLayout(layout_temp);
}

void TempBar::set_temperature(int temp) {
    int sections_color = static_cast<int>((temp / 100.0) * nb_sections);
    for (int i = 0; i < nb_sections; ++i) {
        if (i < sections_color) {
            QColor sectionColor;
            if (temp < 70) {
                int blue = 170 + (i * (150 / nb_sections));  //dim blueincrease to bright blue
                int green = 80 + (i * (100 / nb_sections)); //from dim cyan to regular cyan
                sectionColor.setRgb(0, green, blue);
            } else {
                int red = (i * (240 / nb_sections));         //increase red component
                int green = 80 + (i * (10 / nb_sections));  //decrease green
                int blue = 130 - (i * (90 / nb_sections));
                sectionColor.setRgb(red, green, blue);
            }
            sections[i]->setStyleSheet(QString("background-color: %1").arg(sectionColor.name()));
        } else {
            QColor inactive(22, 32, 90); //dim cyan color
            sections[i]->setStyleSheet(QString("background-color: %1").arg(inactive.name()));
        }
    }
    set_text(temp);
    // QPixmap pixmap(":/resources/thermometer.png");
    // QLabel *iconLabel = new QLabel(this);
    // iconLabel->setPixmap(pixmap);
}
