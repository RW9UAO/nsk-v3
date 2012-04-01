#ifndef DIALOGNASOS1_H
#define DIALOGNASOS1_H
#include <QDialog>
#include <QtGui>

namespace Ui {
    class DialogNasos1;
}

struct data_struct;

class DialogNasos1 : public QDialog{
    Q_OBJECT

public:
    explicit DialogNasos1(data_struct *d, QWidget *parent = 0);
    ~DialogNasos1();

    void updateDataNasos1();

private:
    Ui::DialogNasos1 *dui;
    data_struct * data;

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};
#endif // DIALOGNASOS1_H
