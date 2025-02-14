#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QLabel>

class Temperature : public QWidget
{
    Q_OBJECT
public:
    Temperature(QWidget *parent = nullptr);
    ~Temperature();
    void set_temperature(int i);
    void set_text(int temp);
    int get_nbsections();
    QVector<QWidget*> get_sections();
    QHBoxLayout* get_layout();
    QVBoxLayout* get_mainlayout();
    QLabel *get_label();

private:
    int nb_sections;
    QVector<QWidget*> sections;
    QHBoxLayout *layout;
    QVBoxLayout* main_layout;
    QLabel *label = nullptr;
};

#endif // Temperature_H
