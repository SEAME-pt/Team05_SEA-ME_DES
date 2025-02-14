#include "../include/temperature.h"

Temperature::Temperature(QWidget *parent)
    : QWidget{parent}
{
    setMinimumSize(parent->width() * 0.3, parent->height() * 0.2); 
    setMaximumSize(parent->width() * 0.3, parent->height() * 0.2);
    main_layout = new QVBoxLayout(this);
    main_layout->setSpacing(height() * 0.05);
    layout = new QHBoxLayout();
    layout->setSpacing(width() * 0.02); 
    nb_sections = 8;
    for (int i = 0; i < nb_sections; ++i) {
        QWidget *section = new QWidget(this);
        section->setMinimumSize(width() * 0.1, height() * 0.3); //rectangular section
        section->setMaximumSize(width() * 0.1, height() * 0.3);
        layout->addWidget(section);
        sections.append(section);
    }
    main_layout->addLayout(layout, 1);
    label = new QLabel(this);
    set_temperature(50);
    setLayout(main_layout); //setting layout for qwidget (no need to delete them)
}

Temperature::~Temperature()
{
}

void Temperature::set_text(int temp)
{
    label->setTextFormat(Qt::RichText);
    label->setText("<span style='font-family: Noto Sans; font-size: 24px; color: rgb(0, 120, 140);'>🌡️&nbsp;&nbsp;</span>"
        "<span style='font-family: Digital-7; font-size: 24px; color: rgb(0, 120, 140);'>" + 
        QString::number(temp) + "</span>"
        "<span style='font-family: Calculator; font-size: 24px; color: rgb(0, 120, 140);'> °C</span>");
    label->setContentsMargins(10, 0, 0, 0);
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    main_layout->addWidget(label);
}

void Temperature::set_temperature(int temp)
{
    int sections_color = static_cast<int>((temp / 80.0) * nb_sections);
    for (int i = 0; i < nb_sections; ++i) {
        if (i < sections_color) {
            QColor sectionColor;
            if (temp < 60) {
                int green = std::min(255, 60 + (i * (50 / nb_sections))); //from dim cyan to regular cyan
                int blue = std::min(255, 80 + (i * (50 / nb_sections)));  //dim blueincrease to bright blue
                sectionColor.setRgb(0, green, blue);
            } else {
                int red = (i * (200 / nb_sections));         //increase red component
                int green = std::min(255, 60 + (i * (20 / nb_sections)));  //decrease green
                int blue = std::max(0, 80 - (i * (50 / nb_sections)));
                sectionColor.setRgb(red, green, blue);
            }
            sections[i]->setStyleSheet(QString("background-color: %1").arg(sectionColor.name()));
        } else {
            QColor inactive(22, 32, 60); //dim cyan color
            sections[i]->setStyleSheet(QString("background-color: %1").arg(inactive.name()));
        }
    }
    set_text(temp);
}

int Temperature::get_nbsections()
{
    return nb_sections;
}

QVector<QWidget*> Temperature::get_sections()
{
    return sections;
}

QHBoxLayout* Temperature::get_layout()
{
    return layout;
}

QVBoxLayout* Temperature::get_mainlayout()
{
    return main_layout;
}

QLabel* Temperature::get_label()
{
    return label;
}