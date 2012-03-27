#include "dialogATV12.h"
#include "ui_dialogATV12.h"
#include "data_struct.h"
#include "mainwindow.h"

DialogATV12::DialogATV12(data_struct * d, QWidget *parent) : QDialog(parent), dui(new Ui::DialogATV12){
    dui->setupUi(this);

    this->data = d;
}

DialogATV12::~DialogATV12(){
    delete dui;
}
void DialogATV12::MyEventHandler(){
    this->close();
}
// обновим содержимое диалога
void DialogATV12::updateDataATV12(){
    int i;
    QLabel *p[6] = {dui->LFT1, dui->LFT2, dui->LFT3, dui->LFT4, dui->LFT5, dui->LFT6};
    QLabel *p1[6] = {dui->freq1, dui->freq2, dui->freq3, dui->freq4, dui->freq5, dui->freq6};
    QLabel *p2[6] = {dui->curr1, dui->curr2, dui->curr3, dui->curr4, dui->curr5, dui->curr6};
    QLabel *p3[4] = {dui->stat1, dui->stat2, dui->stat3, dui->stat4};

 for(i = 0; i < 4; i++){
  if(this->data->lft[i] != -1){
    switch(this->data->lft[i]){// LFT - ATV12 has fault
    case 1: p[i]->setText("INF");break;
    case 3: p[i]->setText("CFF");break;
    case 4: p[i]->setText("CFI");break;
    case 5: p[i]->setText("SLF1");break;
    case 9: p[i]->setText("OCF");break;
    case 10: p[i]->setText("CRF1");break;
    case 16: p[i]->setText("OHF");break;
    case 17: p[i]->setText("OLF");break;
    case 18: p[i]->setText("OBF");break;
    case 19: p[i]->setText("OSF");break;
    case 20: p[i]->setText("OPF");break;
    case 21: p[i]->setText("PHF");break;
    case 22: p[i]->setText("USF");break;
    case 23: p[i]->setText("SCF1");break;
    case 24: p[i]->setText("SOF");break;
    case 25: p[i]->setText("TNF");break;
    case 26: p[i]->setText("INF1");break;
    case 27: p[i]->setText("INF2");break;
    case 28: p[i]->setText("INF3");break;
    case 29: p[i]->setText("INF4");break;
    case 32: p[i]->setText("SCF3");break;
    case 33: p[i]->setText("OPF2");break;
    case 42: p[i]->setText("SLF2");break;
    case 45: p[i]->setText("SLF3");break;
    case 51: p[i]->setText("INF9");break;
    case 53: p[i]->setText("INFB");break;
    case 54: p[i]->setText("TJF");break;
    case 55: p[i]->setText("SCF4");break;
    case 56: p[i]->setText("SCF5");break;
    case 69: p[i]->setText("INFE");break;
    case 77: p[i]->setText("CFI2");break;
    case 100: p[i]->setText("ULF");break;
    case 101: p[i]->setText("OLC");break;
    case 106: p[i]->setText("LFF1");break;
    default: p[i]->setText("LFT OK");
    }
  }else{
      p[i]->setText("--");
  }//end if( != -1)
//покажем частоты
  if(this->data->freq[i] != -1){
      QString s = QString("%1").arg((double)this->data->freq[i]/10);
      p1[i]->setText(s);
  }else{
      p1[i]->setText("--");
  }
  //покажем токи
    if(this->data->current[i] != -1){
        QString s = QString("%1").arg(this->data->current[i]);
        p2[i]->setText(s);
    }else{
        p2[i]->setText("--");
    }
    //покажем статус привода
    if(this->data->ATV12status[i] == RUN){
        p3[i]->setText("run");
    }else
        if(this->data->ATV12status[i] == STOP){
            p3[i]->setText("stop");
        }else
            if(this->data->ATV12status[i] == ERROR){
                p3[i]->setText("ERROR");
            }else
                if(this->data->ATV12status[i] == OFFLINE){
                    p3[i]->setText("OFF");
                }else{
                    p3[i]->setText(QString("0x%1").arg(this->data->ATV12status[i],0, 16));
            }
    }//end for(i)
}

void DialogATV12::on_start1_clicked(){
    this->data->start[0] = true;
    this->data->stop[0] = false;
}
void DialogATV12::on_start2_clicked(){
    this->data->start[1] = true;
    this->data->stop[1] = false;
}
void DialogATV12::on_start3_clicked(){
    this->data->start[2] = true;
    this->data->stop[2] = false;
}
void DialogATV12::on_start4_clicked(){
    this->data->start[3] = true;
    this->data->stop[3] = false;
}

void DialogATV12::on_stop1_clicked(){
    this->data->start[0] = false;
    this->data->stop[0] = true;
}
void DialogATV12::on_stop2_clicked(){
    this->data->start[1] = false;
    this->data->stop[1] = true;
}
void DialogATV12::on_stop3_clicked(){
    this->data->start[2] = false;
    this->data->stop[2] = true;
}
void DialogATV12::on_stop4_clicked(){
    this->data->start[3] = false;
    this->data->stop[3] = true;
}
