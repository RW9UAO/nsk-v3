#include <QGraphicsScene>
#include <QGraphicsItem>
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
#include <QTextStream>
#include <sys/mman.h>
#include <termios.h>
#include "mainwindow.h"
#include "thread_math.h"

bool rezerv_need_to_work;
int last_pump = 0;

//=========================================================================================================================================================
Thread_math::Thread_math(MainWindow *wnd) : ui(wnd->ui), wnd(wnd){
}
//=========================================================================================================================================================
void Thread_math::save_pump_ttw(){
    QFile config_file("workinfo.txt");// имя нового конфигурационного файла
    QTextStream config_out(&config_file);// создадим поток для чтения
    QString config_line;

    config_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Unbuffered);
    for(int i=0;i<4;i++){
        config_out << QString("pump%1ttw %2\n").arg(i+1).arg(wnd->data.nasos_TTW[i]);
    }
    config_file.close();
}
//=========================================================================================================================================================
int Thread_math::getkolvo(){
    //поищем включенный насос
    int kolvo = 0;
    for(int i = 0; i < 4; i++){
        if(wnd->data.nasos[i] == 2) kolvo++;
    }
    return kolvo;
}
//=========================================================================================================================================================
//поиск насоса с минимальной наработкой, готового насоса (без алармов), вне резерва
int Thread_math::getminTTW(){
    int minval = 0xFFFFFF, minindex = -1;
    if(rezerv_need_to_work == true){//если нужно крутануть резервный насос
        for(int i=0;i<4;i++){
            if(wnd->data.nasos_rez[i]){//поищем резевный насос
                rezerv_need_to_work = false;
                return i;
            }
        }
    }
    //сначала выкинем все резевные
    for(int i = 0; i < 4; i++){
        if( wnd->data.nasos_rez[i] == false){ //вне резерва
            //qDebug()<<QString("%1 not rezerv").arg(i+1);
            //выкинем все невключенные
            if( wnd->data.nasos[i] == 1){ //в состоянии готовности
                //qDebug()<<QString("%1 ready").arg(i+1);
                if( minval > wnd->data.nasos_TTW[i]){
                    //qDebug()<<QString("%1 > %2").arg(minval).arg(i+1);
                    minval = wnd->data.nasos_TTW[i];
                    minindex = i;
                }
            }
        }
    }
    return minindex;
}
//=========================================================================================================================================================
//должен вызываться 1 раз в 1 секунду
double Thread_math::PID(){
    double error, dTerm;
    double currentPos, G_Dt= 1 / 1000;// 1 second to mSec
    double result = 0;

            //targetPos get from config file
            //Pparam, Dparam, Iparam get from config file
            currentPos = wnd->data.level_to_show_sm;
            //IntegratedError накапливается со временем
    //проверим входной паказатель на предмет ошибки
            if(wnd->data.max11616[wnd->data.level_input_number] >= 0 && wnd->data.max11616[wnd->data.level_input_number] <= 4096){
                error = wnd->data.targetPos - currentPos;
                wnd->data.IntegratedError += error * G_Dt;
                //constrain IntegratedError whith +/-window
                dTerm = wnd->data.Dparam * (currentPos - wnd->data.LastPosition);
                wnd->data.LastPosition = currentPos;
                result = (wnd->data.Pparam * error) + (wnd->data.Iparam * wnd->data.IntegratedError) + dTerm;
            }else{
    //ошибочный, останавливаем мотор
                qDebug() << QString("Thread_math::ERROR reading max11616[10] %1").arg(wnd->data.max11616[wnd->data.level_input_number]);
                //wnd->data.motor_need_to_stop = true;
                //wnd->data.time_to_stop = TIME_TO_STOP_PUMP;
            }
    return result;
}
//=========================================================================================================================================================
int Thread_math::constrain(int input_freq){
    //обрежем границы, нижняя 5 гц, верхняя 50 гц
    if(input_freq < 50) input_freq = 50;
    if(input_freq > 500) input_freq = 500;
    return input_freq;
}
//=========================================================================================================================================================
//включим выбранный насос
void Thread_math::start_one_more_pump(int i){
    if(i > 3)return;// мало-ли что прилетит на вход
// включим пускатель
    wnd->data.pca9555_output1W |= (1<< wnd->data.nasos_bit[i]);
    //дадим команду частотнику на запуск
    if(wnd->data.isATV12){//частотник
        for(int c=0;c<4;c++){
            if(wnd->data.nasos[c] == 2){//насос работает
                wnd->data.freq_w[c] = 500;//предыдущие насосы врубим на 50 Гц
            }
        }
        wnd->data.freq_w[i] = 250;//последнему зададим начальную частоту
        wnd->data.stop[i] = false; wnd->data.start[i]= true;
        qDebug() << QString("start_one_more_pump: start %1").arg(i+1);
    }
    last_pump = i;//фамилия последнего запущенного насоса
}
//=========================================================================================================================================================
void Thread_math::run() {
    int seconds_counter0=0,seconds_counter1=0;
    int time_to_stop_overlevel_swimmer_pump=0;

    rezerv_need_to_work = false;

    //подождем пока I2C поток запустится и даст данные, а пока покажем процент "загрузки"
    for(int i=1;i<10;i++){
        wnd->data.dialogparam = i * 1000;
        emit changeDataMain();//отрисуем MainWindow
        QThread::msleep(1000);
    }
    wnd->data.dialogparam = 0;
    // вот тут вся математика в основном цикле
    while(wnd->done){
//=========================================================================================================================================================
 //проверим аварии с 35 мс
        //а если насос есть, но частотник выключен пускателем? надо подумать. или привода постоянно включены?
      for(int i = 0; i < 4; i++){
        if(wnd->data.nasos_bit[i] == -1 || wnd->data.ATV12status[i] == OFFLINE)//частотник не отвечает
            wnd->data.nasos[i] = 0;//нет насоса
        else{
            wnd->data.nasos[i] = 1;//не включен, готов
            //если есть частотник, сделаем вывод о включенности по команде "start"
            if(wnd->data.isATV12){
                if(wnd->data.ATV12status[i] == RUN)
                    wnd->data.nasos[i] = 2;//включен, работает
            }else{
            //если нет частотника, сделаем вывод о включенности по пускателю
                if(wnd->data.pca9555_output1R & (1<< wnd->data.nasos_bit[i]) ){
                    wnd->data.nasos[i] = 2;//включен, работает
                }
            }
            if(wnd->data.max11616[wnd->data.nasos_current_alarm_bit[i]] > wnd->data.nasos_current_alarm_border[i]){
                wnd->data.nasos[i] = 3;//авария по току
            }
            if(wnd->data.max11616[wnd->data.nasos_temp_alarm_bit[i]] > wnd->data.nasos_temp_alarm_border[i]){
                wnd->data.nasos[i] = 4;//авария по температуре
            }
            if(wnd->data.max11616[wnd->data.nasos_temp_alarm_bit[i]] > wnd->data.nasos_wet_alarm_border[i]){
                wnd->data.nasos[i] = 5;//авария по влажности
            }
            if(wnd->data.max11616[wnd->data.nasos_temp_alarm_bit[i]] > 4000 || wnd->data.max11616[wnd->data.nasos_temp_alarm_bit[i]] < 1000){
                wnd->data.nasos[i] = 6;//авария по обрыву кабеля
            }
         }
      }
        //обработаем ошибки и выключим моторы
      for(int i=0;i<4;i++){
          if(wnd->data.nasos[i] == 0 || wnd->data.nasos[i] > 2){
                wnd->data.pca9555_output1W &= ~(1<<wnd->data.nasos_bit[i]);
                wnd->data.nasos_alarm_at_time[i] = QDateTime::currentDateTime().toString("dd.MM.yy/hh:mm:ss");
                qDebug()<<QString("thread_math: alarm %1 at pump %2").arg(wnd->data.nasos[i]).arg(i+1);
                if(wnd->data.isATV12){
                  wnd->data.stop[i]=true;
                  wnd->data.start[i]=false;
              }
            }
      }
//=========================================================================================================================================================
        for(int i=0;i<4;i++){
            if(wnd->data.nasos[i] == 2){
                wnd->data.nasos_TTW[i]++;//время работы плюсанем
                if(wnd->data.isATV12){
                    //зададим частоту последнему включенному насосу с помощью ПИД
                    constrain(wnd->data.freq_w[last_pump] + PID());//его надо звать раз в секунду
                }
                wnd->data.nasos_time_to_overtime[i]++;
                if(wnd->data.nasos_time_to_overtime[i] > 900){//насос работает слишком долго
                    //сбросим время непрерывной работы насоса
                    wnd->data.nasos_time_to_overtime[i] = 0;
                    int i = getminTTW();
                    if(i != -1){//может случиться так, что нет свободного насоса
                        //включим новый насос
                        start_one_more_pump(i);
                    }
                }
            }
        }
//=========================================================================================================================================================
        //обработка уровней
       if(wnd->data.islevel_meter){
        //датчик уровня
         if(wnd->data.max11616[wnd->data.level_input_number] == -1 ||                                              //АЦП не отвечает
            wnd->data.max11616[wnd->data.level_input_number] >= 3500 ||                                         //датчик в КЗ
            wnd->data.max11616[wnd->data.level_input_number] <  ((double)(wnd->data.level_empty_raw * 0.9)) ){//датчик отвалился
                wnd->data.level_to_show_sm = -1;
             //проверим аварийный поплавок
               if( (wnd->data.pca9555_input0 & (1<<wnd->data.level_4_bit)) == 0 ){
                 //сработал датчик перелива
                wnd->data.level_to_show = 0;
                //старт моторам. х.з. сколько штук стартовать? 1? и потом через заданное время второй по nasos_time_to_overtime?
                if(getkolvo() == 0){
                    start_one_more_pump(getminTTW());
                    time_to_stop_overlevel_swimmer_pump = wnd->data.overlevel_time_to_stop;//считать, время работы насосов после отпускания поплавка перелива
                }
                //квакнем про отвал датчика уровня
                if((wnd->data.error_flags & ERROR_LEVEL_METER)==0){
                    wnd->data.error_flags |= ERROR_LEVEL_METER;
                    qCritical()<< QString("level meter alarm %1").arg(wnd->data.max11616[wnd->data.level_input_number]);
                 }
               }else{
                wnd->data.level_to_show = 1;//уровнемер отвалился, перелива пока нет
                //если моторы включены, начать отсчет времени для останова
                if(getkolvo()){
                    time_to_stop_overlevel_swimmer_pump--;
                    if(time_to_stop_overlevel_swimmer_pump == 0){
                        //стоп моторам
                        if(wnd->data.time_to_stop == false){
                            wnd->data.motor_need_to_stop = true;
                            wnd->data.time_to_stop = TIME_TO_STOP_PUMP;
                        }
                    }
                }
               }
         }else{
             //вычислим уровень в сантиметрах
             //прирост уровня от начала
             double a = (double)wnd->data.max11616[wnd->data.level_input_number] - (double)wnd->data.level_empty_raw;
             if(a < 0) a = 0;//если уровень упал ниже "пустого бака"
             //полная шкала показаний АЦП
             double b = (double)wnd->data.level_full_raw - (double)wnd->data.level_empty_raw;
             double c = a / b;
             double d = c * (double)wnd->data.level_full_sm;
             wnd->data.level_to_show_sm = d;
           if(wnd->data.level_to_show_sm < wnd->data.level_1_sm){
               wnd->data.level_to_show = 2;
           }
           if(wnd->data.level_to_show_sm >= wnd->data.level_1_sm){
               wnd->data.level_to_show = 3;
           }
           if(wnd->data.level_to_show_sm >= wnd->data.level_2_sm){
               wnd->data.level_to_show = 4;
           }
           if(wnd->data.level_to_show_sm >= wnd->data.level_3_sm){
               wnd->data.level_to_show = 5;
           }
           if(wnd->data.level_to_show_sm >= wnd->data.level_4_sm){
               wnd->data.level_to_show = 6;
           }
         }
       }else{
        //поплавки
/* нахуй не нужно
           //проверим аварийный поплавок
           if(wnd->data.pca9555_input0 == -1 || (wnd->data.pca9555_input0 & (1<<wnd->data.level_4_bit)) == 0 ){
             //если не читается дискретный вход или сработал датчик перелива
            wnd->data.level_to_show = 0;
            //стоп моторам
            if(wnd->data.time_to_stop == false){
                wnd->data.motor_need_to_stop = true;
                wnd->data.time_to_stop = TIME_TO_STOP_PUMP;
            }
            qCritical("overlevel swimmer alarm");
           }else{*/
               //пойдем без изысков, последовательно. сначала обработаем корректные ситуации
               wnd->data.level_to_show = 102;//не сработал ни один поплавок
               if((wnd->data.pca9555_input0 & (1<<wnd->data.level_1_bit)) == 0 ){
                    wnd->data.level_to_show = 103;
               }
               if((wnd->data.pca9555_input0 & (1<<wnd->data.level_1_bit)) == 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_2_bit)) == 0 ){
                    wnd->data.level_to_show = 104;
               }
               if((wnd->data.pca9555_input0 & (1<<wnd->data.level_1_bit)) == 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_2_bit)) == 0 &&
                       (wnd->data.pca9555_input0 & (1<<wnd->data.level_3_bit)) == 0 ){
                    wnd->data.level_to_show = 105;
               }
               if((wnd->data.pca9555_input0 & (1<<wnd->data.level_1_bit)) == 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_2_bit)) == 0 &&
                       (wnd->data.pca9555_input0 & (1<<wnd->data.level_3_bit)) == 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_4_bit)) == 0 ){
                    wnd->data.level_to_show = 106;
               }
               //потом обработаем ошибки поплавков
               if((wnd->data.pca9555_input0 & (1<<wnd->data.level_1_bit)) == 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_2_bit)) == 0 &&
                       (wnd->data.pca9555_input0 & (1<<wnd->data.level_3_bit)) != 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_4_bit)) == 0 ){
                    wnd->data.level_to_show = 10;
               }
               if((wnd->data.pca9555_input0 & (1<<wnd->data.level_1_bit)) == 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_2_bit)) != 0 &&
                       (wnd->data.pca9555_input0 & (1<<wnd->data.level_3_bit)) == 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_4_bit)) == 0 ){
                    wnd->data.level_to_show = 11;
               }
               if((wnd->data.pca9555_input0 & (1<<wnd->data.level_1_bit)) == 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_2_bit)) != 0 &&
                       (wnd->data.pca9555_input0 & (1<<wnd->data.level_3_bit)) == 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_4_bit)) != 0 ){
                    wnd->data.level_to_show = 12;
               }
               if((wnd->data.pca9555_input0 & (1<<wnd->data.level_1_bit)) == 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_2_bit)) != 0 &&
                       (wnd->data.pca9555_input0 & (1<<wnd->data.level_3_bit)) != 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_4_bit)) == 0 ){
                    wnd->data.level_to_show = 13;
               }
               if((wnd->data.pca9555_input0 & (1<<wnd->data.level_1_bit)) != 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_2_bit)) == 0 &&
                       (wnd->data.pca9555_input0 & (1<<wnd->data.level_3_bit)) == 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_4_bit)) == 0 ){
                    wnd->data.level_to_show = 14;
               }
               if((wnd->data.pca9555_input0 & (1<<wnd->data.level_1_bit)) != 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_2_bit)) == 0 &&
                       (wnd->data.pca9555_input0 & (1<<wnd->data.level_3_bit)) == 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_4_bit)) != 0 ){
                    wnd->data.level_to_show = 15;
               }
               if((wnd->data.pca9555_input0 & (1<<wnd->data.level_1_bit)) != 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_2_bit)) == 0 &&
                       (wnd->data.pca9555_input0 & (1<<wnd->data.level_3_bit)) != 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_4_bit)) == 0 ){
                    wnd->data.level_to_show = 16;
               }
               if((wnd->data.pca9555_input0 & (1<<wnd->data.level_1_bit)) != 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_2_bit)) == 0 &&
                       (wnd->data.pca9555_input0 & (1<<wnd->data.level_3_bit)) != 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_4_bit)) != 0 ){
                    wnd->data.level_to_show = 17;
               }
               if((wnd->data.pca9555_input0 & (1<<wnd->data.level_1_bit)) != 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_2_bit)) != 0 &&
                       (wnd->data.pca9555_input0 & (1<<wnd->data.level_3_bit)) == 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_4_bit)) == 0 ){
                    wnd->data.level_to_show = 18;
               }
               if((wnd->data.pca9555_input0 & (1<<wnd->data.level_1_bit)) != 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_2_bit)) != 0 &&
                       (wnd->data.pca9555_input0 & (1<<wnd->data.level_3_bit)) == 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_4_bit)) != 0 ){
                    wnd->data.level_to_show = 19;
               }
               if((wnd->data.pca9555_input0 & (1<<wnd->data.level_1_bit)) != 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_2_bit)) != 0 &&
                       (wnd->data.pca9555_input0 & (1<<wnd->data.level_3_bit)) != 0 && (wnd->data.pca9555_input0 & (1<<wnd->data.level_4_bit)) == 0 ){
                    wnd->data.level_to_show = 20;
               }
//есть нюанс. если какой-то поплавок дурит, а сработает поплавок перелива, то насосы не включатся.
               if(wnd->data.level_to_show >= 10 && wnd->data.level_to_show <= 20){
                   //стоп моторам
                   if(wnd->data.time_to_stop == false){
                    wnd->data.motor_need_to_stop = true;
                    wnd->data.time_to_stop = TIME_TO_STOP_PUMP;
                   }
                   qCritical("alarm on unknown level swimmer");
               }
           //}
         }
//=========================================================================================================================================================
//возможно эту процедуру стоит утащить в самый низ, чтобы после нее никто не пытался дрoчить насосы
    //возникла ошибка поплавков, либо слив закончен, остановим моторы
        if(wnd->data.motor_need_to_stop == true){
            if(wnd->data.time_to_stop == -1 || //время плавной остановки в конфиге не задано. тупо тормозим
               wnd->data.time_to_stop == 0 ){//пришло время остановки
                if(wnd->data.isATV12){//частотникам пошлем команду
                    //надо по очереди отключать насосы
                        for(int i = 0; i < 4; i++){
                            if( wnd->data.nasos[i] == 2){//включен, работает
                                wnd->data.stop[i]=true;
                                wnd->data.start[i]=false;
                                qDebug() << QString("motor_need_to_stop: ATV12 STOP %1").arg(i+1);
                                wnd->data.nasos_time_to_overtime[i] = 0;//обнулим время непрерывной работы насоса
                                i = 100500;// х.з. как тут break сработает или нет
                            }
                        }
                }else{//ппуски просто отключаем пускателями
                    for(int i = 0; i < 4; i++){
                        if( wnd->data.nasos[i] == 2){//включен, работает
                            qDebug() << QString("motor STOP %1").arg(i+1);
                            wnd->data.pca9555_output1W &= ~(1<<wnd->data.nasos_bit[i]);
                            i = 100500;
                        }
                    }
                }
//убедиться, что после этого никто не пытается включать насосы
                //wnd->data.motor_need_to_stop = false;
            }else{
                wnd->data.time_to_stop--;
                //здесь надо приводам частоту снижать
                qDebug() << QString("time to stop = %1").arg(wnd->data.time_to_stop);
                for(int i=0;i<4;i++){
                    if(wnd->data.freq_w[i]>50){//если частота привода больше 5 Гц
                    //if(wnd->data.freq_w[i]>25){//если частота привода больше 2.5 Гц
                        wnd->data.freq_w[i] -= 10;//уменьшим на 1 Гц
                    }
                }
            }
        }
        if(wnd->data.nasos[0] != 2 && wnd->data.nasos[1] != 2 &&
                wnd->data.nasos[2] != 2 && wnd->data.nasos[3] != 2 &&
                wnd->data.time_to_stop == 0){
            wnd->data.motor_need_to_stop = false;
        }
//=========================================================================================================================================================
        if(wnd->data.servicemode){              //сервисный режим включен
            if(wnd->data.servicemodeTTL){       //время еще не вышло
                wnd->data.servicemodeTTL--;     //отнимем секунду
            }else{
                wnd->data.servicemode = false;  //отключим сервисный режим
                qDebug("Service mode timeout. Disable.");
            }
        }

//=========================================================================================================================================================
//  логику управления насосами вынес отдельно
        if(wnd->data.islevel_meter){
//уровнемер
            //если уровень жидкости на нулевом уровне - выключаем все включенные насосы.
            if(wnd->data.level_to_show_sm < wnd->data.level_1_sm && // уровень ниже 1-го порога
                    wnd->data.level_to_show == 2){                  // и мы это показываем
                if(getkolvo()){//есть насосы включенные?
                    if(wnd->data.motor_need_to_stop == false){
                        wnd->data.motor_need_to_stop = true;                // остановим моторы
                        wnd->data.time_to_stop = TIME_TO_STOP_PUMP;
                        qDebug("Level < level_1_sm, STOP motor");
                    }
                }
            }
            //если уровень выше первого, но ниже второго - ничего не делаем
            if(wnd->data.level_to_show_sm >= wnd->data.level_1_sm &&
                    wnd->data.level_to_show_sm < wnd->data.level_2_sm &&
                    wnd->data.level_to_show == 3){
            }
            //если уровень выше второго, но ниже третьего - включим один насос
            if(wnd->data.level_to_show_sm >= wnd->data.level_2_sm &&
                    wnd->data.level_to_show_sm < wnd->data.level_3_sm &&
                    wnd->data.level_to_show == 4){
                //если включенных насосов нет
                if(getkolvo() < 1){
                    int i = getminTTW();
                    if(i != -1){
                        //включим найденный насос
                        start_one_more_pump(i);
                        qDebug()<<QString("Level > level_2_sm, START motor %1").arg(i+1);
                    }else qCritical("need to start first pump, but has no free. i`m cryng");
                }
            }
            //если уровень выше третьего, но ниже четвертого - включить два насоса
            if(wnd->data.level_to_show_sm >= wnd->data.level_3_sm &&
                    wnd->data.level_to_show_sm < wnd->data.level_4_sm &&
                    wnd->data.level_to_show == 5){
                //если включенных насосов нет
                if(getkolvo() < 2){
                    int i = getminTTW();
                    if(i != -1){
                        //включим найденный насос
                        start_one_more_pump(i);
                        qDebug()<<QString("Level > level_3_sm, START motor %1").arg(i+1);
                    }else qCritical("need to start second pump, but has no free.");
                }
            }
            //перелив, включим три насоса
            if( wnd->data.level_to_show_sm > wnd->data.level_4_sm ){
                if(getkolvo() < 3){
                    int i = getminTTW();
                    if(i != -1){
                        start_one_more_pump(i);//включим найденный насос
                        qDebug()<<QString("Level > level_4_sm, START motor %1").arg(i+1);
                    }else qCritical("need to start 3 pump, but has no free.");
                }
            }
        }else{
//поплавки
            //если уровень жидкости на нулевом уровне - выключаем все включенные насосы.
            if( wnd->data.level_to_show == 102){
                qDebug("Level < level_1_bit, STOP motor");
                if(wnd->data.motor_need_to_stop == false){
                    wnd->data.motor_need_to_stop = true;                // остановим моторы
                    wnd->data.time_to_stop = TIME_TO_STOP_PUMP;
                 }
            }
            //если уровень выше первого, но ниже второго - ничего не делаем
            if( wnd->data.level_to_show == 103){
            }
            if( wnd->data.level_to_show == 104){
                //если включенных насосов нет
                if(getkolvo() < 1){
                    int i = getminTTW();
                    if(i != -1){
                        start_one_more_pump(i);                        //включим найденный насос
                    }else qCritical("swimmer, need to start first pump, but has no free. i`m cryng");
                }
            }
            if( wnd->data.level_to_show == 105){
                if(getkolvo() < 2){
                    int i = getminTTW();
                    if(i != -1){
                        start_one_more_pump(i);                        //включим найденный насос
                    }else qCritical("swimmer, need to start 2 pump, but has no free. i`m cryng");
                }
            }
            if( wnd->data.level_to_show == 106){
                if(getkolvo() < 3){
                    int i = getminTTW();
                    if(i != -1){
                        start_one_more_pump(i);                        //включим найденный насос
                    }else qCritical("swimmer, need to start 3 pump, but has no free. i`m cryng");
                }
            }

        }

//=========================================================================================================================================================
//сохраним "моточасы"
        seconds_counter0++;
        if(seconds_counter0 >= (60 * 10) ){//60 sec * 10 min
            save_pump_ttw();
            seconds_counter0 = 0;
            qDebug("writing pump times to disk.");
        }
        seconds_counter1++;
        if(seconds_counter1 >= (60 * 60)){//раз в час почистим флаги ошибок
            seconds_counter1 = 0;
            wnd->data.error_flags = 0;
        }
//раз в сутки резервному насосу надо крутануть
        QString ttemp = QDateTime::currentDateTime().toString("hh:mm");
        if( ttemp == "23:45" ){
                rezerv_need_to_work = true;
        }
//=========================================================================================================================================================
        // MainWindow в любом случае активно, повесим обработчики на него
        emit changeDataMain();
        if(wnd->dlgNasos1->isVisible()){
            emit changeDataNasos1();
        }
        //if( wnd->dlgCalibr->isVisible() ){
        //    emit changeDataCalibr();
        //}

        //ждем 1 секунду (на самом деле чуть больше)
        QThread::msleep(1000);

    }//end while(done)
}
//--------------------------------------------------------------------------------------
void Thread_math::quit(){
    wnd->done = false;
}


