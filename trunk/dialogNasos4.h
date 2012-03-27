#ifndef DIALOGNASOS4_H
#define DIALOGNASOS4_H
#include <QDialog>
#include <QtGui>

namespace Ui {
    class DialogNasos4;
}

struct data_struct;

class DialogNasos4 : public QDialog{
    Q_OBJECT

public:
    explicit DialogNasos4(data_struct *d, QWidget *parent = 0);
    ~DialogNasos4();

    void updateDataNasos4();

private:
    Ui::DialogNasos4 *dui;
    data_struct * data;

public slots:
    void MyEventHandler();
private slots:
};
#endif // DIALOGNASOS1_H
