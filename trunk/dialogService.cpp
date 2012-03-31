#include "dialogService.h"
#include "ui_dialogService.h"
#include "data_struct.h"
#include "mainwindow.h"
#include "keypad.h"

DialogService::DialogService(data_struct * d, QWidget *parent) : QDialog(parent), dui(new Ui::DialogService){
    dui->setupUi(this);
    this->data = d;

    if(this->data->servicemode){
        dui->label->setText("Сервисный режим включен");
        dui->pushButton->show();
    }else{
        dui->label->setText("Режим оператора");
        dui->pushButton->hide();
    }
}

DialogService::~DialogService(){
    delete dui;
}
// обновим содержимое диалога
void DialogService::updateDataService(){
}


void DialogService::on_ButtonEnterService_clicked(){
    this->data->dialogparam = 4;//параметр для "подсказки"
    this->data->tempstr = "";
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();//удвинем окно в угол
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();// покажем диалог
    if(this->data->tempstr.size() > 1){//если юзер что-то ввел
        //qDebug()<< QString("%1").arg(this->data->tempstr);
        //сравнение введенного пароля и номера КНС
        if(this->data->tempstr[0] == this->data->KNSnumber[0] &&
            this->data->tempstr[1] == this->data->KNSnumber[1] &&
            this->data->tempstr[2] == this->data->KNSnumber[2] &&
            this->data->tempstr[3] == this->data->KNSnumber[2] &&
            this->data->tempstr[4] == this->data->KNSnumber[1] &&
            this->data->tempstr[5] == this->data->KNSnumber[0]){
            dui->label->setText("Сервисный режим включен");
            this->data->servicemode = true;
            this->data->servicemodeTTL = 30;//секунд
        }else{
            dui->label->setText("Неправильный пароль");
        }
    }
}
//кнопка завершения программы
void DialogService::on_pushButton_clicked(){
    this->data->tempstr = "killall";//попросим вызывавшего нас тоже завершиться
    this->close();
}
