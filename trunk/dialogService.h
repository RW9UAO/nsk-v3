#ifndef DIALOGSERVICE_H
#define DIALOGSERVICE_H
#include <QDialog>
#include <QtGui>

namespace Ui {
    class DialogService;
}

struct data_struct;

class DialogService : public QDialog{
    Q_OBJECT

public:
    explicit DialogService(data_struct *d, QWidget *parent = 0);
    ~DialogService();

    void updateDataService();

private:
    Ui::DialogService *dui;
    data_struct * data;

public slots:
    void MyEventHandler();
private slots:
    void on_ButtonEnterService_clicked();
};

#endif // DIALOGSERVICE_H
