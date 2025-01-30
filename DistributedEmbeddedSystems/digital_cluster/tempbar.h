#ifndef TEMPBAR_H
#define TEMPBAR_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QLabel>

class TempBar : public QWidget
{
    Q_OBJECT
public:
    TempBar(QWidget *parent = nullptr);
    ~TempBar();
    void set_temperature(int i);
    void set_text(int temp);

private:
    int nb_sections;
    QVector<QWidget*> sections;
    QHBoxLayout *layout;
    QVBoxLayout* main_layout;
    QHBoxLayout* layout_temp = nullptr;
};

#endif // TEMPBAR_H
