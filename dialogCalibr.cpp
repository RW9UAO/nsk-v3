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
}

DialogCalibr::~DialogCalibr(){
    delete dui;
}
// обновим содержимое диалога
void DialogCalibr::updateDataCalibr(){
    dui->i2c_raw->setText(QString("%1").arg(this->data->max11616[this->data->level_input_number]));
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
