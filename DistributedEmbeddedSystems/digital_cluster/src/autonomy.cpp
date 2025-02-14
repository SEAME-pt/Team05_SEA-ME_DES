#include "../include/autonomy.h"

//constructor
Autonomy::Autonomy(QWidget *parent)
    : QWidget{parent}
{
    if (parent) {
        setMinimumSize(parent->width() * 0.3, parent->height() * 0.2); 
        setMaximumSize(parent->width() * 0.3, parent->height() * 0.2);
    }
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
    main_layout->addLayout(layout);
    label = new QLabel(this);
    set_autonomy(8);
    std::cout << "autonomy constructor\n";
}

//destructor
Autonomy::~Autonomy()
{
    delete layout;
}

//setter
void Autonomy::set_autonomy(int aut)
{
    int sections_color = static_cast<int>((aut / 10.0) * nb_sections);
    for (int i = nb_sections -1; i >= 0; i--) {
        if (i >= nb_sections - sections_color) {
            QColor section_color;
            if (aut > 6) {
                int red = std::max(0, 120 - ((nb_sections - 1 - i) * (255 / nb_sections))); 
                int green = std::min(255, 80 + ((nb_sections - 1 - i) * (20 / nb_sections))); 
                int blue = std::min(255, 50 + ((nb_sections - 1 - i) * (50 / nb_sections)));  
                section_color.setRgb(red, green, blue);
            } else {
                int red = std::max(0, 120 - ((nb_sections - 1 - i) * (255 / nb_sections)));         
                int green = std::min(255, 80 + ((nb_sections - 1 - i) * (20 / nb_sections))); 
                int blue = std::min(255, 50 + ((nb_sections - 1 - i) * (50 / nb_sections)));
                section_color.setRgb(red, green, blue);
            } 
            sections[i]->setStyleSheet(QString("background-color: %1").arg(section_color.name()));
        } else {
            QColor inactive_color(22, 32, 60); 
            sections[i]->setStyleSheet(QString("background-color: %1").arg(inactive_color.name()));
        }
    }
    label->setTextFormat(Qt::RichText); // Enable rich text
    label->setText("<span style='font-family: Digital-7; font-size: 24px;'>" + QString::number(aut) + 
        "</span><span style='font-family: Calculator; font-size: 24px;'> km</span>");
    label->setStyleSheet("color: rgb(0, 120, 140);");
    label->setAlignment(Qt::AlignTop | Qt::AlignRight);
    label->setContentsMargins(0, 0, 10, 0);
    main_layout->addWidget(label);
}

// getters
int Autonomy::get_nbsections()
{
    return nb_sections;
}

QVector<QWidget*> Autonomy::get_sections()
{
    return sections;
}

QHBoxLayout* Autonomy::get_layout()
{
    return layout;
}

QVBoxLayout* Autonomy::get_mainlayout()
{
    return main_layout;
}

QLabel* Autonomy::get_label()
{
    return label;
}