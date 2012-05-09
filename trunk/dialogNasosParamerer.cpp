#include "dialogNasosParameter.h"
#include "ui_dialogNasosParameter.h"
#include "data_struct.h"
#include "mainwindow.h"

DialogNasosParameter::DialogNasosParameter(data_struct * d, QWidget *parent) : QDialog(parent), dui(new Ui::DialogNasosParameter){
    dui->setupUi(this);
    this->data = d;
    this->setStyleSheet("QDialog{background-image: url(:/pict/pict/black.gif);}");
    dui->label->setStyleSheet("color:rgb(255,255,255);");    dui->label_2->setStyleSheet("color:rgb(255,255,255);");
    dui->label_3->setStyleSheet("color:rgb(255,255,255);");    dui->label_4->setStyleSheet("color:rgb(255,255,255);");
    dui->label_5->setStyleSheet("color:rgb(255,255,255);");    dui->label_6->setStyleSheet("color:rgb(255,255,255);");
    dui->label_7->setStyleSheet("color:rgb(255,255,255);");    dui->label_8->setStyleSheet("color:rgb(255,255,255);");
    dui->label_9->setStyleSheet("color:rgb(255,255,255);");    dui->label_10->setStyleSheet("color:rgb(255,255,255);");
    dui->label_11->setStyleSheet("color:rgb(255,255,255);");    dui->label_12->setStyleSheet("color:rgb(255,255,255);");
    dui->label_13->setStyleSheet("color:rgb(255,255,255);");    dui->label_14->setStyleSheet("color:rgb(255,255,255);");
    dui->label_15->setStyleSheet("color:rgb(255,255,255);");
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
        }
    }
    //покажем картинку с графиком
    dui->label->setPixmap( QPixmap(QString("./bd/%1").arg(this->data->nasos_param[this->data->nasos_param_index[this->data->dialogparam]].pictname) ));
    //и артикул
    dui->label_9->setText(QString("арт %1").arg(this->data->nasos_param[this->data->nasos_param_index[this->data->dialogparam]].articul));
    dui->label_10->setText(QString("%1").arg(this->data->nasos_param[this->data->nasos_param_index[this->data->dialogparam]].pressure));
    dui->label_11->setText(QString("%1").arg(this->data->nasos_param[this->data->nasos_param_index[this->data->dialogparam]].volume));
    dui->label_12->setText(QString("%1").arg(this->data->nasos_param[this->data->nasos_param_index[this->data->dialogparam]].current));
    dui->label_13->setText(QString("%1").arg(this->data->nasos_param[this->data->nasos_param_index[this->data->dialogparam]].rpm));
    dui->label_14->setText(QString("%1").arg(this->data->nasos_param[this->data->nasos_param_index[this->data->dialogparam]].cable));
    dui->label_15->setText(QString("%1").arg(this->data->nasos_param[this->data->nasos_param_index[this->data->dialogparam]].starttype));
}

DialogNasosParameter::~DialogNasosParameter(){
    delete dui;
}

void DialogNasosParameter::on_comboBox_currentIndexChanged(int index){
//сюда попадаем при пролистывании списка артикулов. покажем выбранную картинку и запишем новое значение
    if(this->data->servicemode){
//        qDebug() << QString("index changed %1").arg(index);
        dui->label->setPixmap( QPixmap(QString("./bd/%1").arg(this->data->nasos_param[index].pictname) ));
        this->data->nasos_param_index[this->data->dialogparam] = index;
        dui->label_9->setText(QString("арт %1").arg(this->data->nasos_param[this->data->nasos_param_index[this->data->dialogparam]].articul));
        dui->label_10->setText(QString("%1").arg(this->data->nasos_param[this->data->nasos_param_index[this->data->dialogparam]].pressure));
        dui->label_11->setText(QString("%1").arg(this->data->nasos_param[this->data->nasos_param_index[this->data->dialogparam]].volume));
        dui->label_12->setText(QString("%1").arg(this->data->nasos_param[this->data->nasos_param_index[this->data->dialogparam]].current));
        dui->label_13->setText(QString("%1").arg(this->data->nasos_param[this->data->nasos_param_index[this->data->dialogparam]].rpm));
        dui->label_14->setText(QString("%1").arg(this->data->nasos_param[this->data->nasos_param_index[this->data->dialogparam]].cable));
        dui->label_15->setText(QString("%1").arg(this->data->nasos_param[this->data->nasos_param_index[this->data->dialogparam]].starttype));
    }
}
