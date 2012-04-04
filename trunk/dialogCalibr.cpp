#include "dialogCalibr.h"
#include "ui_dialogCalibr.h"
#include "data_struct.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

DialogCalibr::DialogCalibr(data_struct * d, QWidget *parent) : QDialog(parent), dui(new Ui::DialogCalibr){
    dui->setupUi(this);
    this->data = d;
    dui->i2c_raw->setText(QString("%1").arg(this->data->max11616[this->data->level_input_number]));
    dui->empty_raw->setText(QString("%1").arg(this->data->level_empty_raw));
    dui->full_raw->setText(QString("%1").arg(this->data->level_full_raw));
    dui->pushButton_4->setText(QString("%1").arg(this->data->nasos_bit[0]));
    dui->pushButton_5->setText(QString("%1").arg(this->data->nasos_bit[1]));
    dui->pushButton_6->setText(QString("%1").arg(this->data->nasos_bit[2]));
    dui->pushButton_7->setText(QString("%1").arg(this->data->nasos_bit[3]));
    dui->pushButton_14->setText(QString("%1").arg(this->data->nasos_temp_alarm_bit[0]));
    dui->pushButton_15->setText(QString("%1").arg(this->data->nasos_temp_alarm_bit[1]));
    dui->pushButton_16->setText(QString("%1").arg(this->data->nasos_temp_alarm_bit[2]));
    dui->pushButton_17->setText(QString("%1").arg(this->data->nasos_temp_alarm_bit[3]));
    dui->pushButton_18->setText(QString("%1").arg(this->data->level_1_bit));
    dui->pushButton_19->setText(QString("%1").arg(this->data->level_2_bit));
    dui->pushButton_20->setText(QString("%1").arg(this->data->level_3_bit));
    dui->pushButton_21->setText(QString("%1").arg(this->data->level_4_bit));
    dui->pushButton_22->setText(QString("%1").arg(this->data->nasos_current_alarm_bit[0]));
    dui->pushButton_23->setText(QString("%1").arg(this->data->nasos_current_alarm_bit[1]));
    dui->pushButton_24->setText(QString("%1").arg(this->data->nasos_current_alarm_bit[2]));
    dui->pushButton_25->setText(QString("%1").arg(this->data->nasos_current_alarm_bit[3]));
    dui->label_18->setText(QString("%1").arg(this->data->max11616[this->data->nasos_current_alarm_bit[0]]));
    dui->label_19->setText(QString("%1").arg(this->data->max11616[this->data->nasos_current_alarm_bit[1]]));
    dui->label_20->setText(QString("%1").arg(this->data->max11616[this->data->nasos_current_alarm_bit[2]]));
    dui->label_21->setText(QString("%1").arg(this->data->max11616[this->data->nasos_current_alarm_bit[3]]));
    dui->label_37->setText(QString("%1").arg(this->data->max11616[this->data->nasos_temp_alarm_bit[0]]));
    dui->label_23->setText(QString("%1").arg(this->data->max11616[this->data->nasos_temp_alarm_bit[1]]));
    dui->label_39->setText(QString("%1").arg(this->data->max11616[this->data->nasos_temp_alarm_bit[2]]));
    dui->label_24->setText(QString("%1").arg(this->data->max11616[this->data->nasos_temp_alarm_bit[3]]));
    dui->pushButton_8->setText(QString("%1").arg(this->data->nasos_current_alarm_border[0]));
    dui->pushButton_9->setText(QString("%1").arg(this->data->nasos_current_alarm_border[1]));
    dui->pushButton_10->setText(QString("%1").arg(this->data->nasos_current_alarm_border[2]));
    dui->pushButton_11->setText(QString("%1").arg(this->data->nasos_current_alarm_border[3]));
    dui->pushButton_27->setText(QString("%1").arg(this->data->nasos_temp_alarm_border[0]));
    dui->pushButton_13->setText(QString("%1").arg(this->data->nasos_temp_alarm_border[1]));
    dui->pushButton_26->setText(QString("%1").arg(this->data->nasos_temp_alarm_border[2]));
    dui->pushButton_12->setText(QString("%1").arg(this->data->nasos_temp_alarm_border[3]));
    dui->pushButton_31->setText(QString("%1").arg(this->data->nasos_wet_alarm_border[0]));
    dui->pushButton_29->setText(QString("%1").arg(this->data->nasos_wet_alarm_border[1]));
    dui->pushButton_30->setText(QString("%1").arg(this->data->nasos_wet_alarm_border[2]));
    dui->pushButton_28->setText(QString("%1").arg(this->data->nasos_wet_alarm_border[3]));
}

DialogCalibr::~DialogCalibr(){
    delete dui;
}
// обновим содержимое диалога
void DialogCalibr::updateDataCalibr(){
    dui->i2c_raw->setText(QString("%1").arg(this->data->max11616[this->data->level_input_number]));
    dui->label_18->setText(QString("%1").arg(this->data->max11616[this->data->nasos_current_alarm_bit[0]]));
    dui->label_19->setText(QString("%1").arg(this->data->max11616[this->data->nasos_current_alarm_bit[1]]));
    dui->label_20->setText(QString("%1").arg(this->data->max11616[this->data->nasos_current_alarm_bit[2]]));
    dui->label_21->setText(QString("%1").arg(this->data->max11616[this->data->nasos_current_alarm_bit[3]]));
    dui->label_37->setText(QString("%1").arg(this->data->max11616[this->data->nasos_temp_alarm_bit[0]]));
    dui->label_23->setText(QString("%1").arg(this->data->max11616[this->data->nasos_temp_alarm_bit[1]]));
    dui->label_39->setText(QString("%1").arg(this->data->max11616[this->data->nasos_temp_alarm_bit[2]]));
    dui->label_24->setText(QString("%1").arg(this->data->max11616[this->data->nasos_temp_alarm_bit[3]]));
}

void DialogCalibr::on_pushButton_clicked(){
    this->close();
}

void DialogCalibr::on_pushButton_2_clicked(){//empty
    this->data->level_empty_raw = this->data->max11616[this->data->level_input_number];
    dui->empty_raw->setText(QString("%1").arg(this->data->level_empty_raw));
}

void DialogCalibr::on_pushButton_3_clicked(){//full
    this->data->level_full_raw = this->data->max11616[this->data->level_input_number];
    dui->full_raw->setText(QString("%1").arg(this->data->level_full_raw));
}

void DialogCalibr::on_pushButton_4_clicked(){
    bool ok; int temp;
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        temp = this->data->tempstr.toInt(&ok);
        if(ok){
            this->data->nasos_bit[0] = temp;
            dui->pushButton_4->setText(QString("%1").arg(this->data->nasos_bit[0]));
         }
    }
}

void DialogCalibr::on_pushButton_5_clicked(){
    bool ok; int temp;
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        temp = this->data->tempstr.toInt(&ok);
        if(ok){
            this->data->nasos_bit[1] = temp;
            dui->pushButton_5->setText(QString("%1").arg(this->data->nasos_bit[1]));
         }
    }
}

void DialogCalibr::on_pushButton_6_clicked(){
    bool ok; int temp;
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        temp = this->data->tempstr.toInt(&ok);
        if(ok){
            this->data->nasos_bit[2] = temp;
            dui->pushButton_6->setText(QString("%1").arg(this->data->nasos_bit[2]));
         }
    }
}

void DialogCalibr::on_pushButton_7_clicked(){
    bool ok; int temp;
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        temp = this->data->tempstr.toInt(&ok);
        if(ok){
            this->data->nasos_bit[3] = temp;
            dui->pushButton_7->setText(QString("%1").arg(this->data->nasos_bit[3]));
         }
    }
}

void DialogCalibr::on_pushButton_14_clicked(){
    bool ok; int temp;
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        temp = this->data->tempstr.toInt(&ok);
        if(ok){
            this->data->nasos_temp_alarm_bit[0] = temp;
            dui->pushButton_14->setText(QString("%1").arg(this->data->nasos_temp_alarm_bit[0]));
         }
    }
}
void DialogCalibr::on_pushButton_15_clicked(){
    bool ok; int temp;
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        temp = this->data->tempstr.toInt(&ok);
        if(ok){
            this->data->nasos_temp_alarm_bit[1] = temp;
            dui->pushButton_15->setText(QString("%1").arg(this->data->nasos_temp_alarm_bit[1]));
         }
    }
}
void DialogCalibr::on_pushButton_16_clicked(){
    bool ok; int temp;
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        temp = this->data->tempstr.toInt(&ok);
        if(ok){
            this->data->nasos_temp_alarm_bit[2] = temp;
            dui->pushButton_16->setText(QString("%1").arg(this->data->nasos_temp_alarm_bit[2]));
         }
    }
}
void DialogCalibr::on_pushButton_17_clicked(){
    bool ok; int temp;
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        temp = this->data->tempstr.toInt(&ok);
        if(ok){
            this->data->nasos_temp_alarm_bit[3] = temp;
            dui->pushButton_17->setText(QString("%1").arg(this->data->nasos_temp_alarm_bit[3]));
         }
    }
}

void DialogCalibr::on_pushButton_18_clicked(){
    bool ok; int temp;
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        temp = this->data->tempstr.toInt(&ok);
        if(ok){
            this->data->level_1_bit = temp;
            dui->pushButton_18->setText(QString("%1").arg(this->data->level_1_bit));
         }
    }
}
void DialogCalibr::on_pushButton_19_clicked(){
    bool ok; int temp;
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        temp = this->data->tempstr.toInt(&ok);
        if(ok){
            this->data->level_2_bit = temp;
            dui->pushButton_19->setText(QString("%1").arg(this->data->level_2_bit));
         }
    }
}
void DialogCalibr::on_pushButton_20_clicked(){
    bool ok; int temp;
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        temp = this->data->tempstr.toInt(&ok);
        if(ok){
            this->data->level_3_bit = temp;
            dui->pushButton_20->setText(QString("%1").arg(this->data->level_3_bit));
         }
    }
}
void DialogCalibr::on_pushButton_21_clicked(){
    bool ok; int temp;
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        temp = this->data->tempstr.toInt(&ok);
        if(ok){
            this->data->level_4_bit = temp;
            dui->pushButton_21->setText(QString("%1").arg(this->data->level_4_bit));
         }
    }
}

void DialogCalibr::on_pushButton_22_clicked(){
    bool ok; int temp;
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        temp = this->data->tempstr.toInt(&ok);
        if(ok){
            this->data->nasos_current_alarm_bit[0] = temp;
            dui->pushButton_22->setText(QString("%1").arg(this->data->nasos_current_alarm_bit[0]));
         }
    }
}
void DialogCalibr::on_pushButton_23_clicked(){
    bool ok; int temp;
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        temp = this->data->tempstr.toInt(&ok);
        if(ok){
            this->data->nasos_current_alarm_bit[1] = temp;
            dui->pushButton_23->setText(QString("%1").arg(this->data->nasos_current_alarm_bit[1]));
         }
    }
}
void DialogCalibr::on_pushButton_24_clicked(){
    bool ok; int temp;
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        temp = this->data->tempstr.toInt(&ok);
        if(ok){
            this->data->nasos_current_alarm_bit[2] = temp;
            dui->pushButton_24->setText(QString("%1").arg(this->data->nasos_current_alarm_bit[2]));
         }
    }
}
void DialogCalibr::on_pushButton_25_clicked(){
    bool ok; int temp;
    this->data->dialogparam = 0;
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();
    if(this->data->tempstr.size() > 0){
        temp = this->data->tempstr.toInt(&ok);
        if(ok){
            this->data->nasos_current_alarm_bit[3] = temp;
            dui->pushButton_25->setText(QString("%1").arg(this->data->nasos_current_alarm_bit[3]));
         }
    }
}

void DialogCalibr::on_pushButton_8_clicked(){
    this->data->nasos_current_alarm_border[0] = (double)this->data->max11616[this->data->nasos_current_alarm_bit[0]] * 0.9;
    dui->pushButton_8->setText(QString("%1").arg(this->data->nasos_current_alarm_border[0]));
}
void DialogCalibr::on_pushButton_9_clicked(){
    this->data->nasos_current_alarm_border[1] = (double)this->data->max11616[this->data->nasos_current_alarm_bit[1]] * 0.9;
    dui->pushButton_9->setText(QString("%1").arg(this->data->nasos_current_alarm_border[1]));
}
void DialogCalibr::on_pushButton_10_clicked(){
    this->data->nasos_current_alarm_border[2] = (double)this->data->max11616[this->data->nasos_current_alarm_bit[2]] * 0.9;
    dui->pushButton_10->setText(QString("%1").arg(this->data->nasos_current_alarm_border[2]));
}
void DialogCalibr::on_pushButton_11_clicked(){
    this->data->nasos_current_alarm_border[3] = (double)this->data->max11616[this->data->nasos_current_alarm_bit[3]] * 0.9;
    dui->pushButton_11->setText(QString("%1").arg(this->data->nasos_current_alarm_border[3]));
}

void DialogCalibr::on_pushButton_27_clicked(){
    this->data->nasos_temp_alarm_border[0] = (double)this->data->max11616[this->data->nasos_temp_alarm_bit[0]] * 0.9;
    dui->pushButton_27->setText(QString("%1").arg(this->data->nasos_temp_alarm_border[0]));
}
void DialogCalibr::on_pushButton_13_clicked(){
    this->data->nasos_temp_alarm_border[1] = (double)this->data->max11616[this->data->nasos_temp_alarm_bit[1]] * 0.9;
    dui->pushButton_13->setText(QString("%1").arg(this->data->nasos_temp_alarm_border[1]));
}
void DialogCalibr::on_pushButton_26_clicked(){
    this->data->nasos_temp_alarm_border[2] = (double)this->data->max11616[this->data->nasos_temp_alarm_bit[2]] * 0.9;
    dui->pushButton_26->setText(QString("%1").arg(this->data->nasos_temp_alarm_border[2]));
}
void DialogCalibr::on_pushButton_12_clicked(){
    this->data->nasos_temp_alarm_border[3] = (double)this->data->max11616[this->data->nasos_temp_alarm_bit[3]] * 0.9;
    dui->pushButton_12->setText(QString("%1").arg(this->data->nasos_temp_alarm_border[3]));
}

void DialogCalibr::on_pushButton_31_clicked(){
    this->data->nasos_wet_alarm_border[0] = (double)this->data->max11616[this->data->nasos_temp_alarm_bit[0]] * 0.9;
    dui->pushButton_31->setText(QString("%1").arg(this->data->nasos_wet_alarm_border[0]));
}
void DialogCalibr::on_pushButton_29_clicked(){
    this->data->nasos_wet_alarm_border[1] = (double)this->data->max11616[this->data->nasos_temp_alarm_bit[1]] * 0.9;
    dui->pushButton_29->setText(QString("%1").arg(this->data->nasos_wet_alarm_border[1]));
}
void DialogCalibr::on_pushButton_30_clicked(){
    this->data->nasos_wet_alarm_border[2] = (double)this->data->max11616[this->data->nasos_temp_alarm_bit[2]] * 0.9;
    dui->pushButton_30->setText(QString("%1").arg(this->data->nasos_wet_alarm_border[2]));
}
void DialogCalibr::on_pushButton_28_clicked(){
    this->data->nasos_wet_alarm_border[3] = (double)this->data->max11616[this->data->nasos_temp_alarm_bit[3]] * 0.9;
    dui->pushButton_28->setText(QString("%1").arg(this->data->nasos_wet_alarm_border[3]));
}
