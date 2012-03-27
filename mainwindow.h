#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// при сборке для платы не забываем включить эту галочку (DEFINES+=ARM)
//#define ARM 1
//#define DEBUG 1

#include <QMainWindow>

#include <memory>

#include "dialogATV12.h"
#include "dialogI2C.h"
#include "dialogCE303.h"
#include "dialogPower.h"
#include "dialogAlarm.h"
#include "dialogNasos1.h"
#include "dialogNasos2.h"
#include "dialogNasos3.h"
#include "dialogNasos4.h"
#include "keypad.h"
#include "data_struct.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void updateDataMain();

    Ui::MainWindow *ui;

    bool isATV12;// частотник есть?
    unsigned char ATV12maxNum;//частотник последний номер, не больше 6-ти
    bool isSoftStart;//плавный пуск есть?
    bool isCE303;//счетчик есть? а если найду?
    bool is21;
    bool is35;
    bool islevel_meter;//наличие датчика уровня на I2C

    std::auto_ptr<DialogATV12> dlgATV12;
    std::auto_ptr<DialogCE303> dlgCE303;
    std::auto_ptr<DialogI2C> dlgI2C;
    std::auto_ptr<DialogPower> dlgPower;
    std::auto_ptr<DialogAlarm> dlgAlarm;
    std::auto_ptr<DialogNasos1> dlgNasos1;
    std::auto_ptr<DialogNasos2> dlgNasos2;
    std::auto_ptr<DialogNasos3> dlgNasos3;
    std::auto_ptr<DialogNasos4> dlgNasos4;
    std::auto_ptr<Dialogkeypad> dlgkeypad;

    data_struct data;
    bool done;

private slots:
    void on_ShowDialogATV12_clicked();
    void on_ShowDialogCE303_clicked();
    void on_ShowDialogI2C_clicked();
    void on_Power_clicked();
    void on_Alarm_clicked();
    void on_Nasos1_clicked();
    void on_Nasos2_clicked();
    void on_Nasos3_clicked();
    void on_Nasos4_clicked();

public slots:
    void onDataChangeATV12();
    void onDataChangeCE303();
    void onDataChangeI2C();
    void onDataChangeMain();
    void onDataChangePower();
    void onDataChangeNasos1();
    void onDataChangeNasos2();
    void onDataChangeNasos3();
    void onDataChangeNasos4();
    //void MyEventHandler();
};

#endif // MAINWINDOW_H
