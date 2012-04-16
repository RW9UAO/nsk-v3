#include "dialogService.h"
#include "ui_dialogService.h"
#include "data_struct.h"
#include "mainwindow.h"
#include "keypad.h"

DialogService::DialogService(data_struct * d, QWidget *parent) : QDialog(parent), dui(new Ui::DialogService){
    dui->setupUi(this);
    this->data = d;

    dui->progressBar->hide();

    if(this->data->servicemode){
        dui->label->setText("Сервисный режим включен");
        dui->pushButton->show();
    }else{
        dui->label->setText("Режим оператора");
        dui->pushButton->hide();
    }
}

DialogService::~DialogService(){
    delete dui;
}
// обновим содержимое диалога
void DialogService::updateDataService(){
}


void DialogService::on_ButtonEnterService_clicked(){
    this->data->dialogparam = 4;//параметр для "подсказки"
    this->data->tempstr = "";
    QDialog * d_key = new Dialogkeypad(this->data, this);
    QPoint pos = d_key->pos();//удвинем окно в угол
    pos.setX(0);    pos.setY(0);    d_key->move(pos);
    d_key->exec();// покажем диалог
    if(this->data->tempstr.size() > 1){//если юзер что-то ввел
        //qDebug()<< QString("%1").arg(this->data->tempstr);
        //сравнение введенного пароля и номера КНС
        if(this->data->tempstr[0] == this->data->KNSnumber[0] &&
            this->data->tempstr[1] == this->data->KNSnumber[1] &&
            this->data->tempstr[2] == this->data->KNSnumber[2] &&
            this->data->tempstr[3] == this->data->KNSnumber[2] &&
            this->data->tempstr[4] == this->data->KNSnumber[1] &&
            this->data->tempstr[5] == this->data->KNSnumber[0]){
            dui->label->setText("Сервисный режим включен");
            this->data->servicemode = true;
            this->data->servicemodeTTL = 30;//секунд
        }else{
            dui->label->setText("Неправильный пароль");
        }
    }
}
//кнопка завершения программы
void DialogService::on_pushButton_clicked(){
    this->data->tempstr = "killall";//попросим вызывавшего нас тоже завершиться
    this->close();
}

void DialogService::on_pushButton_2_clicked(){
    this->data->need_to_save_config = true;
}

void DialogService::on_save_clicked(){


    //попробуем открыть /sys/block/sda
    if (QDir("/sys/block/sda").exists() ){
        //попробуем смонтировать
        qDebug("mount /dev/sda1 /mnt");
        QStringList arguments; QProcess myProcess; QString program;
        program = "mount";
        arguments << "/dev/sda1" << "/mnt";
        myProcess.start(program, arguments);
        myProcess.waitForFinished();

        //создадим папку с номером КНС
        qDebug("mkdir /mnt/KHC_num");
        QDir().mkdir(QString("/mnt/kns_%1").arg(this->data->KNSnumber));
        qDebug("mkdir /mnt/KHC_num/bd");
        QDir().mkdir(QString("/mnt/kns_%1/bd").arg(this->data->KNSnumber));

        dui->progressBar->show();
        //скопируем файлы
        QStringList files = QDir("/nand/qt/").entryList(QStringList() << "*", QDir::Files);
        dui->progressBar->setMaximum(files.size());
        for(int i=0;i<files.size();i++){ // перебираем все эти файлы
            dui->progressBar->setValue(i);
            QStringList arguments; QProcess myProcess; QString program;
            program = "cp";
            arguments << "/nand/qt/"+files[i] << "/mnt/kns_" + this->data->KNSnumber + "/" + files[i];
            qDebug() << "cp" << arguments[0] << arguments[1];
            myProcess.start(program, arguments);
            myProcess.waitForFinished();
        }
        QStringList files2 = QDir("/nand/qt/bd").entryList(QStringList() << "*", QDir::Files);
        dui->progressBar->setMaximum(files2.size());
        for(int i=0;i<files2.size();i++){ // перебираем все эти файлы
            dui->progressBar->setValue(i);
            QStringList arguments; QProcess myProcess; QString program;
            program = "cp";
            arguments << "/nand/qt/bd/"+files2[i] << "/mnt/kns_" + this->data->KNSnumber + "/bd/" + files2[i];
            qDebug() << "cp" << arguments[0] << arguments[1];
            myProcess.start(program, arguments);
            myProcess.waitForFinished();
        }


        //отмонтируем флэшку
        qDebug("umount /mnt");{
        QStringList arguments; QProcess myProcess; QString program;
        program = "umount";
        arguments << "/mnt";
        myProcess.start(program, arguments);
        myProcess.waitForFinished();}

        dui->progressBar->hide();

        QMessageBox msgBox;
        msgBox.setText("USB flash\nможно вынимать");
        msgBox.setWindowTitle("Сохранение");
        msgBox.exec();
    }else{
        QMessageBox msgBox;
        msgBox.setText("USB flash not found");
        msgBox.setWindowTitle("Сохранение");
        msgBox.exec();
        qDebug("/dev/sda not found");
    }
}

void DialogService::on_update_clicked(){
    QFile config_file("rebootflag");
    config_file.open(QIODevice::WriteOnly); // откроем файл для чтения
    config_file.close();

    this->data->tempstr = "killall";//попросим вызывавшего нас тоже завершиться
    this->close();
}
