#ifndef DIALOGCE303_H
#define DIALOGCE303_H
#include <QDialog>
#include <QtGui>

namespace Ui {
    class DialogCE303;
}

struct data_struct;

class DialogCE303 : public QDialog{
    Q_OBJECT

public:
    explicit DialogCE303(data_struct *d, QWidget *parent = 0);
    ~DialogCE303();

    void updateDataCE303();

private:
    Ui::DialogCE303 *dui;
    data_struct * data;

public slots:
    void MyEventHandler();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
};

#endif // DIALOGCE303_H
