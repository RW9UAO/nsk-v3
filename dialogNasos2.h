#ifndef DIALOGNASOS2_H
#define DIALOGNASOS2_H
#include <QDialog>
#include <QtGui>

namespace Ui {
    class DialogNasos2;
}

struct data_struct;

class DialogNasos2 : public QDialog{
    Q_OBJECT

public:
    explicit DialogNasos2(data_struct *d, QWidget *parent = 0);
    ~DialogNasos2();

    void updateDataNasos2();

private:
    Ui::DialogNasos2 *dui;
    data_struct * data;

public slots:
    void MyEventHandler();
private slots:
};
#endif // DIALOGNASOS1_H
