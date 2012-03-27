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

//--------------------------------------------------------------------------------------
Thread_math::Thread_math(MainWindow *wnd) : ui(wnd->ui), wnd(wnd){
}
//--------------------------------------------------------------------------------------
void Thread_math::run() {
//    double error, dTerm;
//    double currentPos, LastPosition = 0, IntegratedError = 0, G_Dt;
//    double result = 0;

    qDebug() << "Thread_math::run";
//    qDebug() << QString("PID using: target %1, P %2 I %3 D %4").arg().arg();
//подождем пока I2C поток запустится и даст данные
QThread::msleep(5000);

    // вот тут вся математика в основном цикле
    while(wnd->done){
//fprintf(stderr, "!");//плюнем в консоль

 //====================================================================
//PID section
/*
        G_Dt = 1 / 1000;// 1 second
        //targetPos get from config file
        //Pparam, Dparam, Iparam get from config file
        currentPos = wnd->data.max11616[10];
        //IntegratedError накапливается со временем
//проверим входной паказатель на предмет ошибки
        if(currentPos >= 0 && currentPos <= 4096){
            error = wnd->data.targetPos - currentPos;
            IntegratedError += error * G_Dt;
            //constrain IntegratedError whith +/-window
            //if(IntegratedError > )..... etc
            dTerm = wnd->data.Dparam * (currentPos - LastPosition);
            LastPosition = currentPos;
            result = (wnd->data.Pparam * error) + (wnd->data.Iparam * IntegratedError) + dTerm;

            qDebug() << QString("PID: currentPos %1, result %2, freq %3").arg(currentPos).arg(result).arg((double)wnd->data.freq_w[0]/10);
            //если была нажата кнопка старт (мотор запущен), то установим частоту
            if(wnd->data.start[0] == true){
                wnd->data.freq_w[0] += result;
                //обрежем границы, нижняя 5 гц, верхняя 50 гц
                if(wnd->data.freq_w[0] < 50) wnd->data.freq_w[0] = 50;
                if(wnd->data.freq_w[0] > 500) wnd->data.freq_w[0] = 500;
                }else{
                    wnd->data.freq_w[0] = 200;//иначе 20 гц
                }
        }else{
//ошибочный, останавливаем мотор
            qDebug() << QString("Thread_math::ERROR reading max11616[10] %1").arg(currentPos);
            wnd->data.stop[0] = true;
            wnd->data.start[0] = false;
        }
*/
//=======================================================================
 //проверим аварии с 35 мс
        if(wnd->data.nasos1_bit == -1)wnd->data.nasos[0] = 0;//нет насоса
        else{
            wnd->data.nasos[0] = 1;//не включен, готов
            //если есть частотник, сделаем вывод о включенности по команде "start"
            if(wnd->isATV12){
                if(wnd->data.ATV12status[0] == RUN)
                    wnd->data.nasos[0] = 2;//включен, работает
            }else{
            //если нет частотника, сделаем вывод о включенности по пускателю
                if(wnd->data.pca9555_output1R & (1<< wnd->data.nasos1_bit) ){
                    wnd->data.nasos[0] = 2;//включен, работает
                }
            }
            if(wnd->data.max11616[wnd->data.nasos1_current_alarm_bit] > wnd->data.nasos1_current_alarm_border){
                wnd->data.nasos[0] = 3;//авария по току
            }
            if(wnd->data.max11616[wnd->data.nasos1_temp_alarm_bit] > wnd->data.nasos1_temp_alarm_border){
                wnd->data.nasos[0] = 4;////авария по температуре
            }
            if(wnd->data.max11616[wnd->data.nasos1_wet_alarm_bit] > wnd->data.nasos1_wet_alarm_border){
                wnd->data.nasos[0] = 5;//авария по влажности
            }
         }

        if(wnd->data.nasos2_bit == -1)wnd->data.nasos[1] = 0;//нет насоса
        else{
            wnd->data.nasos[1] = 1;//не включен, готов
            if(wnd->isATV12){
                if(wnd->data.ATV12status[1] == RUN)
                    wnd->data.nasos[1] = 2;//включен, работает
            }else{
                if(wnd->data.pca9555_output1R & (1<< wnd->data.nasos2_bit) ){
                    wnd->data.nasos[1] = 2;//включен, работает
                }
            }
            if(wnd->data.max11616[wnd->data.nasos2_current_alarm_bit] > wnd->data.nasos2_current_alarm_border){
                wnd->data.nasos[1] = 3;//авария по току
            }
            if(wnd->data.max11616[wnd->data.nasos2_temp_alarm_bit] > wnd->data.nasos2_temp_alarm_border){
                wnd->data.nasos[1] = 4;////авария по температуре
            }
            if(wnd->data.max11616[wnd->data.nasos2_wet_alarm_bit] > wnd->data.nasos2_wet_alarm_border){
                wnd->data.nasos[1] = 5;//авария по влажности
            }
        }

        if(wnd->data.nasos3_bit == -1)wnd->data.nasos[2] = 0;//нет насоса
        else{
            wnd->data.nasos[2] = 1;//не включен, готов
            if(wnd->isATV12){
                if(wnd->data.ATV12status[2] == RUN)
                    wnd->data.nasos[2] = 2;//включен, работает
            }else{
                if(wnd->data.pca9555_output1R & (1<< wnd->data.nasos3_bit) ){
                    wnd->data.nasos[2] = 2;//включен, работает
                }
            }
            if(wnd->data.max11616[wnd->data.nasos3_current_alarm_bit] > wnd->data.nasos3_current_alarm_border){
                wnd->data.nasos[2] = 3;//авария по току
            }
            if(wnd->data.max11616[wnd->data.nasos3_temp_alarm_bit] > wnd->data.nasos3_temp_alarm_border){
                wnd->data.nasos[2] = 4;////авария по температуре
            }
            if(wnd->data.max11616[wnd->data.nasos3_wet_alarm_bit] > wnd->data.nasos3_wet_alarm_border){
                wnd->data.nasos[2] = 5;//авария по влажности
            }
        }

        if(wnd->data.nasos4_bit == -1)wnd->data.nasos[3] = 0;//нет насоса
        else{
            wnd->data.nasos[3] = 1;//не включен, готов
            if(wnd->isATV12){
                if(wnd->data.ATV12status[3] == RUN)
                    wnd->data.nasos[3] = 2;//включен, работает
            }else{
                if(wnd->data.pca9555_output1R & (1<< wnd->data.nasos4_bit) ){
                    wnd->data.nasos[3] = 2;//включен, работает
                }
            }
            if(wnd->data.max11616[wnd->data.nasos4_current_alarm_bit] > wnd->data.nasos4_current_alarm_border){
                wnd->data.nasos[3] = 3;//авария по току
            }
            if(wnd->data.max11616[wnd->data.nasos4_temp_alarm_bit] > wnd->data.nasos4_temp_alarm_border){
                wnd->data.nasos[3] = 4;////авария по температуре
            }
            if(wnd->data.max11616[wnd->data.nasos4_wet_alarm_bit] > wnd->data.nasos4_wet_alarm_border){
                wnd->data.nasos[3] = 5;//авария по влажности
            }
        }
        //обработаем ошибки и выключим моторы
        if(wnd->data.nasos[0] == 0 || wnd->data.nasos[0] > 2){
                wnd->data.pca9555_output1W &= ~(1<<wnd->data.nasos1_bit);
        }
        if(wnd->data.nasos[1] == 0 || wnd->data.nasos[1] > 2){
                wnd->data.pca9555_output1W &= ~(1<<wnd->data.nasos2_bit);
        }
        if(wnd->data.nasos[2] == 0 || wnd->data.nasos[2] > 2){
                wnd->data.pca9555_output1W &= ~(1<<wnd->data.nasos3_bit);
        }
        if(wnd->data.nasos[3] == 0 || wnd->data.nasos[3] > 2){
                wnd->data.pca9555_output1W &= ~(1<<wnd->data.nasos4_bit);
        }
//wnd->data.nasos[2] = 2;
        for(int i=0;i<4;i++){
            if(wnd->data.nasos[i] == 2){
                wnd->data.nasos_TTW[i]++;//время работы плюсанем
            }
        }

        //обработка уровней
       if(wnd->islevel_meter){
        //датчик уровня
         if(wnd->data.max11616[wnd->data.level_input_number] == -1 ||                                              //АЦП не отвечает
            wnd->data.max11616[wnd->data.level_input_number] >= 3500 ||                                         //датчик в КЗ
            wnd->data.max11616[wnd->data.level_input_number] <  ((double)(wnd->data.level_empty_raw * 0.9)) ){//датчик отвалился
               //проверим аварийный поплавок
               if(wnd->data.pca9555_input0 == -1 || (wnd->data.pca9555_input0 & (1<<wnd->data.level_4_bit)) == 0 ){
                 //если не читается дискретный вход или сработал датчик перелива
                wnd->data.level_to_show = 0;
                //стоп моторам
                wnd->data.motor_need_to_stop = true;
                wnd->data.time_to_stop = wnd->data.overlevel_time_to_stop;
                qCritical()<< QString("level meter alarm %1").arg(wnd->data.max11616[wnd->data.level_input_number]);
               }else{
                wnd->data.level_to_show = 1;//уровнемер отвалился, перелива пока нет
               }
         }else{
           wnd->data.level_to_show_sm = (double)(wnd->data.level_full_sm * (wnd->data.max11616[wnd->data.level_input_number] - wnd->data.level_empty_raw)) / wnd->data.level_full_raw;
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
           //проверим аварийный поплавок
           if(wnd->data.pca9555_input0 == -1 || (wnd->data.pca9555_input0 & (1<<wnd->data.level_4_bit)) == 0 ){
             //если не читается дискретный вход или сработал датчик перелива
            wnd->data.level_to_show = 0;
            //стоп моторам
            wnd->data.motor_need_to_stop = true;
            wnd->data.time_to_stop = wnd->data.overlevel_time_to_stop;
            qCritical("overlevel alarm");
           }else{
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

               if(wnd->data.level_to_show >= 10 && wnd->data.level_to_show <= 20){
                   //стоп моторам
                   wnd->data.motor_need_to_stop = true;
                   wnd->data.time_to_stop = wnd->data.overlevel_time_to_stop;
                   qCritical("alarm on unknown level");
               }
           }
         }
//возникла ошибка переполнения бочки, остановим моторы
        if(wnd->data.motor_need_to_stop == true){
            if(wnd->data.time_to_stop == -1 || //время плавной остановки в конфиге не задано. тупо тормозим
               wnd->data.time_to_stop == 0 ){//пришло время остановки
                if(wnd->isATV12){//частотникам пошлем команду
                    wnd->data.stop[0]=wnd->data.stop[1]=wnd->data.stop[2]=wnd->data.stop[3] = true;
                    wnd->data.start[0]=wnd->data.start[1]=wnd->data.start[2]=wnd->data.start[3]=false;
                }else{//ппуски просто отключаем пускателями
                    wnd->data.pca9555_output1W &= ~(1<<wnd->data.nasos1_bit);
                    wnd->data.pca9555_output1W &= ~(1<<wnd->data.nasos2_bit);
                    wnd->data.pca9555_output1W &= ~(1<<wnd->data.nasos3_bit);
                    wnd->data.pca9555_output1W &= ~(1<<wnd->data.nasos4_bit);
                }
                wnd->data.nasos[3] = wnd->data.nasos[2] = wnd->data.nasos[1] = wnd->data.nasos[0] = 1;//не включен, готов
//убедиться, что после этого никто не пытается включать насосы
                wnd->data.motor_need_to_stop = false;
            }else{
                wnd->data.time_to_stop--;
                //здесь надо приводам частоту снижать
                for(int i=0;i<4;i++){
                    if(wnd->data.freq_w[i]>50){//если частота привода больше 5 Гц
                        wnd->data.freq_w[i] -= 10;//уменьшим на 1 Гц
                    }
                }
            }
        }
        //=======================================================================
        if(wnd->data.servicemode){              //сервисный режим включен
            if(wnd->data.servicemodeTTL){       //время еще не вышло
                wnd->data.servicemodeTTL--;     //отнимем секунду
            }else{
                wnd->data.servicemode = false;  //отключим сервисный режим
                qDebug("Service mode timeout. Disable.");
            }
        }

        //=======================================================================
        // MainWindow в любом случае активно, повесим обработчики на него
        emit changeDataMain();
        //ждем 1 секунду (на самом деле чуть больше)
        QThread::msleep(1000);

/*
        for(int i = 0; i < 4; i++){
            wnd->data.freq_w[i] = 200;
        }
        QThread::msleep(8000);
        for(int i = 0; i < 4; i++){
            wnd->data.freq_w[i] = 300;
        }
        QThread::msleep(8000);
*/


    //usleep(200000);
    }//end while(done)
    //fprintf(stderr, "!");//плюнем в консоль
}
//--------------------------------------------------------------------------------------
void Thread_math::quit(){
    wnd->done = false;
    qDebug() << "Thread_math::quit";
}


