#ifndef DIALOGNASOS3_H
#define DIALOGNASOS3_H
#include <QDialog>
#include <QtGui>

namespace Ui {
    class DialogNasos3;
}

struct data_struct;

class DialogNasos3 : public QDialog{
    Q_OBJECT

public:
    explicit DialogNasos3(data_struct *d, QWidget *parent = 0);
    ~DialogNasos3();

    void updateDataNasos3();

private:
    Ui::DialogNasos3 *dui;
    data_struct * data;

public slots:
    void MyEventHandler();
private slots:
};
#endif // DIALOGNASOS1_H
