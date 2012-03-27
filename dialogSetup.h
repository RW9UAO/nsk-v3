#ifndef DIALOGSETUP_H
#define DIALOGSETUP_H
#include <QDialog>
#include <QtGui>

namespace Ui {
    class DialogSetup;
}

struct data_struct;

class DialogSetup : public QDialog{
    Q_OBJECT

public:
    explicit DialogSetup(data_struct *d, QWidget *parent = 0);
    ~DialogSetup();

    void updateDataSetup();

private:
    Ui::DialogSetup *dui;
    data_struct * data;

public slots:
    void MyEventHandler();
private slots:
    void on_pushButton_clicked();
    void on_P1_clicked();
    void on_P2_clicked();
    void on_P3_clicked();
    void on_P4_clicked();
};

#endif // DIALOGSETUP_H
