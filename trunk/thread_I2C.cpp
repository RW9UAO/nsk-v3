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
#include "thread_I2C.h"

//--------------------------------------------------------------------------------------
Thread_I2C::Thread_I2C(MainWindow *wnd) : ui(wnd->ui), wnd(wnd){
}
//--------------------------------------------------------------------------------------
void Thread_I2C::run() {
    QFile file;
    QTextStream in, out;
    QString line;
    qDebug() << "Thread_I2C::run";


    // вот тут опрос
    while(wnd->done){
//опросим дискретный ввод/вывод
      if(wnd->is21){
        file.setFileName("/sys/bus/i2c/devices/1-0021/name");
        in.setDevice(&file);
        if( file.open(QIODevice::ReadOnly) ){ // откроем файл для чтения
//            line = in.readLine();
//            qDebug()<<"0-0021 name:"<<line;
            file.close();//close name file
            // cчитаем показатели из PCA9555
            file.setFileName("/sys/bus/i2c/devices/1-0021/input0");
            in.setDevice(&file);
            if( file.open(QIODevice::ReadOnly) ){ // откроем файл для чтения
                line = in.readLine();
//                qDebug() << line;
                //wnd->data.pca9555_input0 = line.toInt();
                int temp = line.toInt();
                if(temp == wnd->data.pca9555_input0_previos){
                    wnd->data.pca9555_input0 = temp;
                }
                wnd->data.pca9555_input0_previos = temp;
//                QString s = QString("pca9555_input0: %1").arg(wnd->data.pca9555_input0);
//                qDebug() << s;
                file.close();
            }else{
                qCritical("error open /sys/bus/i2c/devices/1-0021/input0");
                wnd->data.pca9555_input0 = -1;
            }
            file.setFileName("/sys/bus/i2c/devices/1-0021/input1");
            in.setDevice(&file);
            if( file.open(QIODevice::ReadOnly) ){ // откроем файл для чтения
                line = in.readLine();
                wnd->data.pca9555_output1R = line.toInt();
//                qDebug() << QString("pca9555_output1R: %1").arg(wnd->data.pca9555_output1R);
                file.close();
                if(wnd->data.pca9555_output1R != wnd->data.pca9555_output1W){
                    qDebug("need to change pca9555:output1");
                    file.setFileName("/sys/bus/i2c/devices/1-0021/output1");
                    out.setDevice(&file);
                    if( file.open(QIODevice::WriteOnly) ){ // откроем файл для чтения
                        out << QString("%1").arg(wnd->data.pca9555_output1W);
                        file.close();
                    }else{
                        qCritical("error open /sys/bus/i2c/devices/1-0021/output1");
                    }
                }
            }else{
                qCritical("error open /sys/bus/i2c/devices/1-0021/input1");
                wnd->data.pca9555_output1R = -1;
            }
        }else{
            qCritical("error open /sys/bus/i2c/devices/1-0021/name");
        }
      }
//опросим аналоговый вход
      if(wnd->is35){
        file.setFileName("/sys/bus/i2c/devices/0-0035/name");
        in.setDevice(&file);// создадим поток для чтения
        if( file.open(QIODevice::ReadOnly) ){ // откроем файл для чтения
            line = in.readLine();
  //          qDebug()<<"0-0035 name:"<<line;
            file.close();//close name file
            for(int i = 0; i < 12; i++){
                QString filename=QString("/sys/bus/i2c/devices/0-0035/input%1").arg(i);
                file.setFileName(filename);
                in.setDevice(&file);// создадим поток для чтения
                if( file.open(QIODevice::ReadOnly) ){
                    QThread::msleep(20);
                    line = in.readLine();
                    //надо проверить на валидность показаний +/- 10%
                    //wnd->data.max11616[i] = line.toInt();
                    int temp = line.toInt();
                    if(temp == wnd->data.max11616_previos[i]){
                        wnd->data.max11616[i] = temp;
                    }
                    wnd->data.max11616_previos[i] = temp;
                }else{
                    qCritical() << "error open " << filename;
                    wnd->data.max11616[i] = -1;
                }
                file.close();
            }
//            qDebug() << QString("analog in: %1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12").arg(wnd->data.max11616[0]).arg(wnd->data.max11616[1])
//                        .arg(wnd->data.max11616[2]).arg(wnd->data.max11616[3]).arg(wnd->data.max11616[4]).arg(wnd->data.max11616[5])
//                        .arg(wnd->data.max11616[6]).arg(wnd->data.max11616[7]).arg(wnd->data.max11616[8]).arg(wnd->data.max11616[9])
//                        .arg(wnd->data.max11616[10]).arg(wnd->data.max11616[11]);
        }else{
            qCritical("error open /sys/bus/i2c/devices/0-0035/name");
        }
    }
//обновим показания диалога I2C
        if (NULL != wnd->dlgI2C.get()) {
            emit changeDataI2C();
        }
        //QThread::msleep(100);//0.1 second
        QThread::msleep(500);//0.5 second
//        QThread::msleep(5000);//5 second
    }
//    fprintf(stderr, "!");//плюнем в консоль

}
//--------------------------------------------------------------------------------------
void Thread_I2C::quit(){
    wnd->done = false;
    qDebug() << "Thread_I2C::quit";
}



