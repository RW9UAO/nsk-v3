#include "keypad.h"
#include "ui_keypad.h"
#include "data_struct.h"
#include "mainwindow.h"

Dialogkeypad::Dialogkeypad(data_struct * d, QWidget *parent) : QDialog(parent), dui(new Ui::Dialogkeypad){
    dui->setupUi(this);
    this->data = d;
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    dui->lineEdit->setEchoMode(QLineEdit::Normal);
    switch(this->data->dialogparam){
    case 1:
        dui->label->setText("Время вводится в виде ЧЧ.ММ.СС\nнапример 21.00.00");
        break;
    case 2:
        dui->label->setText("Дата в виде nn.dd.mm.yy 01.23.10.12\nгде nn день недели: 00-вск, 01-пнд");
        break;
    case 3:
        dui->label->setText("Токовый трансформатор. Без точки.");
        break;
    case 4:
        dui->label->setText("Введите пароль");
        dui->lineEdit->setEchoMode(QLineEdit::Password);
        break;
    case 5:
        dui->label->setText("Если нет - введите 0\nесли есть - любую цифру");
        break;
    default:
        //this->close();
        dui->label->setText("");
        break;
    }
    //this->data->dialogparam = 0;
}

Dialogkeypad::~Dialogkeypad(){
    delete dui;
}
void Dialogkeypad::MyEventHandler(){
    this->close();
}


void Dialogkeypad::on_b1_clicked(){
    dui->lineEdit->insert("1");
    if(dui->lineEdit->text().size() == 6 && this->data->dialogparam == 4) on_save_clicked();
}
void Dialogkeypad::on_b2_clicked(){
    dui->lineEdit->insert("2");
    if(dui->lineEdit->text().size() == 6 && this->data->dialogparam == 4) on_save_clicked();
}
void Dialogkeypad::on_b4_clicked(){
    dui->lineEdit->insert("4");
    if(dui->lineEdit->text().size() == 6 && this->data->dialogparam == 4) on_save_clicked();
}
void Dialogkeypad::on_b3_clicked(){
    dui->lineEdit->insert("3");
    if(dui->lineEdit->text().size() == 6 && this->data->dialogparam == 4) on_save_clicked();
}
void Dialogkeypad::on_b5_clicked(){
    dui->lineEdit->insert("5");
    if(dui->lineEdit->text().size() == 6 && this->data->dialogparam == 4) on_save_clicked();
}
void Dialogkeypad::on_b6_clicked(){
    dui->lineEdit->insert("6");
    if(dui->lineEdit->text().size() == 6 && this->data->dialogparam == 4) on_save_clicked();
}
void Dialogkeypad::on_b7_clicked(){
    dui->lineEdit->insert("7");
    if(dui->lineEdit->text().size() == 6 && this->data->dialogparam == 4) on_save_clicked();
}
void Dialogkeypad::on_b8_clicked(){
    dui->lineEdit->insert("8");
    if(dui->lineEdit->text().size() == 6 && this->data->dialogparam == 4) on_save_clicked();
}
void Dialogkeypad::on_b9_clicked(){
    dui->lineEdit->insert("9");
    if(dui->lineEdit->text().size() == 6 && this->data->dialogparam == 4) on_save_clicked();
}
void Dialogkeypad::on_b0_clicked(){
    dui->lineEdit->insert("0");
    if(dui->lineEdit->text().size() == 6 && this->data->dialogparam == 4) on_save_clicked();
}
void Dialogkeypad::on_bdot_clicked(){
    dui->lineEdit->insert(".");
}

void Dialogkeypad::on_bbs_clicked(){
    dui->lineEdit->backspace();
}
void Dialogkeypad::on_save_clicked(){
    this->data->tempstr = dui->lineEdit->text();//cкопируем в глобальную переменную
    this->close();
}
void Dialogkeypad::on_save_2_clicked(){
    this->data->tempstr = "";//юзер нифига не ввел
    this->close();
}
