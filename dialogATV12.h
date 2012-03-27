#ifndef DIALOGATV12_H
#define DIALOGATV12_H
#include <QDialog>
#include <QtGui>

namespace Ui {
    class DialogATV12;
}

struct data_struct;

class DialogATV12 : public QDialog{
    Q_OBJECT

public:
    explicit DialogATV12(data_struct *d, QWidget *parent = 0);
    ~DialogATV12();

    void updateDataATV12();

private:
    Ui::DialogATV12 *dui;
    data_struct * data;

public slots:
    void MyEventHandler();
private slots:
    void on_start1_clicked();
    void on_start2_clicked();
    void on_start3_clicked();
    void on_start4_clicked();
    void on_stop1_clicked();
    void on_stop2_clicked();
    void on_stop3_clicked();
    void on_stop4_clicked();
};

#endif // DIALOGATV12_H
