#ifndef THREAD_MATH_H
#define THREAD_MATH_H

#include <QThread>
#include <QMessageBox>
#include "mainwindow.h"

class Thread_math : public QThread{
     Q_OBJECT
public:
        explicit Thread_math(MainWindow *wnd);

        void run();
        void quit();
signals:
        void changeDataMain();
        void changeDataNasos1();
        void changeDataCalibr();

private:
        Ui::MainWindow *ui;
        MainWindow *wnd;
        int getkolvo();
        int getminTTW();
        int constrain(int input_freq);
        double PID();
        void start_one_more_pump(int i);
        void save_pump_ttw();
};
#endif // THREAD_MATH_H
