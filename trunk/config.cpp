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


void MainWindow::set_defaults(void){

    //cоздадим массив с адресами имен переменных
    int_parameters["max11616ADDR"] = &this->data.max11616ADDR;
    int_parameters["PCA9555ADDR"] = &this->data.pca9555ADDR;
    int_parameters["MAXDEV"] = &this->data.ATV12maxNum;
    int_parameters["nasos1_bit"] = &this->data.nasos1_bit;    int_parameters["nasos2_bit"] = &this->data.nasos2_bit;
    int_parameters["nasos3_bit"] = &this->data.nasos3_bit;    int_parameters["nasos4_bit"] = &this->data.nasos4_bit;
    int_parameters["nasos1_current_alarm_bit"] = &this->data.nasos1_current_alarm_bit;    int_parameters["nasos1_current_alarm_border"] = &this->data.nasos1_current_alarm_border;
    int_parameters["nasos2_current_alarm_bit"] = &this->data.nasos2_current_alarm_bit;    int_parameters["nasos2_current_alarm_border"] = &this->data.nasos2_current_alarm_border;
    int_parameters["nasos3_current_alarm_bit"] = &this->data.nasos3_current_alarm_bit;    int_parameters["nasos3_current_alarm_border"] = &this->data.nasos3_current_alarm_border;
    int_parameters["nasos4_current_alarm_bit"] = &this->data.nasos4_current_alarm_bit;    int_parameters["nasos4_current_alarm_border"] = &this->data.nasos4_current_alarm_border;
    int_parameters["nasos1_temp_alarm_bit"] = &this->data.nasos1_temp_alarm_bit;
    int_parameters["nasos1_temp_alarm_border"] = &this->data.nasos1_temp_alarm_border;    int_parameters["nasos1_wet_alarm_border"] = &this->data.nasos1_wet_alarm_border;
    int_parameters["nasos2_temp_alarm_bit"] = &this->data.nasos2_temp_alarm_bit;
    int_parameters["nasos2_temp_alarm_border"] = &this->data.nasos2_temp_alarm_border;    int_parameters["nasos2_wet_alarm_border"] = &this->data.nasos2_wet_alarm_border;
    int_parameters["nasos3_temp_alarm_bit"] = &this->data.nasos3_temp_alarm_bit;
    int_parameters["nasos3_temp_alarm_border"] = &this->data.nasos3_temp_alarm_border;    int_parameters["nasos3_wet_alarm_border"] = &this->data.nasos3_wet_alarm_border;
    int_parameters["nasos4_temp_alarm_bit"] = &this->data.nasos4_temp_alarm_bit;
    int_parameters["nasos4_temp_alarm_border"] = &this->data.nasos4_temp_alarm_border;    int_parameters["nasos4_wet_alarm_border"] = &this->data.nasos4_wet_alarm_border;
    int_parameters["level_input_number"] = &this->data.level_input_number;
    int_parameters["level_empty_raw"] = &this->data.level_empty_raw;    int_parameters["level_full_raw"] = &this->data.level_full_raw;
    int_parameters["level_full_sm"] = &this->data.level_full_sm;
    int_parameters["level_1_bit"] = &this->data.level_1_bit;    int_parameters["level_2_bit"] = &this->data.level_2_bit;
    int_parameters["level_3_bit"] = &this->data.level_3_bit;    int_parameters["level_4_bit"] = &this->data.level_4_bit;
    int_parameters["level_1_sm"] = &this->data.level_1_sm;    int_parameters["level_2_sm"] = &this->data.level_2_sm;
    int_parameters["level_3_sm"] = &this->data.level_3_sm;    int_parameters["level_4_sm"] = &this->data.level_4_sm;
    int_parameters["overlevel_time_to_stop"] = &this->data.overlevel_time_to_stop;
    int_parameters["nasos_param_index0"] = &this->data.nasos_param_index[0];    int_parameters["nasos_param_index1"] = &this->data.nasos_param_index[1];
    int_parameters["nasos_param_index2"] = &this->data.nasos_param_index[2];    int_parameters["nasos_param_index3"] = &this->data.nasos_param_index[3];
    //int_parameters[""] = &this->data.;

    double_parameters["PIDtarget"] = &this->data.targetPos;    double_parameters["PID_P"] = &this->data.Pparam;
    double_parameters["PID_I"] = &this->data.Iparam;    double_parameters["PID_D"] = &this->data.Dparam;

    this->data.servicemode = false;
    //this->data.servicemode = true;
    this->data.IPboardAddr = "10.10.3.120";
    this->data.need_to_set_date_time_fromCE303 = true;
    this->data.need_to_save_config = false;
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
    this->data.IntegratedError = this->data.LastPosition = 0;
    this->data.nasos1_current_alarm_bit = -1;
    this->data.nasos2_current_alarm_bit = -1;
    this->data.nasos3_current_alarm_bit = -1;
    this->data.nasos4_current_alarm_bit = -1;
    this->data.nasos1_temp_alarm_bit = -1;
    this->data.nasos2_temp_alarm_bit = -1;
    this->data.nasos3_temp_alarm_bit = -1;
    this->data.nasos4_temp_alarm_bit = -1;
    this->data.nasos1_bit = this->data.nasos2_bit = -1;
    this->data.nasos3_bit = this->data.nasos4_bit = -1;
    this->data.nasos1_current_alarm_border=this->data.nasos2_current_alarm_border=this->data.nasos3_current_alarm_border=this->data.nasos4_current_alarm_border=-1;
    this->data.nasos1_temp_alarm_border=this->data.nasos2_temp_alarm_border=this->data.nasos3_temp_alarm_border=this->data.nasos4_temp_alarm_border=-1;
    this->data.level_input_number=this->data.level_empty_raw=this->data.level_full_raw=this->data.level_full_sm=-1;
    this->data.level_1_bit=this->data.level_2_bit=this->data.level_3_bit=this->data.level_4_bit=-1;
    this->data.level_1_sm=this->data.level_2_sm=this->data.level_3_sm=this->data.level_4_sm=this->data.overlevel_time_to_stop=-1;
    this->data.motor_need_to_stop = false;
    this->data.time_to_stop = -1;
    this->data.level_to_show = 0;
    this->data.level_to_show_sm = -1;
    this->data.nasos_param_index[0] = 0;
    this->data.nasos_param_index[1] = 0;
    this->data.nasos_param_index[2] = 0;
    this->data.nasos_param_index[3] = 0;
//получим список файлов с артикулами из папки ./bd
    QFile config_file;// имя конфигурационного файла
    QTextStream config_in;
    QStringList files = QDir("./bd").entryList(QStringList() << "*.art", QDir::Files); // получаем список файлов в текущей папке
    for(int i=0;i<files.size();i++){ // перебираем все эти файлы
        if(i == 11) break;//если файлов больше 11, то они к нам не влезут.
        //qDebug() << QString("%1").arg(files[i]);
        qDebug() << files[i];
        config_file.setFileName("./bd/"+files[i]);
        config_in.setDevice(&config_file);// создадим поток для чтения
        config_file.open(QIODevice::ReadOnly);// откроем файл артикула
        //и построчно считаем его
        this->data.nasos_param[i].name = config_in.readLine();
        this->data.nasos_param[i].articul = config_in.readLine();
        this->data.nasos_param[i].pressure = config_in.readLine().toInt();
        this->data.nasos_param[i].volume = config_in.readLine().toInt();
        this->data.nasos_param[i].current = config_in.readLine().toInt();
        this->data.nasos_param[i].rpm = config_in.readLine().toInt();
        this->data.nasos_param[i].cable = config_in.readLine();
        this->data.nasos_param[i].starttype = config_in.readLine();
        this->data.nasos_param[i].pictname = config_in.readLine() + ".gif";
        config_file.close();
    }
    //this->data.nasos1_on = false;this->data.nasos2_on = false;
    //this->data.nasos3_on = false;this->data.nasos4_on = false;
    this->data.nasos_rez[0] = false;    this->data.nasos_rez[1] = false;
    this->data.nasos_rez[3] = false;    this->data.nasos_rez[2] = false;

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
    //QMapIterator<QString, double *> i(parameters);
    QMutableMapIterator<QString, int *> int_i(int_parameters);
    QMutableMapIterator<QString, double *> double_i(double_parameters);

    //проверить есть ли old_config.txt, возможно предыдущий раз завершился аварией

if( config_file.open(QIODevice::ReadOnly) ){ // откроем файл для чтения
do {
      // берем очередную строку из файла
      config_line = config_in.readLine();
      ///////////////////////////////////////
      int_i.toFront();//перемещаемся в начало списка
      while(int_i.hasNext()){//пока можно взять следующий
          int_i.next();
          if( config_line.contains(int_i.key()) ){//поищем в строке слово из списка
              *int_i.value() = get_double_from_config(config_line);//положим переменную
          }
      }
      double_i.toFront();//перемещаемся в начало списка
      while(double_i.hasNext()){//пока можно взять следующий
          double_i.next();
          if( config_line.contains(double_i.key()) ){//поищем в строке слово из списка
              *double_i.value() = get_double_from_config(config_line);//положим переменную
          }
      }
      if (config_line.contains("KNSnumber") ){
        config_line.remove(0, config_line.indexOf(" ",0) + 1);
        this->data.KNSnumber = config_line;
      }
      if (config_line.contains("IPboardAddr") ){
        config_line.remove(0, config_line.indexOf(" ",0) + 1);
        this->data.IPboardAddr = config_line;
      }
      if (config_line.contains("level_meter") ){
          if (config_line.contains("yes") ){
                  this->data.islevel_meter = true;
                  qDebug("I2C level meter include");
          }else{
                  this->data.islevel_meter = false;
          }
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
//=============================================================================================
//  config file saver
void MainWindow::save_config(void){
    QFile file("config.txt");
    file.rename("config.txt", "old_config.txt");
    QFile old_config_file("old_config.txt");// имя старого конфигурационного файла
    QTextStream config_in(&old_config_file);// создадим поток для чтения
    QFile config_file("config.txt");// имя нового конфигурационного файла
    QTextStream config_out(&config_file);// создадим поток для чтения
    QString config_line;
    //QMapIterator<QString, double *> i(parameters);
    QMutableMapIterator<QString, int *> int_i(int_parameters);
    QMutableMapIterator<QString, double *> double_i(double_parameters);

    config_file.open(QIODevice::WriteOnly);
if( old_config_file.open(QIODevice::ReadOnly) ){
do {
        config_line = config_in.readLine();// берем очередную строку из файла
//проверим все int из списка
        int_i.toFront();
        while(int_i.hasNext()){
            int_i.next();
            if( config_line.contains(int_i.key()) ){
                config_line = QString("%1 %2").arg(int_i.key()).arg(*int_i.value());
            }
        }
        double_i.toFront();
        while(double_i.hasNext()){
            double_i.next();
            if( config_line.contains(double_i.key()) ){
                config_line = QString("%1 %2").arg(double_i.key()).arg(*double_i.value(),0,'f',3);
            }
        }
//bool прийдется тут руками растолкать
        if (config_line.contains("KNSnumber") ){
          config_line = QString("KNSnumber %1").arg(this->data.KNSnumber);
        }
        if (config_line.contains("IPboardAddr") ){
          config_line = QString("IPboardAddr %1").arg(this->data.IPboardAddr);
        }
        config_out << config_line << "\n";
    } while (!config_line.isNull());             // последней строки достигли в файле
  config_file.close();                           // закроем конфигурационный файл
  old_config_file.close();                           // закроем конфигурационный файл
  old_config_file.remove();
}else{
  QMessageBox msgBox;
  msgBox.setText("Error open file\r\nconfig.txt");
  msgBox.exec();
qDebug("error open config.txt");
}
}
