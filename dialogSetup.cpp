#include "dialogSetup.h"
#include "ui_dialogSetup.h"
#include "data_struct.h"
#include "mainwindow.h"

DialogSetup::DialogSetup(data_struct * d, QWidget *parent) : QDialog(parent), dui(new Ui::DialogSetup){
    dui->setupUi(this);
    this->data = d;
    //this->setWindowFlags(Qt::FramelessWindowHint);
    //this->updatesEnabled();
    //this->updatesEnabled();
    dui->P1->setText(QString("%1").arg(this->data->level_1_sm));
    dui->P2->setText(QString("%1").arg(this->data->level_2_sm));
    dui->P3->setText(QString("%1").arg(this->data->level_3_sm));
    dui->P4->setText(QString("%1").arg(this->data->level_4_sm));
    setStyleSheet("QDialog{background-image: url(:/pict/pict/ustanovki_fon.gif);}");
    if(this->data->servicemode){
        dui->P1->setEnabled(true);dui->P2->setEnabled(true);
        dui->P3->setEnabled(true);dui->P4->setEnabled(true);
    }else{
        dui->P1->setDisabled(true);dui->P2->setDisabled(true);
        dui->P3->setDisabled(true);dui->P4->setDisabled(true);
        dui->P1->setStyleSheet("background-image: url(none);color:rgb(0,0,0)");
        dui->P2->setStyleSheet("background-image: url(none);color:rgb(0,0,0)");
        dui->P3->setStyleSheet("background-image: url(none);color:rgb(0,0,0)");
        dui->P4->setStyleSheet("background-image: url(none);color:rgb(0,0,0)");
    }
}

DialogSetup::~DialogSetup(){
    delete dui;
}
void DialogSetup::MyEventHandler(){
    this->close();
}
// обновим содержимое диалога
void DialogSetup::updateDataSetup(){
}


void DialogSetup::on_pushButton_clicked(){
    this->close();
}

void DialogSetup::on_P1_clicked(){
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        //qDebug()<< QString("%1").arg(this->data->tempstr);
        int temp = this->data->tempstr.toInt();
        if(temp < 100 && temp > 0){
            this->data->level_1_sm = temp;
            //this->setUpdatesEnabled(true);
            //this->repaint();
            //this->setUpdatesEnabled(false);
            dui->P1->setText(QString("%1").arg(this->data->level_1_sm));
        }else{
            QMessageBox msgBox;
            msgBox.setText("неправильное значение.\nНадо от 1 до 100 см.");
            msgBox.exec();
            qDebug("error on poplavok1 user input");
        }
    }
}

void DialogSetup::on_P2_clicked(){
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        int temp = this->data->tempstr.toInt();
        if(temp < 100 && temp > this->data->level_1_sm){
            this->data->level_2_sm = temp;
            dui->P2->setText(QString("%1").arg(this->data->level_2_sm));
        }else{
            QMessageBox msgBox;
            msgBox.setText(QString("неправильное значение.\nНадо от %1 до 100 см.").arg(this->data->level_1_sm));
            msgBox.exec();
            qDebug("error on poplavok2 user input");
        }
    }
}

void DialogSetup::on_P3_clicked(){
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        int temp = this->data->tempstr.toInt();
        if(temp < 100 && temp > this->data->level_2_sm){
            this->data->level_3_sm = temp;
            dui->P3->setText(QString("%1").arg(this->data->level_3_sm));
        }else{
            QMessageBox msgBox;
            msgBox.setText(QString("неправильное значение.\nНадо от %1 до 100 см.").arg(this->data->level_2_sm));
            msgBox.exec();
            qDebug("error on poplavok3 user input");
        }
    }
}

void DialogSetup::on_P4_clicked(){
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        int temp = this->data->tempstr.toInt();
        if(temp < 100 && temp > this->data->level_3_sm){
            this->data->level_4_sm = temp;
            dui->P4->setText(QString("%1").arg(this->data->level_4_sm));
        }else{
            QMessageBox msgBox;
            msgBox.setText(QString("неправильное значение.\nНадо от %1 до 100 см.").arg(this->data->level_3_sm));
            msgBox.exec();
            qDebug("error on poplavok4 user input");
        }
    }
}
