#include "dialogI2C.h"
#include "ui_dialogI2C.h"
#include "data_struct.h"
#include "mainwindow.h"

#define ActiveColor "background-color: rgb(70, 200, 70);"
#define NonActiveColor "background-color: rgb(220, 220, 220);"
#define ADCRef 4096
#define R0 510

DialogI2C::DialogI2C(data_struct * d, QWidget *parent) : QDialog(parent), dui(new Ui::DialogI2C){
    dui->setupUi(this);

    this->data = d;
}

DialogI2C::~DialogI2C(){
    delete dui;
}
void DialogI2C::MyEventHandler(){
    this->close();
}
// обновим содержимое диалога
void DialogI2C::updateDataI2C(){

    //this->data->pca9555_input0 = 0x55;
    //this->data->pca9555_output1R = 0xAA;

    //this->data->max11616[0] = 0;
    //this->data->max11616[1] = 4095;

    if(this->data->max11616[0] != -1){
//покажем напряжения с входов
        dui->U1->setText(QString("%1").arg(this->data->max11616[0]));
        dui->U2->setText(QString("%1").arg(this->data->max11616[1]));
        dui->U3->setText(QString("%1").arg(this->data->max11616[2]));
        dui->U4->setText(QString("%1").arg(this->data->max11616[3]));
        dui->U5->setText(QString("%1").arg(this->data->max11616[4]));
        dui->U6->setText(QString("%1").arg(this->data->max11616[5]));
        dui->U7->setText(QString("%1").arg(this->data->max11616[6]));
        dui->U8->setText(QString("%1").arg(this->data->max11616[7]));
        dui->U9->setText(QString("%1").arg(this->data->max11616[8]));
        dui->U10->setText(QString("%1").arg(this->data->max11616[9]));
        dui->U11->setText(QString("%1").arg(this->data->max11616[10]));
        dui->U12->setText(QString("%1").arg(this->data->max11616[11]));
//покажем отмасштабированные значения
        dui->I1->setText(QString("%1").arg((double)this->data->max11616[0]/R0, 0, 'f', 2));
        dui->I2->setText(QString("%1").arg((float)this->data->max11616[1]/R0, 0, 'f', 2));
        dui->I3->setText(QString("%1").arg((float)this->data->max11616[2]/R0, 0, 'f', 2));
        dui->I4->setText(QString("%1").arg((float)this->data->max11616[3]/R0, 0, 'f', 2));
        dui->I5->setText(QString("%1").arg((float)this->data->max11616[4]/R0, 0, 'f', 2));
        dui->I6->setText(QString("%1").arg((float)this->data->max11616[5]/R0, 0, 'f', 2));
        dui->I7->setText(QString("%1").arg((float)this->data->max11616[6]/R0, 0, 'f', 2));
        dui->I8->setText(QString("%1").arg((float)this->data->max11616[7]/R0, 0, 'f', 2));
        dui->I9->setText(QString("%1").arg((float)this->data->max11616[8]/R0, 0, 'f', 2));
        dui->I10->setText(QString("%1").arg((float)this->data->max11616[9]/R0, 0, 'f', 2));
        dui->I11->setText(QString("%1").arg((float)this->data->max11616[10]/R0, 0, 'f', 2));
        dui->I12->setText(QString("%1").arg((float)this->data->max11616[11]/R0, 0, 'f', 2));
    }else{
        dui->U1->setText("--");
        dui->U2->setText("--");
        dui->U3->setText("--");
        dui->U4->setText("--");
        dui->U5->setText("--");
        dui->U6->setText("--");
        dui->U7->setText("--");
        dui->U8->setText("--");
        dui->U9->setText("--");
        dui->U10->setText("--");
        dui->U11->setText("--");
        dui->U12->setText("--");
    }

    if(this->data->pca9555_input0 != -1){
//нарисуем галочки на включенных входах
        if(this->data->pca9555_input0 & 0x01){
            dui->radioButton1->setChecked(true);
        }else{dui->radioButton1->setChecked(false);}
        if(this->data->pca9555_input0 & 0x02){
            dui->radioButton2->setChecked(true);
        }else{dui->radioButton2->setChecked(false);}
        if(this->data->pca9555_input0 & 0x04){
            dui->radioButton3->setChecked(true);
        }else{dui->radioButton3->setChecked(false);}
        if(this->data->pca9555_input0 & 0x08){
            dui->radioButton4->setChecked(true);
        }else{dui->radioButton4->setChecked(false);}
        if(this->data->pca9555_input0 & 0x10){
            dui->radioButton5->setChecked(true);
        }else{dui->radioButton5->setChecked(false);}
        if(this->data->pca9555_input0 & 0x20){
            dui->radioButton6->setChecked(true);
        }else{dui->radioButton6->setChecked(false);}
        if(this->data->pca9555_input0 & 0x40){
            dui->radioButton7->setChecked(true);
        }else{dui->radioButton7->setChecked(false);}
        if(this->data->pca9555_input0 & 0x80){
            dui->radioButton8->setChecked(true);
        }else{dui->radioButton8->setChecked(false);}
//активируем выхода
        dui->radioButton1_2->setEnabled(true);        dui->radioButton2_2->setEnabled(true);
        dui->radioButton3_2->setEnabled(true);        dui->radioButton4_2->setEnabled(true);
        dui->radioButton5_2->setEnabled(true);        dui->radioButton6_2->setEnabled(true);
        dui->radioButton7_2->setEnabled(true);        dui->radioButton8_2->setEnabled(true);
//подсветим включенные выхода
        if(this->data->pca9555_output1R & 0x01){
            dui->radioButton1_2->setStyleSheet(ActiveColor);
        }else{dui->radioButton1_2->setStyleSheet(NonActiveColor);}
        if(this->data->pca9555_output1R & 0x02){
            dui->radioButton2_2->setStyleSheet(ActiveColor);
        }else{dui->radioButton2_2->setStyleSheet(NonActiveColor);}
        if(this->data->pca9555_output1R & 0x04){
            dui->radioButton3_2->setStyleSheet(ActiveColor);
        }else{dui->radioButton3_2->setStyleSheet(NonActiveColor);}
        if(this->data->pca9555_output1R & 0x08){
            dui->radioButton4_2->setStyleSheet(ActiveColor);
        }else{dui->radioButton4_2->setStyleSheet(NonActiveColor);}
        if(this->data->pca9555_output1R & 0x10){
            dui->radioButton5_2->setStyleSheet(ActiveColor);
        }else{dui->radioButton5_2->setStyleSheet(NonActiveColor);}
        if(this->data->pca9555_output1R & 0x20){
            dui->radioButton6_2->setStyleSheet(ActiveColor);
        }else{dui->radioButton6_2->setStyleSheet(NonActiveColor);}
        if(this->data->pca9555_output1R & 0x40){
            dui->radioButton7_2->setStyleSheet(ActiveColor);
        }else{dui->radioButton7_2->setStyleSheet(NonActiveColor);}
        if(this->data->pca9555_output1R & 0x80){
            dui->radioButton8_2->setStyleSheet(ActiveColor);
        }else{dui->radioButton8_2->setStyleSheet(NonActiveColor);}
//отдадим в программу выхода для включения/выключения
        if(dui->radioButton1_2->isChecked())this->data->pca9555_output1W |= 0x01;else this->data->pca9555_output1W &= ~0x01;
        if(dui->radioButton2_2->isChecked())this->data->pca9555_output1W |= 0x02;else this->data->pca9555_output1W &= ~0x02;
        if(dui->radioButton3_2->isChecked())this->data->pca9555_output1W |= 0x04;else this->data->pca9555_output1W &= ~0x04;
        if(dui->radioButton4_2->isChecked())this->data->pca9555_output1W |= 0x08;else this->data->pca9555_output1W &= ~0x08;
        if(dui->radioButton5_2->isChecked())this->data->pca9555_output1W |= 0x10;else this->data->pca9555_output1W &= ~0x10;
        if(dui->radioButton6_2->isChecked())this->data->pca9555_output1W |= 0x20;else this->data->pca9555_output1W &= ~0x20;
        if(dui->radioButton7_2->isChecked())this->data->pca9555_output1W |= 0x40;else this->data->pca9555_output1W &= ~0x40;
        if(dui->radioButton8_2->isChecked())this->data->pca9555_output1W |= 0x80;else this->data->pca9555_output1W &= ~0x80;

    }else{
        dui->radioButton1_2->setDisabled(true);        dui->radioButton2_2->setDisabled(true);
        dui->radioButton3_2->setDisabled(true);        dui->radioButton4_2->setDisabled(true);
        dui->radioButton5_2->setDisabled(true);        dui->radioButton6_2->setDisabled(true);
        dui->radioButton7_2->setDisabled(true);        dui->radioButton8_2->setDisabled(true);
    }
}
