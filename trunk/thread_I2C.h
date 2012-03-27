#ifndef THREAD_I2C_H
#define THREAD_I2C_H

#include <QThread>
#include <QMessageBox>
#include "mainwindow.h"

class Thread_I2C : public QThread{
     Q_OBJECT
public:
        explicit Thread_I2C(MainWindow *wnd);

        void run();
        void quit();

signals:
          void changeDataI2C();

private:
        Ui::MainWindow *ui;
        MainWindow *wnd;
};
#endif // THREAD_I2C_H
