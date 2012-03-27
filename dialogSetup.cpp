#include "dialogSetup.h"
#include "ui_dialogSetup.h"
#include "data_struct.h"
#include "mainwindow.h"

DialogSetup::DialogSetup(data_struct * d, QWidget *parent) : QDialog(parent), dui(new Ui::DialogSetup){
    dui->setupUi(this);
    this->data = d;
    this->setWindowFlags(Qt::FramelessWindowHint);
    dui->P1->setText(QString("%1").arg(this->data->level_1_sm));
    dui->P2->setText(QString("%1").arg(this->data->level_2_sm));
    dui->P3->setText(QString("%1").arg(this->data->level_3_sm));
    dui->P4->setText(QString("%1").arg(this->data->level_4_sm));
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
