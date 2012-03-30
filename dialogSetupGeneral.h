#ifndef DIALOGSETUPGENERAL_H
#define DIALOGSETUPGENERAL_H
#include <QDialog>
#include <QtGui>

namespace Ui {
    class DialogSetupGeneral;
}

struct data_struct;

class DialogSetupGeneral : public QDialog{
    Q_OBJECT

public:
    explicit DialogSetupGeneral(data_struct *d, QWidget *parent = 0);
    ~DialogSetupGeneral();

//    void updateSetupGeneral();

private:
    Ui::DialogSetupGeneral *dui;
    data_struct * data;

public slots:
    void MyEventHandler();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_12_clicked();
    void on_pushButton_16_clicked();
};

#endif // DIALOGSETUPGENERAL_H
