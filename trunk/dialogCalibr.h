#ifndef DIALOGCALIBR_H
#define DIALOGCALIBR_H
#include <QDialog>
#include <QtGui>

namespace Ui {
    class DialogCalibr;
}

struct data_struct;

class DialogCalibr : public QDialog{
    Q_OBJECT

public:
    explicit DialogCalibr(data_struct *d, QWidget *parent = 0);
    ~DialogCalibr();

    void updateDataCalibr();

private:
    Ui::DialogCalibr *dui;
    data_struct * data;

public slots:
    //void MyEventHandler();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
};

#endif // DIALOGCALIBR_H
