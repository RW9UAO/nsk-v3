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
    QString tempstr;
    int dialogparam;
    bool servicemode;
    //ATV12 data
    int lft[6];
    int eta[6];
    int freq[6];
    int freq_w[6];
    int current[6];
    bool start[6];
    bool stop[6];
    int ATV12status[6];
    //CE303 data
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
    int pca9555_input0, pca9555_input0_previos;
    int pca9555_output1R;
    int pca9555_output1W;
    int max11616[12], max11616_previos[12];
    //PID
    double targetPos, Pparam, Dparam, Iparam;
    int nasos[4], nasos_TTW[4];
    int nasos1_bit, nasos2_bit, nasos3_bit, nasos4_bit;
    int nasos1_current_alarm_bit,nasos2_current_alarm_bit,nasos3_current_alarm_bit,nasos4_current_alarm_bit;
    int nasos1_temp_alarm_bit,nasos2_temp_alarm_bit,nasos3_temp_alarm_bit,nasos4_temp_alarm_bit;
    int nasos1_wet_alarm_bit,nasos2_wet_alarm_bit,nasos3_wet_alarm_bit,nasos4_wet_alarm_bit;
    int nasos1_current_alarm_border,nasos2_current_alarm_border,nasos3_current_alarm_border,nasos4_current_alarm_border;
    int nasos1_temp_alarm_border,nasos2_temp_alarm_border,nasos3_temp_alarm_border,nasos4_temp_alarm_border;
    int nasos1_wet_alarm_border,nasos2_wet_alarm_border,nasos3_wet_alarm_border,nasos4_wet_alarm_border;
    int level_input_number, level_empty_raw, level_full_raw, level_full_sm;
    int level_1_bit, level_2_bit, level_3_bit, level_4_bit;
    int level_1_sm, level_2_sm, level_3_sm, level_4_sm, overlevel_time_to_stop;
    bool motor_need_to_stop;
    int time_to_stop;
    int level_to_show, level_to_show_sm;
};

#endif // DATA_STRUCT_H
