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
//        void changeDataATV12();
//        void changeDataCE303();
        void changeDataMain();
        void changeDataNasos1();
        void changeDataNasos2();
        void changeDataNasos3();
        void changeDataNasos4();

private:
        Ui::MainWindow *ui;
        MainWindow *wnd;
};
#endif // THREAD_MATH_H
