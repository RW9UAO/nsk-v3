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
    this->data.KNSnumber = "123";
    this->isATV12 = true;        //по умолчанию включим 4-ре частотника и счетчик
    this->isSoftStart = false;
    this->ATV12maxNum = 4;
    this->isCE303 = true;
    this->is21 = true;
    this->is35 = true;
    this->islevel_meter = true;
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
    for(int i = 0; i < 12; i++){this->data.max11616[i] = -1;}
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
//  config file parser
//void Thread_485::read_config(void){
void MainWindow::read_config(void){
    QFile config_file("config.txt");// имя конфигурационного файла
//    QTextStream out(&file);
    QTextStream config_in(&config_file);// создадим поток для чтения
    QString config_line;

if( config_file.open(QIODevice::ReadOnly) ){ // откроем файл для чтения
do {
      // берем очередную строку из файла
      config_line = config_in.readLine();
      ///////////////////////////////////////

      if (config_line.contains("level_meter") ){
          if (config_line.contains("yes") ){
                  this->islevel_meter = true;
                  qDebug("I2C level meter include");
          }else{
                  this->islevel_meter = false;
          }
      }
      if (config_line.contains("level_input_number") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.level_input_number = temp;
          }
      }
      if (config_line.contains("level_empty_raw") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.level_empty_raw = temp;
          }
      }
      if (config_line.contains("level_full_raw") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.level_full_raw = temp;
          }
      }
      if (config_line.contains("level_full_sm") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.level_full_sm = temp;
          }
      }
      if (config_line.contains("level_1_bit") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.level_1_bit = temp;
          }
      }
      if (config_line.contains("level_2_bit") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.level_2_bit = temp;
          }
      }
      if (config_line.contains("level_3_bit") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.level_3_bit = temp;
          }
      }
      if (config_line.contains("level_4_bit") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.level_4_bit = temp;
          }
      }
      if (config_line.contains("level_1_sm") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.level_1_sm = temp;
          }
      }
      if (config_line.contains("level_2_sm") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.level_2_sm = temp;
          }
      }
      if (config_line.contains("level_3_sm") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.level_3_sm = temp;
          }
      }
      if (config_line.contains("level_4_sm") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.level_4_sm = temp;
          }
      }
      if (config_line.contains("overlevel_time_to_stop") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.level_1_bit = temp;
          }
      }
      if (config_line.contains("nasos1_bit") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.nasos1_bit = temp;
          }
      }
      if (config_line.contains("nasos2_bit") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.nasos2_bit = temp;
          }
      }
      if (config_line.contains("nasos3_bit") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.nasos3_bit = temp;
          }
      }
      if (config_line.contains("nasos4_bit") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.nasos4_bit = temp;
          }
      }

      if (config_line.contains("nasos1_current_alarm_border") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.nasos1_current_alarm_border = temp;
          }
      }
      if (config_line.contains("nasos2_current_alarm_border") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.nasos2_current_alarm_border = temp;
          }
      }
      if (config_line.contains("nasos3_current_alarm_border") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.nasos3_current_alarm_border = temp;
          }
      }
      if (config_line.contains("nasos4_current_alarm_border") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.nasos4_current_alarm_border = temp;
          }
      }

      if (config_line.contains("nasos1_temp_alarm_border") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.nasos1_temp_alarm_border = temp;
          }
      }
      if (config_line.contains("nasos2_temp_alarm_border") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.nasos2_temp_alarm_border = temp;
          }
      }
      if (config_line.contains("nasos3_temp_alarm_border") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.nasos3_temp_alarm_border = temp;
          }
      }
      if (config_line.contains("nasos4_temp_alarm_border") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.nasos4_temp_alarm_border = temp;
          }
      }
      if (config_line.contains("nasos1_wet_alarm_border") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.nasos1_wet_alarm_border = temp;
          }
      }
      if (config_line.contains("nasos2_wet_alarm_border") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.nasos2_wet_alarm_border = temp;
          }
      }
      if (config_line.contains("nasos3_wet_alarm_border") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.nasos3_wet_alarm_border = temp;
          }
      }
      if (config_line.contains("nasos4_wet_alarm_border") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
               this->data.nasos4_wet_alarm_border = temp;
          }
      }

    if (config_line.contains("nasos1_current_alarm_bit") ){
        bool ok;
        double temp;
        config_line.remove(0, config_line.indexOf(" ",0) + 1);
        temp = config_line.toDouble(&ok);
        if(ok){
             this->data.nasos1_current_alarm_bit = temp;
        }
    }
    if (config_line.contains("nasos2_current_alarm_bit") ){
        bool ok;
        double temp;
        config_line.remove(0, config_line.indexOf(" ",0) + 1);
        temp = config_line.toDouble(&ok);
        if(ok){
             this->data.nasos2_current_alarm_bit = temp;
        }
    }
    if (config_line.contains("nasos3_current_alarm_bit") ){
        bool ok;
        double temp;
        config_line.remove(0, config_line.indexOf(" ",0) + 1);
        temp = config_line.toDouble(&ok);
        if(ok){
             this->data.nasos3_current_alarm_bit = temp;
        }
    }
    if (config_line.contains("nasos4_current_alarm_bit") ){
        bool ok;
        double temp;
        config_line.remove(0, config_line.indexOf(" ",0) + 1);
        temp = config_line.toDouble(&ok);
        if(ok){
             this->data.nasos4_current_alarm_bit = temp;
        }
    }
    if (config_line.contains("nasos1_temp_alarm_bit") ){
        bool ok;
        double temp;
        config_line.remove(0, config_line.indexOf(" ",0) + 1);
        temp = config_line.toDouble(&ok);
        if(ok){
             this->data.nasos1_temp_alarm_bit = temp;
        }
    }
    if (config_line.contains("nasos2_temp_alarm_bit") ){
        bool ok;
        double temp;
        config_line.remove(0, config_line.indexOf(" ",0) + 1);
        temp = config_line.toDouble(&ok);
        if(ok){
             this->data.nasos2_temp_alarm_bit = temp;
        }
    }
    if (config_line.contains("nasos3_temp_alarm_bit") ){
        bool ok;
        double temp;
        config_line.remove(0, config_line.indexOf(" ",0) + 1);
        temp = config_line.toDouble(&ok);
        if(ok){
             this->data.nasos3_temp_alarm_bit = temp;
        }
    }
    if (config_line.contains("nasos4_temp_alarm_bit") ){
        bool ok;
        double temp;
        config_line.remove(0, config_line.indexOf(" ",0) + 1);
        temp = config_line.toDouble(&ok);
        if(ok){
             this->data.nasos4_temp_alarm_bit = temp;
        }
    }
    if (config_line.contains("nasos1_wet_alarm_bit") ){
        bool ok;
        double temp;
        config_line.remove(0, config_line.indexOf(" ",0) + 1);
        temp = config_line.toDouble(&ok);
        if(ok){
             this->data.nasos1_wet_alarm_bit = temp;
        }
    }
    if (config_line.contains("nasos2_wet_alarm_bit") ){
        bool ok;
        double temp;
        config_line.remove(0, config_line.indexOf(" ",0) + 1);
        temp = config_line.toDouble(&ok);
        if(ok){
             this->data.nasos2_wet_alarm_bit = temp;
        }
    }
    if (config_line.contains("nasos3_wet_alarm_bit") ){
        bool ok;
        double temp;
        config_line.remove(0, config_line.indexOf(" ",0) + 1);
        temp = config_line.toDouble(&ok);
        if(ok){
             this->data.nasos3_wet_alarm_bit = temp;
        }
    }
    if (config_line.contains("nasos4_wet_alarm_bit") ){
        bool ok;
        double temp;
        config_line.remove(0, config_line.indexOf(" ",0) + 1);
        temp = config_line.toDouble(&ok);
        if(ok){
             this->data.nasos4_wet_alarm_bit = temp;
        }
    }
      if (config_line.contains("PIDtarget") ){
          bool ok;
          double temp;
          config_line.remove(0, config_line.indexOf(" ",0) + 1);
          temp = config_line.toDouble(&ok);
          if(ok){
              this->data.targetPos = temp;
              qDebug() << QString("PIDtarget %1").arg(this->data.targetPos);
          }
      }
      if (config_line.contains("PID_P") ){
          bool ok;
          double temp;
          config_line.remove(0, 6);
          temp = config_line.toDouble(&ok);
          if(ok){
              this->data.Pparam = temp;
              qDebug() << QString("Pparam %1").arg(this->data.Pparam);
          }
      }
      if (config_line.contains("PID_D") ){
          bool ok;
          double temp;
          config_line.remove(0, 6);
          temp = config_line.toDouble(&ok);
          if(ok){
              this->data.Dparam = temp;
              qDebug() << QString("Dparam %1").arg(this->data.Dparam);
          }
      }
      if (config_line.contains("PID_I") ){
          bool ok;
          double temp;
          config_line.remove(0, 6);
          temp = config_line.toDouble(&ok);
          if(ok){
              this->data.Iparam = temp;
              qDebug() << QString("Iparam %1").arg(this->data.Iparam);
          }
      }
      if (config_line.contains("PCA9555") ){   // сравним с нужным параметром
          if (config_line.contains("yes") ){ // включено?
              this->is21 = true;
              qDebug("PCA9555 include");
          }else{
              this->is21 = false;   // иначе выключено
          }
      }
      if (config_line.contains("MAX11616") ){   // сравним с нужным параметром
          if (config_line.contains("yes") ){ // включено?
              this->is35 = true;
              qDebug("MAX11616 include");
          }else{
              this->is35 = false;   // иначе выключено
          }
      }
    if (config_line.contains("ATV12") ){   // сравним с нужным параметром
        if (config_line.contains("yes") ){ // включено?
            this->isATV12 = true;
            qDebug("ATV12 include");
        }else{
            this->isATV12 = false;   // иначе выключено
        }
    }
    if (config_line.contains("CE303") ){
        if (config_line.contains("yes") ){
            this->isCE303 = true;
            qDebug("CE303 include");
        }else{
            this->isCE303 = false;
         }
    }
    if (config_line.contains("ATS12") ){
        if (config_line.contains("yes") ){
                this->isSoftStart = true;
                qDebug("SoftStart include");
        }else{
                this->isSoftStart = false;
        }
    }
    if (config_line.contains("MAXDEV") ){
        bool ok;
        int temp;
        config_line.remove(0, 7);              // уберем лишние символы из строки включая пробел
        temp = config_line.toInt(&ok, 10);     // преобразуем в integer
        if(ok){
            this->ATV12maxNum = temp;    // если без ошибок, установим
            QString s = QString("ATV12 has maximum %1 addr").arg(this->ATV12maxNum);
            qDebug() << s;
        }
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
