#include "dialogSetupGeneral.h"
#include "ui_dialogSetupGeneral.h"
#include "data_struct.h"
#include "mainwindow.h"

DialogSetupGeneral::DialogSetupGeneral(data_struct * d, QWidget *parent) : QDialog(parent), dui(new Ui::DialogSetupGeneral){
    dui->setupUi(this);

    this->data = d;
    setStyleSheet("QDialog{background-color: rgb(190,190,190); }");
    dui->pushButton_2->setText(QString("%1").arg(this->data->KNSnumber));
    dui->pushButton_3->setText(QString("%1").arg(this->data->pca9555ADDR));
    dui->pushButton_4->setText(QString("%1").arg(this->data->max11616ADDR));
    if(this->data->isSoftStart)dui->pushButton_6->setText("yes");
        else    dui->pushButton_6->setText("no");
    if(this->data->isATV12)dui->pushButton_7->setText("yes");
        else    dui->pushButton_7->setText("no");
    if(this->data->islevel_meter)dui->pushButton_8->setText("уровнемер");
        else    dui->pushButton_6->setText("поплавки");
    dui->pushButton_9->setText(QString("%1").arg(this->data->level_empty_raw));
    dui->pushButton_10->setText(QString("%1").arg(this->data->level_full_raw));
    dui->pushButton_11->setText(QString("%1").arg(this->data->level_full_sm));
    dui->pushButton_12->setText(QString("%1").arg(this->data->overlevel_time_to_stop));
    dui->pushButton_15->setText(QString("%1 %2 %3").arg(this->data->Pparam).arg(this->data->Iparam).arg(this->data->Dparam));
    if(this->data->servicemode){
        dui->pushButton_2->setEnabled(true);        dui->pushButton_3->setEnabled(true);
        dui->pushButton_4->setEnabled(true);        dui->pushButton_5->setEnabled(true);
        dui->pushButton_6->setEnabled(true);        dui->pushButton_7->setEnabled(true);
        dui->pushButton_8->setEnabled(true);        dui->pushButton_9->setEnabled(true);
        dui->pushButton_10->setEnabled(true);        dui->pushButton_13->setEnabled(true);
        dui->pushButton_11->setEnabled(true);        dui->pushButton_14->setEnabled(true);
        dui->pushButton_12->setEnabled(true);       dui->pushButton_15->setEnabled(true);
    }else{
        dui->pushButton_2->setDisabled(true);dui->pushButton_9->setDisabled(true);
        dui->pushButton_3->setDisabled(true);dui->pushButton_10->setDisabled(true);
        dui->pushButton_4->setDisabled(true);dui->pushButton_11->setDisabled(true);
        dui->pushButton_5->setDisabled(true);dui->pushButton_12->setDisabled(true);
        dui->pushButton_6->setDisabled(true);dui->pushButton_13->setDisabled(true);
        dui->pushButton_7->setDisabled(true);dui->pushButton_14->setDisabled(true);
        dui->pushButton_8->setDisabled(true);dui->pushButton_15->setDisabled(true);
    }
}

DialogSetupGeneral::~DialogSetupGeneral(){
    delete dui;
}
void DialogSetupGeneral::MyEventHandler(){
    this->close();
}
void DialogSetupGeneral::on_pushButton_clicked(){
    this->close();
}

void DialogSetupGeneral::on_pushButton_2_clicked(){//КНС
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0 && this->data->tempstr.size() < 4){
        this->data->KNSnumber = this->data->tempstr;
        dui->pushButton_2->setText(QString("%1").arg(this->data->KNSnumber));
    }
}

void DialogSetupGeneral::on_pushButton_3_clicked(){//21
    bool ok; int temp;
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() == 1){
        temp = this->data->tempstr.toInt(&ok);
        if(ok){
            this->data->pca9555ADDR = temp;
            dui->pushButton_3->setText(QString("%1").arg(this->data->pca9555ADDR));
         }
    }
}

void DialogSetupGeneral::on_pushButton_4_clicked(){//35
    bool ok; int temp;
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() == 1){
        temp = this->data->tempstr.toInt(&ok);
        if(ok){
            this->data->max11616ADDR = temp;
            dui->pushButton_4->setText(QString("%1").arg(this->data->max11616ADDR));
         }
    }
}

void DialogSetupGeneral::on_pushButton_6_clicked(){//softstart
    this->data->dialogparam = 5;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        if(this->data->tempstr == QString("0") ){
            this->data->isSoftStart = false;
            dui->pushButton_6->setText("no");
        }else{
            this->data->isSoftStart = true;
            dui->pushButton_6->setText("yes");
        }
    }
}

void DialogSetupGeneral::on_pushButton_7_clicked(){//atv12
    this->data->dialogparam = 5;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        if(this->data->tempstr == QString("0") ){
            this->data->isATV12 = false;
            dui->pushButton_7->setText("no");
        }else{
            this->data->isATV12 = true;
            dui->pushButton_7->setText("yes");
        }
    }
}

void DialogSetupGeneral::on_pushButton_8_clicked(){
    this->data->dialogparam = 5;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        if(this->data->tempstr == QString("0") ){
            this->data->islevel_meter = false;
            dui->pushButton_8->setText("no");
        }else{
            this->data->islevel_meter = true;
            dui->pushButton_8->setText("yes");
        }
    }
}

void DialogSetupGeneral::on_pushButton_9_clicked(){//пустой бак с АЦП
    bool ok; int temp;
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        temp = this->data->tempstr.toInt(&ok);
        if(ok){
            this->data->level_empty_raw = temp;
            dui->pushButton_9->setText(QString("%1").arg(this->data->level_empty_raw));
         }
    }
}

void DialogSetupGeneral::on_pushButton_10_clicked(){
    bool ok; int temp;
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        temp = this->data->tempstr.toInt(&ok);
        if(ok){
            this->data->level_full_raw = temp;
            dui->pushButton_10->setText(QString("%1").arg(this->data->level_full_raw));
         }
    }
}

void DialogSetupGeneral::on_pushButton_11_clicked(){
    bool ok; int temp;
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        temp = this->data->tempstr.toInt(&ok);
        if(ok){
            this->data->level_full_sm = temp;
            dui->pushButton_11->setText(QString("%1").arg(this->data->level_full_sm));
         }
    }
}

void DialogSetupGeneral::on_pushButton_12_clicked(){
    bool ok; int temp;
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        temp = this->data->tempstr.toInt(&ok);
        if(ok){
            this->data->overlevel_time_to_stop = temp;
            dui->pushButton_12->setText(QString("%1").arg(this->data->overlevel_time_to_stop));
         }
    }
}

void DialogSetupGeneral::on_pushButton_16_clicked(){//калибровка    
    QDialog * d_key = new DialogCalibr(this->data, this);
    d_key->setWindowFlags(Qt::FramelessWindowHint);
    d_key->setStyleSheet("QDialog {background-color: rgb(120,120,120); }");
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
}
