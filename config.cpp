#include <QtDebug>
#include <qtextcodec.h>
#include <errno.h>
#include <sys/time.h>
//#include <sys/ioctl.h>
#include <fcntl.h>          //is defined open and close
#include <unistd.h>         //is defined read and write
#include <sys/types.h>
#include <signal.h>
#include <QFile>
#include <stdint.h>
#include <QTextStream>
#include <sys/mman.h>
#include <termios.h>
#include "mainwindow.h"
#include "thread_485.h"
#include "ui_mainwindow.h"

//void Thread_485::set_defaults(void){
void MainWindow::set_defaults(void){
    this->data.servicemode = false;
    //this->data.servicemode = true;
    this->data.error_flags = 0;
    this->data.KNSnumber = "123";
    this->data.isATV12 = true;        //по умолчанию включим 4-ре частотника и счетчик
    this->data.isSoftStart = false;
    this->data.ATV12maxNum = 4;
    this->data.isCE303 = true;
    this->data.is21 = true;
    this->data.is35 = true;
    this->data.islevel_meter = true;
    for(int i = 0; i < 6; i++){
            this->data.current[i] = -1;
            this->data.freq[i] = -1;
            this->data.freq_w[i] = 0;
            this->data.lft[i] = -1;
            this->data.eta[i] = -1;
            this->data.start[i] = false;
            this->data.stop[i] = false;
    }
    for(int i=0;i<4;i++){
        this->data.nasos[i] = 0;
        this->data.nasos_TTW[i] = 0;
    }
    this->data.time_ = "-";
    this->data.date_ = "-";
    this->data.fccur = -1;
    this->data.condi = -1;
    this->data.powep = this->data.poweq = -1;
    this->data.curra = this->data.currb = this->data.currc = -1;
    this->data.cos_f = -1;
    this->data.angleAB = this->data.angleBC = this->data.angleCA = -1;
    this->data.volta = this->data.voltb = this->data.voltb = -1;
    this->data.et0pe = -1;
    this->data.CE303needupdate = update_none;
    this->data.pca9555_input0 = -1;
    this->data.pca9555_output1R = -1;
    this->data.pca9555_output1W = 0;//all OFF
    this->data.pca9555ADDR = 0;
    for(int i = 0; i < 12; i++){this->data.max11616[i] = -1;}
    this->data.max11616ADDR = 0;
    this->data.targetPos = 0; this->data.Pparam = 1; this->data.Dparam = 1; this->data.Iparam = 1;
    this->data.nasos1_current_alarm_bit = -1;
    this->data.nasos2_current_alarm_bit = -1;
    this->data.nasos3_current_alarm_bit = -1;
    this->data.nasos4_current_alarm_bit = -1;
    this->data.nasos1_temp_alarm_bit = -1;
    this->data.nasos2_temp_alarm_bit = -1;
    this->data.nasos3_temp_alarm_bit = -1;
    this->data.nasos4_temp_alarm_bit = -1;
    this->data.nasos1_wet_alarm_bit = -1;
    this->data.nasos2_wet_alarm_bit = -1;
    this->data.nasos3_wet_alarm_bit = -1;
    this->data.nasos4_wet_alarm_bit = -1;
    this->data.nasos1_bit = this->data.nasos2_bit = -1;
    this->data.nasos3_bit = this->data.nasos4_bit = -1;
    this->data.nasos1_current_alarm_border=this->data.nasos2_current_alarm_border=this->data.nasos3_current_alarm_border=this->data.nasos4_current_alarm_border=-1;
    this->data.nasos1_temp_alarm_border=this->data.nasos2_temp_alarm_border=this->data.nasos3_temp_alarm_border=this->data.nasos4_temp_alarm_border=-1;
    this->data.nasos1_wet_alarm_border=this->data.nasos2_wet_alarm_border=this->data.nasos3_wet_alarm_border=this->data.nasos4_wet_alarm_border=-1;
    this->data.level_input_number=this->data.level_empty_raw=this->data.level_full_raw=this->data.level_full_sm=-1;
    this->data.level_1_bit=this->data.level_2_bit=this->data.level_3_bit=this->data.level_4_bit=-1;
    this->data.level_1_sm=this->data.level_2_sm=this->data.level_3_sm=this->data.level_4_sm=this->data.overlevel_time_to_stop=-1;
    this->data.motor_need_to_stop = false;
    this->data.time_to_stop = -1;
    this->data.level_to_show = 0;
    this->data.level_to_show_sm = -1;
}
//========================================================================================================================
double get_double_from_config(QString config_line){
    bool ok;    double temp;
    //удалим все символы перед пробелом
    config_line.remove(0, config_line.indexOf(" ",0) + 1);
    //преобразуем
    temp = config_line.toDouble(&ok);
    if(ok){         return temp;    }
    return -1;
}
//========================================================================================================================
//  config file parser
//void Thread_485::read_config(void){
void MainWindow::read_config(void){
    QFile config_file("config.txt");// имя конфигурационного файла
    QTextStream config_in(&config_file);// создадим поток для чтения
    QString config_line;

if( config_file.open(QIODevice::ReadOnly) ){ // откроем файл для чтения
do {
      // берем очередную строку из файла
      config_line = config_in.readLine();
      ///////////////////////////////////////

      if( config_line.contains("max11616ADDR") ){
        this->data.max11616ADDR = get_double_from_config(config_line);
      }
      if( config_line.contains("PCA9555ADDR") ){
        this->data.pca9555ADDR = get_double_from_config(config_line);
      }
      if (config_line.contains("level_meter") ){
          if (config_line.contains("yes") ){
                  this->data.islevel_meter = true;
                  qDebug("I2C level meter include");
          }else{
                  this->data.islevel_meter = false;
          }
      }
      if (config_line.contains("level_input_number") ){
          this->data.level_input_number = get_double_from_config(config_line);
      }
      if (config_line.contains("level_empty_raw") ){
               this->data.level_empty_raw = get_double_from_config(config_line);
      }
      if (config_line.contains("level_full_raw") ){
               this->data.level_full_raw = get_double_from_config(config_line);
      }
      if (config_line.contains("level_full_sm") ){
               this->data.level_full_sm = get_double_from_config(config_line);
      }
      if (config_line.contains("level_1_bit") ){
               this->data.level_1_bit = get_double_from_config(config_line);
      }
      if (config_line.contains("level_2_bit") ){
               this->data.level_2_bit = get_double_from_config(config_line);
      }
      if (config_line.contains("level_3_bit") ){
               this->data.level_3_bit = get_double_from_config(config_line);
      }
      if (config_line.contains("level_4_bit") ){
               this->data.level_4_bit = get_double_from_config(config_line);
      }
      if (config_line.contains("level_1_sm") ){
               this->data.level_1_sm = get_double_from_config(config_line);
      }
      if (config_line.contains("level_2_sm") ){
               this->data.level_2_sm = get_double_from_config(config_line);
      }
      if (config_line.contains("level_3_sm") ){
               this->data.level_3_sm = get_double_from_config(config_line);
      }
      if (config_line.contains("level_4_sm") ){
               this->data.level_4_sm = get_double_from_config(config_line);
      }
      if (config_line.contains("overlevel_time_to_stop") ){
               this->data.level_1_bit = get_double_from_config(config_line);
      }
      if (config_line.contains("nasos1_bit") ){
               this->data.nasos1_bit = get_double_from_config(config_line);
      }
      if (config_line.contains("nasos2_bit") ){
               this->data.nasos2_bit = get_double_from_config(config_line);
      }
      if (config_line.contains("nasos3_bit") ){
               this->data.nasos3_bit = get_double_from_config(config_line);
      }
      if (config_line.contains("nasos4_bit") ){
               this->data.nasos4_bit = get_double_from_config(config_line);
      }
      if (config_line.contains("nasos1_current_alarm_border") ){
              this->data.nasos1_current_alarm_border = get_double_from_config(config_line);
      }
      if (config_line.contains("nasos2_current_alarm_border") ){
               this->data.nasos2_current_alarm_border = get_double_from_config(config_line);
      }
      if (config_line.contains("nasos3_current_alarm_border") ){
               this->data.nasos3_current_alarm_border = get_double_from_config(config_line);
      }
      if (config_line.contains("nasos4_current_alarm_border") ){
               this->data.nasos4_current_alarm_border = get_double_from_config(config_line);
      }
      if (config_line.contains("nasos1_temp_alarm_border") ){
               this->data.nasos1_temp_alarm_border = get_double_from_config(config_line);
      }
      if (config_line.contains("nasos2_temp_alarm_border") ){
               this->data.nasos2_temp_alarm_border = get_double_from_config(config_line);
      }
      if (config_line.contains("nasos3_temp_alarm_border") ){
               this->data.nasos3_temp_alarm_border = get_double_from_config(config_line);
      }
      if (config_line.contains("nasos4_temp_alarm_border") ){
               this->data.nasos4_temp_alarm_border = get_double_from_config(config_line);
      }
      if (config_line.contains("nasos1_wet_alarm_border") ){
               this->data.nasos1_wet_alarm_border = get_double_from_config(config_line);
      }
      if (config_line.contains("nasos2_wet_alarm_border") ){
               this->data.nasos2_wet_alarm_border = get_double_from_config(config_line);
      }
      if (config_line.contains("nasos3_wet_alarm_border") ){
               this->data.nasos3_wet_alarm_border = get_double_from_config(config_line);
      }
      if (config_line.contains("nasos4_wet_alarm_border") ){
          this->data.nasos4_wet_alarm_border = get_double_from_config(config_line);
      }
    if (config_line.contains("nasos1_current_alarm_bit") ){
             this->data.nasos1_current_alarm_bit = get_double_from_config(config_line);
    }
    if (config_line.contains("nasos2_current_alarm_bit") ){
             this->data.nasos2_current_alarm_bit = get_double_from_config(config_line);
    }
    if (config_line.contains("nasos3_current_alarm_bit") ){
             this->data.nasos3_current_alarm_bit = get_double_from_config(config_line);
    }
    if (config_line.contains("nasos4_current_alarm_bit") ){
             this->data.nasos4_current_alarm_bit = get_double_from_config(config_line);
    }
    if (config_line.contains("nasos1_temp_alarm_bit") ){
             this->data.nasos1_temp_alarm_bit = get_double_from_config(config_line);
    }
    if (config_line.contains("nasos2_temp_alarm_bit") ){
             this->data.nasos2_temp_alarm_bit = get_double_from_config(config_line);
    }
    if (config_line.contains("nasos3_temp_alarm_bit") ){
             this->data.nasos3_temp_alarm_bit = get_double_from_config(config_line);
    }
    if (config_line.contains("nasos4_temp_alarm_bit") ){
             this->data.nasos4_temp_alarm_bit = get_double_from_config(config_line);
    }
    if (config_line.contains("nasos1_wet_alarm_bit") ){
             this->data.nasos1_wet_alarm_bit = get_double_from_config(config_line);
    }
    if (config_line.contains("nasos2_wet_alarm_bit") ){
             this->data.nasos2_wet_alarm_bit = get_double_from_config(config_line);
    }
    if (config_line.contains("nasos3_wet_alarm_bit") ){
             this->data.nasos3_wet_alarm_bit = get_double_from_config(config_line);
    }
    if (config_line.contains("nasos4_wet_alarm_bit") ){
             this->data.nasos4_wet_alarm_bit = get_double_from_config(config_line);
    }
      if (config_line.contains("PIDtarget") ){
              this->data.targetPos = get_double_from_config(config_line);
              qDebug() << QString("PIDtarget %1").arg(this->data.targetPos);
      }
      if (config_line.contains("PID_P") ){
              this->data.Pparam = get_double_from_config(config_line);
              qDebug() << QString("Pparam %1").arg(this->data.Pparam);
      }
      if (config_line.contains("PID_D") ){
              this->data.Dparam = get_double_from_config(config_line);
              qDebug() << QString("Dparam %1").arg(this->data.Dparam);
      }
      if (config_line.contains("PID_I") ){
              this->data.Iparam = get_double_from_config(config_line);
              qDebug() << QString("Iparam %1").arg(this->data.Iparam);
      }
      if (config_line.contains("PCA9555") ){   // сравним с нужным параметром
          if (config_line.contains("yes") ){ // включено?
              this->data.is21 = true;
              qDebug("PCA9555 include");
          }else{
              this->data.is21 = false;   // иначе выключено
          }
      }
      if (config_line.contains("MAX11616") ){   // сравним с нужным параметром
          if (config_line.contains("yes") ){ // включено?
              this->data.is35 = true;
              qDebug("MAX11616 include");
          }else{
              this->data.is35 = false;   // иначе выключено
          }
      }
    if (config_line.contains("ATV12") ){   // сравним с нужным параметром
        if (config_line.contains("yes") ){ // включено?
            this->data.isATV12 = true;
            qDebug("ATV12 include");
        }else{
            this->data.isATV12 = false;   // иначе выключено
        }
    }
    if (config_line.contains("CE303") ){
        if (config_line.contains("yes") ){
            this->data.isCE303 = true;
            qDebug("CE303 include");
        }else{
            this->data.isCE303 = false;
         }
    }
    if (config_line.contains("ATS12") ){
        if (config_line.contains("yes") ){
                this->data.isSoftStart = true;
                qDebug("SoftStart include");
        }else{
                this->data.isSoftStart = false;
        }
    }
    if (config_line.contains("MAXDEV") ){
            this->data.ATV12maxNum = get_double_from_config(config_line);
            QString s = QString("ATV12 has maximum %1 addr").arg(this->data.ATV12maxNum);
            qDebug() << s;
    }
  } while (!config_line.isNull());             // последней строки достигли в файле
config_file.close();                           // закроем конфигурационный файл

qDebug() << QString("borders:\n%1 %2 %3 %4\n%5 %6 %7 %8\n%9 %10 %11 %12")
.arg(this->data.nasos1_current_alarm_border).arg(this->data.nasos2_current_alarm_border).arg(this->data.nasos3_current_alarm_border).arg(this->data.nasos4_current_alarm_border)
.arg(this->data.nasos1_temp_alarm_border).arg(this->data.nasos2_temp_alarm_border).arg(this->data.nasos3_temp_alarm_border).arg(this->data.nasos4_temp_alarm_border)
.arg(this->data.nasos1_wet_alarm_border).arg(this->data.nasos2_wet_alarm_border).arg(this->data.nasos3_wet_alarm_border).arg(this->data.nasos4_wet_alarm_border);

//qDebug() << QString("levels:\n%1 %2 %3 %4\n%5 %6 %7 %8\n%9 %10 %11 %12") it`s too looong

}else{
  QMessageBox msgBox;
  msgBox.setText("Error open file\r\nconfig.txt");
  msgBox.exec();
qDebug("error open config.txt");
}
}
