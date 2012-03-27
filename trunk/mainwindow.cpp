//#include "thread_485.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    //проинициализируем переменные
    set_defaults();
    this->done = true;
    read_config();
#if defined(ARM)
    this->setWindowFlags(Qt::FramelessWindowHint);
#endif
    this->dlgATV12.reset(new DialogATV12( &this->data, this));
    //this->dlgCE303.reset(new DialogCE303( &this->data, this));
    this->dlgI2C.reset(new DialogI2C( &this->data, this));
    this->dlgPower.reset(new DialogPower( &this->data, this));
    this->dlgAlarm.reset(new DialogAlarm( &this->data, this));
    this->dlgNasos1.reset(new DialogNasos1( &this->data, this));
    this->dlgNasos2.reset(new DialogNasos2( &this->data, this));
    this->dlgNasos3.reset(new DialogNasos3( &this->data, this));
    this->dlgNasos4.reset(new DialogNasos4( &this->data, this));
    //this->dlgkeypad.reset(new Dialogkeypad( &this->data, this));
    //this->dlgService.reset(new DialogService( &this->data, this));

    this->done = true;
/*
    //setStyleSheet("QMainWindow {background-image: url(:/pict/setgreen.gif);}");
    setStyleSheet(  "QMainWindow { background-image: url(:/pict/pict/pult_fon.gif);}"
                    "QPushButton.Power );*/

    setStyleSheet("QMainWindow {background-image: url(:/pict/pict/pult_fon.gif); }");
    ui->frame->setStyleSheet("QFrame {background-image: url(:/pict/pict/popl_0_uroven_avariya.gif); }");
    ui->labelLevel->setText("");
    ui->labelLevel->setStyleSheet("QLabel{font-family:Monospace;font-size:55pt;color:rgb(200,255,240);background-image: url(none);}");    
}

MainWindow::~MainWindow(){
    delete ui;
}
//=======================================================================================================================================
// обновим содержимое MainWindow
void MainWindow::onDataChangeMain(){
    int h,m,s;
    if(this->data.volta < 10 || this->data.voltb < 10 || this->data.voltc < 10){
        //авария по фазе или напряжению
        ui->Power->setStyleSheet("QPushButton{background-image:url(:/pict/pict/setred.gif);background-repeat:none;background-color:black;background-position:center;font-size:1px}");//
     }else{
        ui->Power->setStyleSheet("QPushButton{background-image:url(:/pict/pict/setgreen.gif);background-repeat:none;background-color:black;background-position:center;font-size:1px}");//
    }

    if(this->data.volta == -1 || this->data.angleAB == -1 || this->data.et0pe == -1){
        //авария счетчика
        ui->ShowDialogCE303->setStyleSheet("QPushButton{background-image:url(:/pict/pict/schetchik_red.gif);background-repeat:none;background-color:black;background-position:center;font-size:1px}");//
     }else{
        ui->ShowDialogCE303->setStyleSheet("QPushButton{background-image:url(:/pict/pict/schetchik_green.gif);background-repeat:none;background-color:black;background-position:center;font-size:1px}");//
    }

    //if(){
        //есть одна или более ошибок
        //ui->ShowDialogCE303->setStyleSheet("QPushButton{background-image:url(:/pict/pict/log_avarij_red.gif);background-repeat:none;background-color:black;background-position:center;font-size:1px}");//
     //}else{
        ui->Alarm->setStyleSheet("QPushButton{background-image:url(:/pict/pict/log_avarij_green.gif);background-repeat:none;background-color:black;background-position:center;font-size:1px}");//
    //}
     //павильон
     ui->Pavilion->setStyleSheet("QPushButton{background-image:url(:/pict/pict/housegreen.gif);background-repeat:none;background-color:black;background-position:center;font-size:1px}");//

     //насосы
     QPushButton *p[4] = {ui->Nasos1, ui->Nasos2, ui->Nasos3, ui->Nasos4};
     for(int i=1; i<5;i++){
         switch(this->data.nasos[i-1]){
         case 0://нет насоса
             p[i-1]->setStyleSheet("QPushButton{background-image:none;background-repeat:none;background-color:black;background-position:center;font-size:1px}");
             break;
         case 1:// не включен, готов
             p[i-1]->setStyleSheet("QPushButton{background-image:url(:/pict/pict/nasosgray.gif);background-repeat:none;background-color:black;background-position:center;font-size:18px;color:green;}");
             break;
         case 2://включен, работает
             p[i-1]->setStyleSheet("QPushButton{background-image:url(:/pict/pict/nasosgreen.gif);background-repeat:none;background-color:black;background-position:center;font-size:18px;color:rgb(200,255,0);}");
             m = this->data.nasos_TTW[i-1] / 60;
             s = this->data.nasos_TTW[i-1] - (m * 60);
             h = m / 24;
             m = m - (h * 24);
             if(this->isATV12){
                p[i-1]->setText(QString("%1:%2:%3\n%4Hz\n%5A").arg(h).arg(m).arg(s).arg(this->data.freq[i-1]).arg(this->data.current[i-1]) );
             }else{
                p[i-1]->setText(QString("%1:%2:%3").arg(h).arg(m).arg(s) );
             }
             break;
         case 3://авария по току
             p[i-1]->setStyleSheet("QPushButton{background-image:url(:/pict/pict/nasosred_a.gif);background-repeat:none;background-color:black;background-position:center;font-size:8px}");
             break;
         case 4://авария по температуре
             p[i-1]->setStyleSheet("QPushButton{background-image:url(:/pict/pict/nasosred_t.gif);background-repeat:none;background-color:black;background-position:center;font-size:8px}");
             break;
         case 5://авария по влажности
             p[i-1]->setStyleSheet("QPushButton{background-image:url(:/pict/pict/nasosred_vl.gif);background-repeat:none;background-color:black;background-position:center;font-size:8px}");
             break;
         }

     }
     switch(this->data.level_to_show){
     case 0: ui->frame->setStyleSheet("QFrame {background-image: url(:/pict/pict/popl_4_uroven_avariya.gif); }");break;
     case 1: ui->frame->setStyleSheet("QFrame {background-image: url(:/pict/pict/popl_0_uroven_avariya.gif); }");break;
     case 102:
     case 2: ui->frame->setStyleSheet("QFrame {background-image: url(:/pict/pict/popl_0_uroven.gif); }");break;
     case 103:
     case 3: ui->frame->setStyleSheet("QFrame {background-image: url(:/pict/pict/popl_1_uroven.gif); }");break;
     case 104:
     case 4: ui->frame->setStyleSheet("QFrame {background-image: url(:/pict/pict/popl_1_2_uroven.gif); }");break;
     case 105:
     case 5: ui->frame->setStyleSheet("QFrame {background-image: url(:/pict/pict/popl_1_2_3_uroven.gif); }");break;
     case 106:
     case 6: ui->frame->setStyleSheet("QFrame {background-image: url(:/pict/pict/popl_1_2_3_4_uroven.gif); }");break;
     case 10: ui->frame->setStyleSheet("QFrame {background-image: url(:/pict/pict/popl_1_2_4_uroven.gif); }");break;
     case 11: ui->frame->setStyleSheet("QFrame {background-image: url(:/pict/pict/popl_1_3_4_uroven.gif); }");break;
     case 12: ui->frame->setStyleSheet("QFrame {background-image: url(:/pict/pict/popl_1_3_uroven.gif); }");break;
     case 13: ui->frame->setStyleSheet("QFrame {background-image: url(:/pict/pict/popl_1_4_uroven.gif); }");break;
     case 14: ui->frame->setStyleSheet("QFrame {background-image: url(:/pict/pict/popl_1_2_4_uroven.gif); }");break;
     case 15: ui->frame->setStyleSheet("QFrame {background-image: url(:/pict/pict/popl_2_3_uroven.gif); }");break;
     case 16: ui->frame->setStyleSheet("QFrame {background-image: url(:/pict/pict/popl_2_4_uroven.gif); }");break;
     case 17: ui->frame->setStyleSheet("QFrame {background-image: url(:/pict/pict/popl_2_uroven.gif); }");break;
     case 18: ui->frame->setStyleSheet("QFrame {background-image: url(:/pict/pict/popl_3_4_uroven.gif); }");break;
     case 19: ui->frame->setStyleSheet("QFrame {background-image: url(:/pict/pict/popl_3_uroven.gif); }");break;
     case 20: ui->frame->setStyleSheet("QFrame {background-image: url(:/pict/pict/popl_4_uroven.gif); }");break;
     }
     if(this->data.level_to_show >= 2 && this->data.level_to_show <= 6){
        ui->labelLevel->setText(QString("%1").arg(this->data.level_to_show_sm));
     }else{
        ui->labelLevel->setText("");
     }
}
//=======================================================================================================================================
// создадим и покажем диалог с частотниками
void MainWindow::on_ShowDialogATV12_clicked(){
    dlgATV12->exec();
}
// создадим и покажем диалог с счетчиком
void MainWindow::on_ShowDialogCE303_clicked(){
    this->dlgCE303.reset(new DialogCE303( &this->data, this));
    QPoint pos = dlgCE303->pos();
    pos.setX(0);    pos.setY(0);    dlgCE303->move(pos);
    dlgCE303->exec();
}
void MainWindow::on_ShowDialogI2C_clicked(){
    dlgI2C->exec();
}
void MainWindow::on_Power_clicked(){
   dlgPower->exec();
}
void MainWindow::on_Alarm_clicked(){
   dlgAlarm->exec();
}
void MainWindow::on_Nasos1_clicked(){
   dlgNasos1->exec();
}
void MainWindow::on_Nasos2_clicked(){
   dlgNasos2->exec();
}
void MainWindow::on_Nasos3_clicked(){
   dlgNasos3->exec();
}
void MainWindow::on_Nasos4_clicked(){
   dlgNasos4->exec();
}
void MainWindow::on_setup_clicked(){
    this->dlgSetup.reset(new DialogSetup( &this->data, this));
    QPoint pos = dlgSetup->pos();
    pos.setX(0);    pos.setY(0);    dlgSetup->move(pos);
    dlgSetup->exec();
}
void MainWindow::on_service_clicked(){
    this->dlgService.reset(new DialogService( &this->data, this));
    dlgService->exec();
}
//=======================================================================================================================================
// сигналы от обновление показателей в диалогах
void MainWindow::onDataChangeATV12(){
    if (NULL != this->dlgATV12.get()){
        this->dlgATV12->updateDataATV12();
    }
}
void MainWindow::onDataChangeCE303(){
    if (NULL != this->dlgCE303.get()){
        this->dlgCE303->updateDataCE303();
    }
}
void MainWindow::onDataChangeI2C(){
    if (NULL != this->dlgI2C.get()){
        this->dlgI2C->updateDataI2C();
    }
}
void MainWindow::onDataChangePower(){
    if (NULL != this->dlgPower.get()){
        this->dlgPower->updateDataPower();
    }
}
void MainWindow::onDataChangeNasos1(){
    if (NULL != this->dlgNasos1.get()){
        this->dlgNasos1->updateDataNasos1();
    }
}
void MainWindow::onDataChangeNasos2(){
    if (NULL != this->dlgNasos2.get()){
        this->dlgNasos2->updateDataNasos2();
    }
}
void MainWindow::onDataChangeNasos3(){
    if (NULL != this->dlgNasos3.get()){
        this->dlgNasos3->updateDataNasos3();
    }
}
void MainWindow::onDataChangeNasos4(){
    if (NULL != this->dlgNasos4.get()){
        this->dlgNasos4->updateDataNasos4();
    }
}
//=======================================================================================================================================

