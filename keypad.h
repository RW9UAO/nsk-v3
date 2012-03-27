#ifndef KEYPAD_H
#define KEYPAD_H
#include <QDialog>
#include <QtGui>

namespace Ui {
    class Dialogkeypad;
}

struct data_struct;

class Dialogkeypad : public QDialog{
    Q_OBJECT

public:
    explicit Dialogkeypad(data_struct *d, QWidget *parent = 0);
    ~Dialogkeypad();

private:
    Ui::Dialogkeypad *dui;
    data_struct * data;

public slots:
    void MyEventHandler();
private slots:
    void on_b1_clicked();
    void on_b2_clicked();
    void on_b4_clicked();
    void on_b3_clicked();
    void on_b5_clicked();
    void on_b6_clicked();
    void on_b7_clicked();
    void on_b8_clicked();
    void on_b9_clicked();
    void on_b0_clicked();
    void on_bdot_clicked();
    void on_bbs_clicked();
    void on_save_clicked();
    void on_save_2_clicked();
};

#endif // KEYPAD_H
