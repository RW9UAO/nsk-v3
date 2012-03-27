#include "dialogCE303.h"
#include "ui_dialogCE303.h"
#include "data_struct.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


DialogCE303::DialogCE303(data_struct * d, QWidget *parent) : QDialog(parent), dui(new Ui::DialogCE303){
    dui->setupUi(this);
    this->data = d;
    if(this->data->servicemode){
        dui->pushButton->setEnabled(true);
        dui->pushButton_2->setEnabled(true);
        dui->pushButton_3->setEnabled(true);
    }else{
        dui->pushButton->setDisabled(true);
        dui->pushButton_2->setDisabled(true);
        dui->pushButton_3->setDisabled(true);
    }
}

DialogCE303::~DialogCE303(){
    delete dui;
}
void DialogCE303::MyEventHandler(){
    this->close();
}

void DialogCE303::updateDataCE303(){

    dui->date_->setText(this->data->date_);
    dui->time_->setText(this->data->time_);
    if(this->data->volta != -1){
        QString s = QString("%1 %2 %3").arg(this->data->volta).arg(this->data->voltb).arg(this->data->voltc);
        dui->volta->setText(s);
    }else{
        dui->volta->setText("--");
    }
    if(this->data->fccur != -1){
        QString s = QString("%1").arg(this->data->fccur);
        dui->fccur->setText(s);
    }else{
        dui->fccur->setText("--");
    }
    if(this->data->powep != -1){
        QString s = QString("%1").arg(this->data->powep);
        dui->powep->setText(s);
    }else{
        dui->powep->setText("--");
    }
    if(this->data->poweq != -1){
        QString s = QString("%1").arg(this->data->poweq);
        dui->poweq->setText(s);
    }else{
        dui->poweq->setText("--");
    }
    if(this->data->curra != -1){
        QString s = QString("%1 %2 %3").arg(this->data->curra).arg(this->data->currb).arg(this->data->currc);
        dui->currABC->setText(s);
    }else{
        dui->currABC->setText("--");
    }
    if(this->data->cos_f != -1){
        QString s = QString("%1").arg(this->data->cos_f);
        dui->COS_f->setText(s);
    }else{
        dui->COS_f->setText("--");
    }
    if(this->data->angleAB != -1){
        QString s = QString("%1 %2 %3").arg(this->data->angleAB).arg(this->data->angleBC).arg(this->data->angleCA);
        dui->angles->setText(s);
    }else{
        dui->angles->setText("--");
    }
    if(this->data->et0pe != -1){
        QString s = QString("%1").arg(this->data->et0pe);
        dui->et0pe->setText(s);
    }else{
        dui->et0pe->setText("--");
    }
}
//кнопка "Врeмя"
void DialogCE303::on_pushButton_clicked(){
    this->data->dialogparam = 1;//параметр для "подсказки"
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();//удвинем окно в угол
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();// покажем диалог
    if(this->data->tempstr.size() == 8){//если юзер что-то ввел
        //qDebug()<< QString("%1").arg(this->data->tempstr);
        this->data->CE303needupdate = update_time;
    }
    //delete d_key;
}
//кнопка "Дата"
void DialogCE303::on_pushButton_2_clicked(){
    this->data->dialogparam = 2;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 8){
        //qDebug()<< QString("%1").arg(this->data->tempstr);
        this->data->CE303needupdate = update_date;
    }
}
//кнопка "к-т тр-ра"
void DialogCE303::on_pushButton_3_clicked(){
    this->data->dialogparam = 3;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        //qDebug()<< QString("%1").arg(this->data->tempstr);
        this->data->CE303needupdate = update_trans;
    }

}
