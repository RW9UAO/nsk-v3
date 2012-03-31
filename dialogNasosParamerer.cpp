#include "dialogNasosParameter.h"
#include "ui_dialogNasosParameter.h"
#include "data_struct.h"
#include "mainwindow.h"

DialogNasosParameter::DialogNasosParameter(data_struct * d, QWidget *parent) : QDialog(parent), dui(new Ui::DialogNasosParameter){
    dui->setupUi(this);
    this->data = d;
    if(this->data->dialogparam < 0 || this->data->dialogparam > 3) return;
    if(this->data->servicemode){
        //если включен сервисный режим покажем все варианты насосов
        for(int i = 0; i < 11; i++){
            dui->comboBox->addItem(this->data->nasos_param[i].name);
        }
     }else{
        dui->comboBox->addItem(this->data->nasos_param[this->data->nasos_param_index[this->data->dialogparam]].name);
        dui->label->setPixmap( QPixmap(QString("./bd/%1").arg(this->data->nasos_param[this->data->nasos_param_index[this->data->dialogparam]].pictname) ));
    }
}

DialogNasosParameter::~DialogNasosParameter(){
    delete dui;
}
