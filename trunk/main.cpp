#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QThread>
#include <QLatin1String>
#include <QDialog>
#include <QtGui>
#include <QtGui/QApplication>
#include <fcntl.h>          //is defined open and close
#include <unistd.h>         //is defined read and write
#include <iostream>
#include "mainwindow.h"
#include "thread_485.h"
#include "thread_I2C.h"
#include "thread_math.h"


// Данные для ведения логов
static QTextStream* logStream;
static QFile* logFile;
//const QString TextDescription = QString("%1 Built on " __DATE__ " at " __TIME__ ".\n").arg(QLatin1String(APP_VERSION) );
const QString TextDescription = QString("Built on " __DATE__ " at " __TIME__ ".\n");
// Типы сообщений
static const char* msgType[] ={
"(II) ", // Info
"(WW) ", // Warning
"(EE) ", // Error
"(FF) " // Fatal error
};
//==============================================================================================
// Вывод логов в файл
//qDebug(...);
//qWarning(...);
//qCritical(...);
//qFatal
void customMessageHandler(QtMsgType type, const char* msg){
    std::cout << msgType[type] << msg << std::endl;
    if(logStream && logStream->device()){
        *logStream << msgType[type] << QDateTime::currentDateTime().toString("dd.MM.yy/hh:mm:ss") << " "<< msg << endl;
    }
}
// Создание файла для логов
void installLog(){
    logFile = new QFile("messages.log");
    if(logFile->open(QFile::WriteOnly | QIODevice::Append | QIODevice::Unbuffered))
        logStream = new QTextStream(logFile);

    logStream->setCodec("utf-8");

// Запись заголовка с информацией о запуске
    if(logStream && logStream->device()){
        *logStream << endl << TextDescription << endl;
/*        *logStream << QString("Markers: (II) informational, (WW) warning,") << endl;
        *logStream << QString("(EE) error, (FF) fatal error.") << endl;
        *logStream << getOSInfo() << endl;*/
        *logStream << QString("Runned at %1.").arg(QDateTime::currentDateTime().toString()) << endl << endl;
    }

    qInstallMsgHandler(customMessageHandler);

    qDebug("Success opening log file");
}
// Закрытие файла логов
void finishLog(){
    qDebug("Success closing log file");

    delete logStream;
    logStream = 0;
    delete logFile;
    logFile = 0;

qInstallMsgHandler(0);
}
//==============================================================================================
int main(int argc, char *argv[]){

    installLog();

    QApplication a(argc, argv);
    MainWindow w;
    Thread_485 t(&w);
    Thread_math t2(&w);
    Thread_I2C t3(&w);

    // установим кодировку
    QTextCodec *cyrillicCodec = QTextCodec::codecForName("UTF8-");
    QTextCodec::setCodecForTr(cyrillicCodec);
    QTextCodec::setCodecForLocale(cyrillicCodec);
    QTextCodec::setCodecForCStrings(cyrillicCodec);

    //покажем основное окно
    w.show();
// сигналы для обновления отрисовки диалогов
    QObject::connect(
                &t, SIGNAL(changeDataATV12()),
                &w, SLOT(onDataChangeATV12()),
                Qt::QueuedConnection);
    QObject::connect(
                &t, SIGNAL(changeDataCE303()),  //это два сигнала из одного потока
                &w, SLOT(onDataChangeCE303()),
                Qt::QueuedConnection);
    QObject::connect(
                &t2, SIGNAL(changeDataMain()),
                &w, SLOT(onDataChangeMain()),
                Qt::QueuedConnection);
    /*QObject::connect(
                &t2, SIGNAL(changeDataPower()),
                &w, SLOT(onDataChangePower()),
                Qt::QueuedConnection);*/
    QObject::connect(
                &t2, SIGNAL(changeDataNasos1()),
                &w, SLOT(onDataChangeNasos1()),
                Qt::QueuedConnection);
    /*QObject::connect(
                &t2, SIGNAL(changeDataNasos2()),
                &w, SLOT(onDataChangeNasos2()),
                Qt::QueuedConnection);
    QObject::connect(
                &t2, SIGNAL(changeDataNasos3()),
                &w, SLOT(onDataChangeNasos3()),
                Qt::QueuedConnection);
    QObject::connect(
                &t2, SIGNAL(changeDataNasos4()),
                &w, SLOT(onDataChangeNasos4()),
                Qt::QueuedConnection);*/
    QObject::connect(
                &t3, SIGNAL(changeDataI2C()),
                &w, SLOT(onDataChangeI2C()),
                Qt::QueuedConnection);
    QObject::connect(
                &t3, SIGNAL(changeDataCalibr()),
                &w, SLOT(onDataChangeCalibr()),
                Qt::QueuedConnection);
//запустим потоки
    t.init();//инициализация переменных
    t.start(QThread::LowPriority);
    //t.start(QThread::TimeCriticalPriority);
    t2.start(QThread::LowPriority);
    t3.start(QThread::LowPriority);

//запуск приложения
    a.exec();
//прибьем потоки
    t.quit();
    t2.quit();
    t3.quit();
//подождем пока завершатся
    t.wait();
    //qDebug("w485");
    t2.wait();
    //qDebug("wMath");
    t3.wait();
//выход в ОС
    finishLog();
    return 0;
}
