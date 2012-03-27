#ifndef DIALOGPOWER_H
#define DIALOGPOWER_H

#include <QDialog>
#include <QtGui>

namespace Ui {
    class DialogPower;
}

struct data_struct;

class DialogPower : public QDialog{
    Q_OBJECT

public:
    explicit DialogPower(data_struct *d, QWidget *parent = 0);
    ~DialogPower();

    void updateDataPower();

private:
    Ui::DialogPower *dui;
    data_struct * data;

public slots:
    void MyEventHandler();
private slots:
};

#endif // DIALOGPOWER_H
