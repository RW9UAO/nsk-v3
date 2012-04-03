#ifndef DIALOGALARM_H
#define DIALOGALARM_H

#include <QDialog>
#include <QtGui>

namespace Ui {
    class DialogAlarm;
}

struct data_struct;

class DialogAlarm : public QDialog{
    Q_OBJECT

public:
    explicit DialogAlarm(data_struct *d, QWidget *parent = 0);
    ~DialogAlarm();

private:
    Ui::DialogAlarm *dui;
    data_struct * data;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};
#endif // DIALOGALARM_H
