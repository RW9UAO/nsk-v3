#include "keypad.h"
#include "ui_keypad.h"
#include "data_struct.h"
#include "mainwindow.h"

Dialogkeypad::Dialogkeypad(data_struct * d, QWidget *parent) : QDialog(parent), dui(new Ui::Dialogkeypad){
    dui->setupUi(this);
    this->data = d;
    //this->setWindowFlags(Qt::CustomizeWindowHint);
    if(this->data->dialogparam == 1){
        //dui->label->setText("Время вводится в виде ЧЧ.ММ.СС\nнапример 23.00.00");
        dui->label->setText("hh.mm.ss 23.00.00");
    }
    if(this->data->dialogparam == 2){
        dui->label->setText("nn.dd.mm.yy 01.23.10.12\n00-вск, 01-пнд");
    }
    if(this->data->dialogparam == 3){
        dui->label->setText("Current trans");
    }
}

Dialogkeypad::~Dialogkeypad(){
    delete dui;
}
void Dialogkeypad::MyEventHandler(){
    this->close();
}


void Dialogkeypad::on_b1_clicked(){
    dui->lineEdit->insert("1");
}
void Dialogkeypad::on_b2_clicked(){
    dui->lineEdit->insert("2");
}
void Dialogkeypad::on_b4_clicked(){
    dui->lineEdit->insert("4");
}
void Dialogkeypad::on_b3_clicked(){
    dui->lineEdit->insert("3");
}
void Dialogkeypad::on_b5_clicked(){
    dui->lineEdit->insert("5");
}
void Dialogkeypad::on_b6_clicked(){
    dui->lineEdit->insert("6");
}
void Dialogkeypad::on_b7_clicked(){
    dui->lineEdit->insert("7");
}
void Dialogkeypad::on_b8_clicked(){
    dui->lineEdit->insert("8");
}
void Dialogkeypad::on_b9_clicked(){
    dui->lineEdit->insert("9");
}
void Dialogkeypad::on_b0_clicked(){
    dui->lineEdit->insert("0");
}
void Dialogkeypad::on_bdot_clicked(){
    dui->lineEdit->insert(".");
}

void Dialogkeypad::on_bbs_clicked(){
    dui->lineEdit->backspace();
}
void Dialogkeypad::on_save_clicked(){
    this->data->tempstr = dui->lineEdit->text();
    this->close();
}
void Dialogkeypad::on_save_2_clicked(){
    this->data->tempstr = "";
    this->close();
}
