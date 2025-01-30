#ifndef AUTONOMYBAR_H
#define AUTONOMYBAR_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QLabel>
#include <QFontDatabase>
#include <QDir>
#include <QCoreApplication>

class AutonomyBar : public QWidget
{
    Q_OBJECT
public:
    AutonomyBar(QWidget *parent = nullptr);
    ~AutonomyBar();
    void set_autonomy(int i);

private:
    int nb_sections;
    QVector<QWidget*> sections;
    QHBoxLayout *layout;
    QVBoxLayout *main_layout;
};

#endif // AutonomyBar_H
