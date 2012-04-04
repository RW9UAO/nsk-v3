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

//=========================================================================================================================================================
Thread_math::Thread_math(MainWindow *wnd) : ui(wnd->ui), wnd(wnd){
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
//поиск насоса с минимальной наработкой
//поиск готового насоса (без алармов), вне резерва
int Thread_math::getminTTW(){
    int minval = wnd->data.nasos_TTW[0], minindex = -1;
    //поищем с минимальной наработкой
    for(int i = 0; i < 4; i++){
        if(wnd->data.nasos_TTW[i] <= minval &&//если только < то при 0-й наработке вернет -1
                wnd->data.nasos_rez[i] == false &&//вне резерва
                wnd->data.nasos[i] == 1){       //и в состоянии готовности
            minval = wnd->data.nasos_TTW[i];
            minindex = i;
        }
    }
    return minindex;
}
//=========================================================================================================================================================
double Thread_math::PID(){
    double error, dTerm;
    double currentPos, G_Dt;
    double result = 0;

    //PID section
            G_Dt = 1 / 1000;// 1 second
            //targetPos get from config file
            //Pparam, Dparam, Iparam get from config file
            currentPos = wnd->data.max11616[10];
            //IntegratedError накапливается со временем
    //проверим входной паказатель на предмет ошибки
            if(currentPos >= 0 && currentPos <= 4096){
                error = wnd->data.targetPos - currentPos;
                wnd->data.IntegratedError += error * G_Dt;
                //constrain IntegratedError whith +/-window
                //if(IntegratedError > )..... etc
                dTerm = wnd->data.Dparam * (currentPos - wnd->data.LastPosition);
                wnd->data.LastPosition = currentPos;
                result = (wnd->data.Pparam * error) + (wnd->data.Iparam * wnd->data.IntegratedError) + dTerm;
            }else{
    //ошибочный, останавливаем мотор
                qDebug() << QString("Thread_math::ERROR reading max11616[10] %1").arg(currentPos);
                //wnd->data.motor_need_to_stop = true;
                //wnd->data.time_to_stop = wnd->data.overlevel_time_to_stop;
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
    switch(i){
    case 0:wnd->data.pca9555_output1W |= (1<< wnd->data.nasos1_bit);  break;
    case 1:wnd->data.pca9555_output1W |= (1<< wnd->data.nasos2_bit);  break;
    case 2:wnd->data.pca9555_output1W |= (1<< wnd->data.nasos3_bit);  break;
    case 3:wnd->data.pca9555_output1W |= (1<< wnd->data.nasos4_bit);  break;
    }
    //дадим команду частотнику на запуск
    if(wnd->data.isATV12){//частотник
        //wnd->data.freq_w[i] = 250;//по идее сюда частоту должен ПИД регулятор отдать
        wnd->data.freq_w[i] = 30;//по идее сюда частоту должен ПИД регулятор отдать
        //wnd->data.freq_w[i] = constrain(wnd->data.freq_w[i] + PID());
        wnd->data.stop[i] = false; wnd->data.start[i]= true;
        qDebug() << QString("Level 2, start %1").arg(i+1);
    }
}
//=========================================================================================================================================================
void Thread_math::run() {
    int nasos_time_to_overtime[4]={0,0,0,0};//время непрерывной работы насоса.

    emit changeDataMain();//отрисуем MainWindow
    //подождем пока I2C поток запустится и даст данные
QThread::msleep(9000);

    // вот тут вся математика в основном цикле
    while(wnd->done){
//=========================================================================================================================================================
 //проверим аварии с 35 мс
        //завести массив из wnd->data.nasos1_bit вместо 4-х переменных
        //проверку на ответ частотника
        if(wnd->data.nasos1_bit == -1 || wnd->data.ATV12status[0] == OFFLINE)wnd->data.nasos[0] = 0;//нет насоса
        else{
            wnd->data.nasos[0] = 1;//не включен, готов
            //если есть частотник, сделаем вывод о включенности по команде "start"
            if(wnd->data.isATV12){
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
            if(wnd->data.max11616[wnd->data.nasos1_temp_alarm_bit] > wnd->data.nasos1_wet_alarm_border){
                wnd->data.nasos[0] = 5;//авария по влажности
            }
            if(wnd->data.max11616[wnd->data.nasos1_temp_alarm_bit] > 4000 || wnd->data.max11616[wnd->data.nasos1_temp_alarm_bit] < 1000){
                wnd->data.nasos[0] = 6;//авария по обрыву кабеля
            }
         }
//------------------------------------------------
        if(wnd->data.nasos2_bit == -1)wnd->data.nasos[1] = 0;//нет насоса
        else{
            wnd->data.nasos[1] = 1;//не включен, готов
            if(wnd->data.isATV12){
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
            if(wnd->data.max11616[wnd->data.nasos2_temp_alarm_bit] > wnd->data.nasos2_wet_alarm_border){
                wnd->data.nasos[1] = 5;//авария по влажности
            }
            if(wnd->data.max11616[wnd->data.nasos2_temp_alarm_bit] > 4000 || wnd->data.max11616[wnd->data.nasos2_temp_alarm_bit] < 1000){
                wnd->data.nasos[1] = 6;//авария по обрыву кабеля
            }
        }
//---------------------------------------------------
        if(wnd->data.nasos3_bit == -1)wnd->data.nasos[2] = 0;//нет насоса
        else{
            wnd->data.nasos[2] = 1;//не включен, готов
            if(wnd->data.isATV12){
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
            if(wnd->data.max11616[wnd->data.nasos3_temp_alarm_bit] > wnd->data.nasos3_wet_alarm_border){
                wnd->data.nasos[2] = 5;//авария по влажности
            }
            if(wnd->data.max11616[wnd->data.nasos3_temp_alarm_bit] > 4000 || wnd->data.max11616[wnd->data.nasos3_temp_alarm_bit] < 1000){
                wnd->data.nasos[2] = 6;//авария по обрыву кабеля
            }
        }
//------------------------------------------------------
        if(wnd->data.nasos4_bit == -1)wnd->data.nasos[3] = 0;//нет насоса
        else{
            wnd->data.nasos[3] = 1;//не включен, готов
            if(wnd->data.isATV12){
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
            if(wnd->data.max11616[wnd->data.nasos4_temp_alarm_bit] > wnd->data.nasos4_wet_alarm_border){
                wnd->data.nasos[3] = 5;//авария по влажности
            }
            if(wnd->data.max11616[wnd->data.nasos4_temp_alarm_bit] > 4000 || wnd->data.max11616[wnd->data.nasos4_temp_alarm_bit] < 1000){
                wnd->data.nasos[3] = 6;//авария по обрыву кабеля
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
//=========================================================================================================================================================
        for(int i=0;i<4;i++){
            if(wnd->data.nasos[i] == 2){
                wnd->data.nasos_TTW[i]++;//время работы плюсанем
                nasos_time_to_overtime[i]++;
                if(nasos_time_to_overtime[i] > 900){//насос работает слишком долго
                    //сбросим время непрерывной работы насоса
                    nasos_time_to_overtime[i] = 0;
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
               //проверим аварийный поплавок
               if(wnd->data.pca9555_input0 == -1 || (wnd->data.pca9555_input0 & (1<<wnd->data.level_4_bit)) == 0 ){
                 //если не читается дискретный вход или сработал датчик перелива
                wnd->data.level_to_show = 0;
                //стоп моторам
                if(wnd->data.time_to_stop == false){
                    wnd->data.motor_need_to_stop = true;
                    wnd->data.time_to_stop = wnd->data.overlevel_time_to_stop;
                 }
                if((wnd->data.error_flags & ERROR_LEVEL_METER)==0){
                    wnd->data.error_flags |= ERROR_LEVEL_METER;
                    qCritical()<< QString("level meter alarm %1").arg(wnd->data.max11616[wnd->data.level_input_number]);
                 }
               }else{
                wnd->data.level_to_show = 1;//уровнемер отвалился, перелива пока нет
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
           //проверим аварийный поплавок
           if(wnd->data.pca9555_input0 == -1 || (wnd->data.pca9555_input0 & (1<<wnd->data.level_4_bit)) == 0 ){
             //если не читается дискретный вход или сработал датчик перелива
            wnd->data.level_to_show = 0;
            //стоп моторам
            if(wnd->data.time_to_stop == false){
                wnd->data.motor_need_to_stop = true;
                wnd->data.time_to_stop = wnd->data.overlevel_time_to_stop;
            }
            qCritical("overlevel swimmer alarm");
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
                   if(wnd->data.time_to_stop == false){
                    wnd->data.motor_need_to_stop = true;
                    wnd->data.time_to_stop = wnd->data.overlevel_time_to_stop;
                   }
                   qCritical("alarm on unknown level swimmer");
               }
           }
         }
//=========================================================================================================================================================
//возникла ошибка переполнения бочки, либо слив закончен, остановим моторы
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
                                nasos_time_to_overtime[i] = 0;//обнулим время непрерывной работы насоса
                                i = 100500;// х.з. как тут break сработает или нет
                            }
                        }
                }else{//ппуски просто отключаем пускателями
                    for(int i = 0; i < 4; i++){
                        if( wnd->data.nasos[i] == 2){//включен, работает
                            qDebug() << QString("motor STOP %1").arg(i+1);
                            switch(i){
                            case 0:wnd->data.pca9555_output1W &= ~(1<<wnd->data.nasos1_bit);break;
                            case 1:wnd->data.pca9555_output1W &= ~(1<<wnd->data.nasos2_bit);break;
                            case 2:wnd->data.pca9555_output1W &= ~(1<<wnd->data.nasos3_bit);break;
                            case 3:wnd->data.pca9555_output1W &= ~(1<<wnd->data.nasos4_bit);break;
                            }
                            i = 100500;
                        }
                    }
                }
//                wnd->data.nasos[3] = wnd->data.nasos[2] = wnd->data.nasos[1] = wnd->data.nasos[0] = 1;//не включен, готов
//убедиться, что после этого никто не пытается включать насосы
                //wnd->data.motor_need_to_stop = false;
            }else{
                wnd->data.time_to_stop--;
                //здесь надо приводам частоту снижать
                qDebug() << QString("time to stop = %1").arg(wnd->data.time_to_stop);
                for(int i=0;i<4;i++){
                    //if(wnd->data.freq_w[i]>50){//если частота привода больше 5 Гц
                    if(wnd->data.freq_w[i]>25){//если частота привода больше 5 Гц
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
                        wnd->data.time_to_stop = wnd->data.overlevel_time_to_stop;
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
                    }else qCritical("need to start second pump, but has no free. Алярма!");
                }
            }
            //перелив, включим три насоса
        }else{
//поплавки
            //если уровень жидкости на нулевом уровне - выключаем все включенные насосы.
            if((wnd->data.pca9555_input0 & (1<<wnd->data.level_1_bit)) != 0 &&
                    (wnd->data.pca9555_input0 & (1<<wnd->data.level_2_bit)) != 0 &&
                    (wnd->data.pca9555_input0 & (1<<wnd->data.level_3_bit)) != 0 &&
                    (wnd->data.pca9555_input0 & (1<<wnd->data.level_4_bit)) != 0 && //не сработал ни один поплавок
                    wnd->data.level_to_show == 102){                                //и не было ошибок
                qDebug("Level < level_1_bit, STOP motor");
                if(wnd->data.motor_need_to_stop == false){
                    wnd->data.motor_need_to_stop = true;                // остановим моторы
                    wnd->data.time_to_stop = wnd->data.overlevel_time_to_stop;
                 }
            }
            //если уровень выше первого, но ниже второго - ничего не делаем
            if((wnd->data.pca9555_input0 & (1<<wnd->data.level_1_bit)) == 0 &&
                    (wnd->data.pca9555_input0 & (1<<wnd->data.level_2_bit)) != 0 &&
                    wnd->data.level_to_show == 103){
            }

        }
//=========================================================================================================================================================
        // MainWindow в любом случае активно, повесим обработчики на него
        emit changeDataMain();
        if(wnd->dlgNasos1->isVisible()){
            emit changeDataNasos1();
        }
/*        if(wnd->dlgNasos2->isVisible()){
            emit changeDataNasos2();
        }
        if(wnd->dlgNasos3->isVisible()){
            emit changeDataNasos3();
        }
        if(wnd->dlgNasos4->isVisible()){
            emit changeDataNasos4();
        }*/
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
    //qDebug() << "Thread_math::quit";
}


