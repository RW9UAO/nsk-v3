#ifndef DIALOGNASOSPARAMETER_H
#define DIALOGNASOSPARAMETER_H
#include <QDialog>
#include <QtGui>

namespace Ui {
    class DialogNasosParameter;
}

struct data_struct;

class DialogNasosParameter : public QDialog{
    Q_OBJECT

public:
    explicit DialogNasosParameter(data_struct *d, QWidget *parent = 0);
    ~DialogNasosParameter();


private slots:
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::DialogNasosParameter *dui;
    data_struct * data;
};

#endif // DIALOGNASOSPARAMETER_H
