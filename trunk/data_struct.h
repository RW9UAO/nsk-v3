#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#define RUN 1
#define STOP 0
#define ERROR 2
#define OFFLINE -1

#include <QTextStream>
#include <qtextcodec.h>

// глобальные переменные, задаем и обрабатываем в разных потоках
struct data_struct{
    unsigned int error_flags;
#define ERROR_ATV12_1 1<<0
#define ERROR_ATV12_2 1<<1
#define ERROR_ATV12_3 1<<2
#define ERROR_ATV12_4 1<<3
#define ERROR_CE303 1<<4
#define ERROR_21 1<<5
#define ERROR_35 1<<6
#define ERROR_LEVEL_METER 1<<7
#define ALARM_OVERLEVE 1<<8
#define ALARM_220 1<<9
    bool isSoftStart;//плавный пуск есть?
    QString tempstr;//переменная для обмена с диалогами
    int dialogparam;//передадим параметр в диалог
    bool servicemode;//сервисный режим, доступ к настройкам
    int servicemodeTTL;//время жизни сервисного режима
    QString KNSnumber;//номер КНС
    QString IPboardAddr;
    bool need_to_set_date_time_fromCE303;
    //ATV12 data
    bool isATV12;// частотник есть?
    int ATV12maxNum;//частотник последний номер, не больше 6-ти
    int lft[6];//слово состояния привода
    int eta[6];//слово состояния привода
    int freq[6];//текущая частота привода
    int freq_w[6];//желаемая частота привода
    int current[6];//текущий ток
    bool start[6];//команда на старт
    bool stop[6];//команда на стоп
    int ATV12status[6];
    //CE303 data
    bool isCE303;//счетчик есть? а если найду?
    QString time_;
    QString date_;
    int fccur;
    int condi;
    int powep;
    int poweq;
    int curra, currb, currc;
    int cos_f;
    int angleAB, angleBC, angleCA;
    int volta, voltb, voltc;
    int et0pe;
    int CE303needupdate;
#define update_none 0
#define update_time 1
#define update_date 2
#define update_trans 3
    //I2C data
    bool is21;
    int pca9555_input0;
    int pca9555_output1R;
    int pca9555_output1W;
    int pca9555ADDR;
    bool is35;
    int max11616[12], max11616ADDR;
    //PID
    double targetPos, Pparam, Dparam, Iparam, IntegratedError, LastPosition;
    int nasos[4];//состояние насоса
    int nasos_TTW[4];//время наработки насоса
    int nasos_bit[4];
    int nasos_current_alarm_bit[4];
    int nasos_temp_alarm_bit[4];
    int nasos_current_alarm_border[4];
    int nasos_temp_alarm_border[4];
    int nasos_wet_alarm_border[4];
    int level_input_number, level_empty_raw, level_full_raw, level_full_sm;
    int level_1_bit, level_2_bit, level_3_bit, level_4_bit;
    int level_1_sm, level_2_sm, level_3_sm, level_4_sm, overlevel_time_to_stop;
    bool motor_need_to_stop;//надо остановить насосы
    int time_to_stop;//вспомогательная переменная, счетчик времени до остановки
    bool islevel_meter;//наличие датчика уровня на I2C
    int level_to_show, level_to_show_sm;
    int nasos_param_index[4];
    struct n_param{
        QString name;
        QString articul;
        int pressure;
        int volume;
        int current;
        int rpm;
        QString starttype;
        QString cable;
        QString pictname;
    }nasos_param[11];
    bool need_to_save_config;//надо записать конфиг
    //bool nasos1_on,nasos2_on,nasos3_on,nasos4_on;//признак включенного насоса
    bool nasos_rez[4];//признак резервного насоса
};

#endif // DATA_STRUCT_H
