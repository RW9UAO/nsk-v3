#include "dialogNasosParameter.h"
#include "ui_dialogNasosParameter.h"
#include "data_struct.h"
#include "mainwindow.h"

DialogNasosParameter::DialogNasosParameter(data_struct * d, QWidget *parent) : QDialog(parent), dui(new Ui::DialogNasosParameter){
    dui->setupUi(this);
    this->data = d;
    //если номер насоса не указан, завершим
    if(this->data->dialogparam < 0 || this->data->dialogparam > 3) return;
    dui->label_8->setText(QString("Насос %1").arg(this->data->dialogparam + 1));//номер запрошенного насоса
    if(this->data->servicemode){
        //если включен сервисный режим покажем все варианты насосов
        for(int i = 0; i < 11; i++){
            dui->comboBox->addItem(this->data->nasos_param[i].name);
        }
     }else{
        if(this->data->nasos_param_index[this->data->dialogparam] < 11){
            //покажем текущий насос
            dui->comboBox->addItem(this->data->nasos_param[this->data->nasos_param_index[this->data->dialogparam]].name);
            //покажем картинку с графиком
            dui->label->setPixmap( QPixmap(QString("./bd/%1").arg(this->data->nasos_param[this->data->nasos_param_index[this->data->dialogparam]].pictname) ));
            //и артикул
            dui->label_9->setText(QString("арт %1").arg(this->data->nasos_param[this->data->nasos_param_index[this->data->dialogparam]].articul));
        }
    }
}

DialogNasosParameter::~DialogNasosParameter(){
    delete dui;
}
