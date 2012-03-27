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
#include <QString>
#include <sys/mman.h>
#include <termios.h>
#include "mainwindow.h"
#include "thread_485.h"
#include "ui_mainwindow.h"
//время ожидания в мсек
#define ATV12wait 50
#define CE303wait 180

const unsigned char auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
} ;
const unsigned char auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
};
//--------------------------------------------------------------------------------------
    QFile* Posix_File;//handle для UART
unsigned char buffer[255];
unsigned char bytes_to_send;
//int *gpio_base;
void *gpio_base;
bool ce303open = false;
//--------------------------------------------------------------------------------------
//Modbus  CRC Generation Function
unsigned short CRC16_MB (unsigned char *puchMsg, unsigned short usDataLen )  {
  unsigned char uchCRCHi = 0xFF ;   // high byte of CRC initialized
  unsigned char uchCRCLo = 0xFF ;   // low byte of CRC initialized
  unsigned char uIndex ;           // will index into CRC lookup table

  while (usDataLen--){         // pass through message buffer
    uIndex = uchCRCLo ^ *puchMsg++ ;  // calculate the CRC
    uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex] ;
    uchCRCHi = auchCRCLo[uIndex] ;
    }
  buffer[bytes_to_send] = uchCRCLo;  bytes_to_send ++;
  buffer[bytes_to_send] = uchCRCHi;  bytes_to_send ++;
  return (uchCRCHi << 8 | uchCRCLo) ;
}
//--------------------------------------------------------------------------------------
int Thread_485::write_modbus_reg(unsigned char dev_address, unsigned short reg, unsigned short value){
int wrb;
        buffer[0] = dev_address;
        buffer[1] = 0x06;//read holding
        buffer[2] = reg >> 8;         buffer[3] = reg & 0xFF;
        buffer[4] = value >>8;        buffer[5] = value & 0xFF;
        bytes_to_send = 6;
        CRC16_MB(buffer, bytes_to_send);
        if (write(Posix_File->handle(), buffer, bytes_to_send) != bytes_to_send){            return -1;        }
        Posix_File->flush();
        QThread::msleep(ATV12wait);
        wrb = read(Posix_File->handle(), buffer, 250);
        if(wrb == -1){      return -1;       }
        return 0;
}
//--------------------------------------------------------------------------------------
int Thread_485::read_modbus_reg(unsigned char dev_address, unsigned short reg){
int wrb;
        buffer[0] = dev_address;
        buffer[1] = 0x03;//read holding
        buffer[2] = reg >> 8;        buffer[3] = reg & 0xFF;
        buffer[4] = 0x00;
        buffer[5] = 0x01;// registers
        bytes_to_send = 6;
        CRC16_MB(buffer, bytes_to_send);
        //if (
        Posix_File->write((const char *)buffer, bytes_to_send);
        //!= bytes_to_send){             return -1;        }
        Posix_File->flush();
        QThread::msleep(ATV12wait);
buffer[0] = 0;
        wrb = Posix_File->read((char *)buffer, 250);
#if defined (DEBUG)
        QString s = QString("read_modbus_reg: adr %1 reg %2 read %3 %4").arg(dev_address).arg(reg).arg(wrb).arg( ( (unsigned short)buffer[3] <<8 | buffer[4] ) ) ;
        qDebug() << s;
#endif
//        usleep(ATV12wait);

        if(wrb == -1){           return -1;        }
        if(wrb == 0) return -1;
        if(buffer[0] != dev_address || buffer[2] != 0x02) return -1;

        return  ( (unsigned short)buffer[3] <<8 | buffer[4] );
}
//--------------------------------------------------------------------------------------
void Thread_485::set_8N1(void){
struct termios Posix_CommConfig;
    tcgetattr(Posix_File->handle(), &Posix_CommConfig);
    fcntl( Posix_File->handle(), F_SETFL, 0);
    cfsetispeed( &Posix_CommConfig, B19200 );
    cfsetospeed( &Posix_CommConfig, B19200 );
//    cfsetispeed( &Posix_CommConfig, B4800 );
//    cfsetospeed( &Posix_CommConfig, B4800 );
    cfmakeraw( &Posix_CommConfig );
    Posix_CommConfig.c_cflag &= ~( CBAUD | CSTOPB | PARENB | CRTSCTS);
    Posix_CommConfig.c_lflag &= ~( ICANON | ECHO | ECHOE | ISIG);
    Posix_CommConfig.c_cflag |= ( B19200 | CLOCAL | CREAD | CS8);
    //Posix_CommConfig.c_cflag |= ( B4800 | CLOCAL | CREAD | CS8);
    Posix_CommConfig.c_oflag &= ~OPOST;
    Posix_CommConfig.c_cc[VMIN] = 0;
    //Posix_CommConfig.c_cc[VTIME] = 1; // x100 mSec
    Posix_CommConfig.c_cc[VTIME] = 0;
    tcsetattr( Posix_File->handle(), TCSANOW, &Posix_CommConfig );
#if defined(ARM)
    unsigned int temp;
    temp = 3;//CS8
    *(unsigned int*)((uintptr_t)gpio_base + 0x10C) = temp;//LCR
#endif
}
void Thread_485::set_7P1(void){
struct termios Posix_CommConfig;
    tcgetattr(Posix_File->handle(), &Posix_CommConfig);
    fcntl( Posix_File->handle(), F_SETFL, 0);
    cfsetispeed( &Posix_CommConfig, B19200 );
    cfsetospeed( &Posix_CommConfig, B19200 );
    cfmakeraw( &Posix_CommConfig );
    Posix_CommConfig.c_cflag &= ~( CBAUD | CRTSCTS | CSTOPB);//1 stop bit
    Posix_CommConfig.c_lflag &= ~( ICANON | ECHO | ECHOE | ISIG);
    Posix_CommConfig.c_cflag |= ( B19200 | CLOCAL | CREAD | CS7 | PARENB | PARODD/*| CMSPAR*/);//7 bit, parity
    Posix_CommConfig.c_oflag &= ~OPOST;
    Posix_CommConfig.c_cc[VMIN] = 0;
    //Posix_CommConfig.c_cc[VTIME] = 2;
    Posix_CommConfig.c_cc[VTIME] = 0;
    tcsetattr( Posix_File->handle(), TCSANOW, &Posix_CommConfig );

#if defined(ARM)
    unsigned int temp;
    temp = 1<<4 | 1<<3 | 2 | 1<<2;//EPE, PBE, CS7
    *(unsigned int*)( (uintptr_t)gpio_base + 0x10C ) = temp;//LCR
#endif
}
//==============================================================================
// запрос счетчика СЕ303, чтение () и запись (параметр)
int Thread_485::request(const char s_req[100]){
int wrb, temp, s = 4, i = 0;
unsigned char BCC, tempbuf[200], c=0;

//if (ce303open == false) return -1;

    buffer[0] = SOH;
    if(s_req[6] == ')'){
        buffer[1] = 'R';
    }else{
        buffer[1] = 'W';
    }
    buffer[2] = '1';
    buffer[3] = STX;

    while(s_req[i] && i < 100){
        buffer[s++] = s_req[i++];
    }
//qDebug() << QString("CE303: %1").arg(QLatin1String((const char *)buffer));
    buffer[s++] = ETX;
    BCC = 0;    for(temp = 1; temp < s; temp++){BCC = BCC + buffer[temp];}
    buffer[s++] = BCC & 0x7F;
    bytes_to_send = s;
    if (Posix_File->write((const char *)buffer, bytes_to_send) != bytes_to_send){     return -1;    }
    Posix_File->flush();
    QThread::msleep(CE303wait);
    wrb = Posix_File->read((char *)buffer, 250);
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111
  //strcpy((char *)buffer, "220.10\r\n221.22\r\n222.333\r\n"); return 0;
//    qDebug() << QString("CE303: %1 %2").arg(wrb).arg(QLatin1String((const char *)buffer));
    if(wrb == -1){     return -1;    }
    if(wrb == 1 && buffer[0] == 0x06){/*qDebug("ce303:ACK");*/return 0;}
    if(wrb == 1 && buffer[0] == 0x15){/*qDebug()<< QString("ce303: %1 NACK").arg(QLatin1String(s_req));*/return -1;}
    if(wrb < 5){
//        qDebug() << QString("ce303 read %1 bytes (%2) %3 %4 %5 %6 %7").arg(wrb).arg(QLatin1String(s_req)).arg(buffer[0], 0, 16).arg(buffer[1], 0, 16).arg(buffer[2], 0, 16).arg(buffer[3], 0, 16).arg(buffer[4], 0, 16);
        /* printf("answer %d bytes len\r\n", wrb); */  return -1;    }

    temp=0;
    while( (temp < (wrb-2)) && buffer[temp] != ETX ){//крутимся до конца буфера
      while( (temp < (wrb-2)) && buffer[temp++] != '(') ;//промотаем все до (
      while( (temp < (wrb-2)) && buffer[temp] != ')'){
          tempbuf[c++] = buffer[temp++];                // то что внутри () выведем
      }
      tempbuf[c++] = 0x0d;tempbuf[c++] = 0x0a;          // разделим строки
    }
    memcpy(buffer, tempbuf, c);                         // вернем буфер
    buffer[c+1] = 0;
return 0;
}
//--------------------------------------------------------------------------------------
// открыть канал связи к счетчику СЕ303, используем адрес/пароль по умлочанию
int Thread_485::openCE303(void){
    int temp, wrb;
    unsigned char BCC;

    if (ce303open) return 0;

    //open for address
    memcpy(buffer, "/?777777!\r\n", 11);
      Posix_File->write( (const char *)buffer, 11);
      Posix_File->flush();
//      qDebug() << QString("openCE303: write %1").arg(QLatin1String((const char *)buffer));
    QThread::msleep(CE303wait);
buffer[0] = 0;
      wrb = Posix_File->read((char *)buffer, 250);
//qDebug() << QString("openCE303: read %1 %2").arg(wrb).arg(QLatin1String((const char *)buffer));
//if(wrb == 1)qDebug() << QString("openCE303: %1").arg(buffer[0], 0, 16);
      if(wrb == 0 || wrb == -1) return -1;
//	  printf("/?_addr_! %s", buffer);
      buffer[0] = 0x06;
      buffer[1] = '0';
      buffer[2] = '6';	//19200
      //buffer[2] = '5';	//9600
      buffer[3] = '1';	//programming mode
      buffer[4] = '\r';
      buffer[5] = '\n';
      Posix_File->write((const char *)buffer, 6);
      Posix_File->flush();
    QThread::msleep(CE303wait);
buffer[0] = 0;
      wrb = Posix_File->read((char *)buffer, 250);
//      printf("ACK 0 6 1 [%s]\r\n", buffer);
//      s = QString("openCE303: ACK 0 6 1 [%1]").arg(QLatin1String((const char *)buffer));
//      qDebug() << s;
//open password accsess
      buffer[0] = SOH;
      buffer[1] = 'P';
      buffer[2] = '1';
      buffer[3] = STX;
      buffer[4] = '(';
      buffer[5] = '7';
      buffer[6] = '7';
      buffer[7] = '7';
      buffer[8] = '7';
      buffer[9] = '7';
      buffer[10] = '7';
      buffer[11] = ')';
      buffer[12] = ETX;
    BCC = 0;
    for(temp = 1; temp < 13; temp++){BCC = BCC + buffer[temp];}
      buffer[13] = BCC & 0x7F;
      Posix_File->write((const char *)buffer, 14);
      Posix_File->flush();
      QThread::msleep(CE303wait);
      wrb = Posix_File->read((char *)buffer, 250);
//	  printf("P1 [%s]\r\n", buffer);
      ce303open = true;
      //qDebug("CE303 open Ok");
      return 0;
}
//--------------------------------------------------------------------------------------
void Thread_485::closeCE303(void){
    unsigned char wrb;
    buffer[0] = SOH;
    buffer[1] = 'B';//close channel
    //buffer[2] = '0';
    buffer[2] = '1';
    buffer[3] = ETX;
  unsigned char BCC = 0;
  for(int temp = 1; temp < 4; temp++){BCC = BCC + buffer[temp];}
    buffer[13] = BCC & 0x7F;
    Posix_File->write((const char *)buffer, 4);
    Posix_File->flush();
    QThread::msleep(CE303wait);
    wrb = Posix_File->read((char *)buffer, 250);
//    qDebug() << QString("closeCE303: %1 [%2]").arg(wrb).arg(QLatin1String((const char *)buffer));
    //ce303open = false;
    //qDebug("CE303 close");
}
//--------------------------------------------------------------------------------------
Thread_485::Thread_485(MainWindow *wnd) : ui(wnd->ui), wnd(wnd){
}
//--------------------------------------------------------------------------------------
void Thread_485::init() {

    //проинициализируем переменные
    //set_defaults();

wnd->done = true;


    //    wnd->data.freq[0] = 0;

    //read_config();

#if defined(ARM)
    int fd;
    if( ( fd = open("/dev/mem", O_RDWR | O_SYNC) ) == -1 ){
        qDebug("/dev/mem open failed");return;    }
    //gpio_base = (int *)( mmap(0, 4096UL, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0xB8000000) );
    gpio_base =  mmap(0, 4096UL, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0xB8000000);
    if (gpio_base == (void *) -1){
        qDebug("mmap failed"); return;    }
#endif
}//--------------------------------------------------------------------------------------
void Thread_485::run() {
    int ce303offline = 0;
    qDebug() << "Thread_485::run";
// откроем 485-й порт
    Posix_File = new QFile("/dev/ttyS1");
    if (!Posix_File->open(QIODevice::ReadWrite)){
        QMessageBox msgBox;
        msgBox.setText("Error open \r\n/dev/ttyS1");
        msgBox.exec();
        qDebug("error open /dev/ttyS1");
        exit(-1); return;
    }

//если есть частотник - закинем начальные настройки
    if(wnd->isATV12){
        set_8N1();
        for(int temp=1;temp<7;temp++){
            write_modbus_reg(temp, 8401, 2);//CHCF = SEP
            write_modbus_reg(temp, 8413, 164);//FR1 = MDB
            write_modbus_reg(temp, 8423, 10);//CD1 = MDB
    //    write_reg(temp, 7124, 129);//RSF = L1H
        }
    }//*/
/*/закинем настройки
// перед этим нажать два раза кнопку ДСТП, чтобы появилась надпись ENABLE
    if(wnd->isCE303){
        set_7P1();              // переключим режим УАРТа
        if(openCE303() != -1){
            if(request("CONDI()") != -1){ // режим работы счетчика
                QString temp;
                QTextStream t((const char*)buffer);
                temp = t.readLine();
                wnd->data.condi = temp.toFloat();
                wnd->data.condi |= 1<<8;//программирование разрешается без нажатия кнопки
                wnd->data.condi |= 1<<6;//время ответа по интерфейсу не менее 20 мс;
                wnd->data.condi |= 1<<13;//обнулять низя
                char aa[10];
                sprintf(aa, "CONDI(%d)", wnd->data.condi);
                if(request(aa) != -1){ // режим работы счетчика (bit8=1)
                    qDebug("CONDI write - OK!");
                }else{
                    qDebug("CONDI write - ERR!");
                }
             }else{
                qDebug("CONDI read - ERR!");
            }
        }else{qDebug("CE303 open - ERR!");}
    }//*/
    while(wnd->done){
                //fprintf(stderr, ".");//плюнем в консоль
/*-----------
// посчитаем время выполнения
           long now, prev=0;
           QString hour, min, sec, program;
           QStringList arguments;
           QProcess myProcess;
           program = "date";
           arguments << "+%k%n%M%n%S%n%N%n";
           myProcess.start(program, arguments);
           myProcess.waitForFinished();
           hour=myProcess.readLine();
           min=myProcess.readLine();
           sec=myProcess.readLine();
//           nan=myProcess.readLine();
           now=hour.toInt() * 3600 + min.toInt() * 60 + sec.toInt();
//           now *= 1000;
//           now += nan.toInt()/1000000;
           qDebug() << "ttw: " << (now - prev);
           prev = now;
//-----------*/
//---------------------------------------
        //считаем показатели частотников
        if(wnd->isATV12){
             set_8N1();
// по идее, тут надо использовать mutex, чтобы остальные процедуры не получили битые данные
        for(int i = 0; i < wnd->ATV12maxNum; i++){
            //read LFT
            wnd->data.lft[i] = read_modbus_reg(i + 1, 0x1BD1);// i+1 - т.к. цикл с 0 и индексы тоже с 0
            if(wnd->data.lft[i] != OFFLINE){//если не ответил, не будем тратить время
                //read ETA
                wnd->data.eta[i] = read_modbus_reg(i + 1, 0x0C81);
//                qDebug() << QString("atv12:ETA %1 = 0x%2").arg(i+1).arg(wnd->data.eta[i],0,16);
//                qDebug() << QString("atv12:ETA & 0x0F 0x%1").arg(wnd->data.eta[i] & 0x0f,0,16);
                // тут надо выставлять флаги - привод RUN/STOP/ERROR
                if(wnd->data.eta[i] != OFFLINE){
                    if(wnd->data.eta[i] == 0x250 || wnd->data.eta[i] == 0x217){
                        wnd->data.ATV12status[i] = STOP;
                    }else
                    if((wnd->data.eta[i] & 0x3FF) == 0x237){
                        wnd->data.ATV12status[i] = RUN;
                    }else
                    if((wnd->data.eta[i] & 0xF) == 0x08){//Malfunction, fault present
                        wnd->data.ATV12status[i] = ERROR;
                    }else{
                        wnd->data.ATV12status[i] = wnd->data.eta[i];
                    }
                }
                //freq, не понятно зачем читать частоту
                wnd->data.freq[i] = read_modbus_reg(i + 1, 3202);
                //current
                wnd->data.current[i] = read_modbus_reg(i + 1, 3204);
                //qDebug() << QString("atv12:lft %1").arg(wnd->data.lft[i]);
            }else{
                wnd->data.eta[i] = OFFLINE;
                wnd->data.freq[i] = OFFLINE;
                wnd->data.current[i] = OFFLINE;
                wnd->data.ATV12status[i] = OFFLINE;
            }
        }
//а тут отпустить mutex
//пройдемся по кнопкам старт/стоп
        for(int i = 0; i < wnd->ATV12maxNum; i++){
            if( wnd->data.ATV12status[i] == ERROR){
                qDebug() << QString("try to reset error %1").arg(i+1);
                write_modbus_reg(i + 1, 8501, 0x80);//try to fault reset
                //write_modbus_reg(i + 1, 8501, 0x0002);//quick STOP
                write_modbus_reg(i + 1, 8501, 0x0000);//STOP
            }
            //получена команда СТАРТ и мотор остановлен
            if(wnd->data.start[i] && wnd->data.ATV12status[i] == STOP){
                qDebug() << QString("try to START %1").arg(i+1);
                write_modbus_reg(i + 1, 8501, 0x0006);//enable
                write_modbus_reg(i + 1, 8501, 0x0007);//enable
                write_modbus_reg(i + 1, 8501, 0x000F);//enable, forward, RUN asked
            }
            //получена команда СТОП
            if(wnd->data.stop[i]){
                //write_modbus_reg(i + 1, 8501, 0x0002);//quick STOP
                write_modbus_reg(i + 1, 8501, 0x0000);//STOP
            }
            if( wnd->data.freq[i] !=  wnd->data.freq_w[i] && wnd->data.freq[i] != -1){
                qDebug() << QString("new freq %1 at drive %2").arg(wnd->data.freq_w[i]).arg(i+1);
                write_modbus_reg(i + 1, 8502, wnd->data.freq_w[i]);
            }
        }
        //wnd->data.freq[0]++;
        //обновим показания диалога ATV12
        if (NULL != wnd->dlgATV12.get()) {
            emit changeDataATV12();
        }
        }//if(wnd->isATV12){
//---------------------------------------
//считаем показания счетчика
        if(wnd->isCE303){
            set_7P1();              // переключим режим УАРТа
            if(openCE303() != -1){  // первый раз канал открывается нормально
                int ttry = 3;       // читается после открытия - нормально
                while(ttry){        // но потом первые два чтения с оишбкой
                    ttry--;
                    if(request("FCCUR()") != -1){ // коэффициент трансформации по току
                        QString temp;
                        QTextStream t((const char*)buffer);
                        temp = t.readLine();
                        wnd->data.fccur = temp.toFloat();
                break;
                    }else{
                        wnd->data.fccur = -1;
                    }
                }
                if(request("DATE_()") != -1){ // текущая дата
                    QTextStream t((const char*)buffer + 3);
                    wnd->data.date_ = t.readLine();
//                    qDebug() << QString("CE303.DATE(): %1").arg(QLatin1String((const char *)buffer));
                }else{
                    wnd->data.date_ = "--";
                    ce303offline++;
                }
                if(request("TIME_()") != -1){ // текущее время
                    QTextStream t((const char*)buffer);
                    wnd->data.time_ = t.readLine();
                }else{
                    wnd->data.time_ = "--";
                    ce303offline++;
                }
                if(request("CONDI()") != -1){ // режим работы счетчика (bit8=1)
//                    qDebug() << QString("CE303.CONDI(): %1").arg(QLatin1String((const char *)buffer));
                    QString temp;
                    QTextStream t((const char*)buffer);
                    temp = t.readLine();
                    wnd->data.condi = temp.toFloat();
                    //qDebug() << QString("CE303.CONDI(): 0x%1").arg(wnd->data.condi, 0, 16);
                }else{
                    wnd->data.condi = -1;
                    ce303offline++;
                }
                if(request("POWEP()") != -1){ // суммарная мощность активная
                    QString temp;
                    QTextStream t((const char*)buffer);
                    temp = t.readLine();
                    wnd->data.powep = temp.toFloat();
                }else{
                    wnd->data.powep = -1;
                    ce303offline++;
                }
                if(request("CURRE()") != -1){ // ток по трём фазам
                    QString temp;
                    QTextStream t((const char*)buffer);
                    temp = t.readLine();
                    wnd->data.curra = temp.toFloat();
                    temp = t.readLine();
                    wnd->data.currb = temp.toFloat();
                    temp = t.readLine();
                    wnd->data.currc = temp.toFloat();
                }else{
                    wnd->data.curra = wnd->data.currb = wnd->data.currc = -1;
                    ce303offline++;
                }
                if(request("COS_f()") != -1){ // суммарный коэффициент мощности
                    QString temp;
                    QTextStream t((const char*)buffer);
                    temp = t.readLine();
                    wnd->data.cos_f = temp.toFloat();
                }else{
                    wnd->data.cos_f = -1;
                    ce303offline++;
                }
                if(request("ET0PE()") != -1){ // энергия с момента обнуления, активная, потребленная
                    QString temp;
                    QTextStream t((const char*)buffer);
                    temp = t.readLine();
                    wnd->data.et0pe = temp.toFloat();
                }else{
                    wnd->data.et0pe = -1;
                    ce303offline++;
                }
                if(request("POWEQ()") != -1){ // суммарная мощность реактивная
                    QString temp;
                    QTextStream t((const char*)buffer);
                    temp = t.readLine();
                    wnd->data.poweq = temp.toFloat();
                }else{
                    wnd->data.poweq = -1;
                    ce303offline++;
                }
                if(request("CORUU()") != -1){ // углы
                    QString temp;
                    QTextStream t((const char*)buffer);
                    temp = t.readLine();
                    wnd->data.angleAB = temp.toFloat();
                    temp = t.readLine();
                    wnd->data.angleBC = temp.toFloat();
                    temp = t.readLine();
                    wnd->data.angleCA = temp.toFloat();
                }else{
                    wnd->data.angleAB = wnd->data.angleBC = wnd->data.angleCA = -1;
                    ce303offline++;
                }
                if(request("VOLTA()") != -1){ // напряжение на трёх фазах
                    QString temp;
                    QTextStream t((const char*)buffer);
//                    bool ok;
                    temp = t.readLine();
//                    qDebug() << QString("CE303.VOLTA(): %1").arg(QLatin1String((const char *)buffer));
//                    qDebug() << "temp:" << temp;
                    //wnd->data.volta = temp.toFloat(&ok);
                    wnd->data.volta = temp.toFloat();
                    temp = t.readLine();
                    wnd->data.voltb = temp.toFloat();
                    temp = t.readLine();
                    wnd->data.voltc = temp.toFloat();
//                    qDebug() << QString("volta %1").arg(wnd->data.volta);
//                    if(ok)qDebug("Ok");
                }else{
                    wnd->data.volta = wnd->data.voltb = wnd->data.voltb = -1;
                    ce303offline++;
                }
                //--------------------------------------------------
                if(wnd->data.CE303needupdate == update_time){
                    char aa[20];
                    QByteArray ar = wnd->data.tempstr.toAscii();
                    sprintf(aa, "TIME_(%s)", (char*)(ar.data() ));
                    //преобразуем точку в двоеточие
                    aa[8] = aa[11] = ':';
                    int t = request(aa);
                    if(t == 0 || t == 6 ){
                        qDebug()<<QString("%1 write - OK!").arg(aa);
                    }
                    if(t == 0x15){qDebug()<<QString("TIME_ write - NACK");}
                    if(t == -1){
                        qDebug("TIME_ write - ERR!");
                        QMessageBox msgBox;
                        msgBox.setText("Error write\nTIME_ command");
                        msgBox.exec();
                    }

                    wnd->data.CE303needupdate = update_none;
                }
                //--------------------------------------------------
                if(wnd->data.CE303needupdate == update_date){
                    char aa[20];
                    QByteArray ar = wnd->data.tempstr.toAscii();
                    sprintf(aa, "DATE_(%s)", (char*)(ar.data() ));
                    int t = request(aa);
                    if(t == 0 || t == 6 ){
                        qDebug()<<QString("%1 write - OK!").arg(aa);
                    }
                    if(t == 0x15){
                        qDebug()<<QString("DATE_ write - NACK");
                        QMessageBox msgBox;
                        msgBox.setText("NACK\nDATE_ command");
                        msgBox.exec();
}
                    if(t == -1){
                        qDebug("DATE_ write - ERR!");
                        QMessageBox msgBox;
                        msgBox.setText("Error write\nDATE_ command");
                        msgBox.exec();
                    }

                    wnd->data.CE303needupdate = update_none;
                }
                //--------------------------------------------------
                if(wnd->data.CE303needupdate == update_trans){
                    char aa[20];
                    QByteArray ar = wnd->data.tempstr.toAscii();
                    sprintf(aa, "FCCUR(%s)", (char*)(ar.data() ));
                    int t = request(aa);
                    if(t == 0 || t == 6 ){
                        qDebug()<<QString("%1 write - OK!").arg(aa);
                    }
                    if(t == 0x15){
                        qDebug()<<QString("FCCUR write - NACK");
                        QMessageBox msgBox;
                        msgBox.setText("NACK\nFCCUR command");
                        msgBox.exec();
}
                    if(t == -1){
                        qDebug("FCCUR write - ERR!");
                        QMessageBox msgBox;
                        msgBox.setText("Error write\nFCCUR command");
                        msgBox.exec();
                    }

                    wnd->data.CE303needupdate = update_none;
                }
                //closeCE303();
            }//if(openCE303() != -1)
            if(ce303offline > 5){
                ce303open = false;
                ce303offline = 0;
                qDebug("CE303 goes offline, try open it. wait some time");// после отвала се303 тупит пару минут
            }
            //обновим показания диалога CE303
            //if (NULL != wnd->dlgCE303.get()) {
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            if(!wnd->data.dlgCE303isVisible){
                emit changeDataCE303();
            }
        }//if(wnd->isCE303){
//--------------------------
        else{//вместо работы со счетчиком немного потупим.
            //usleep(200000);
            QThread::msleep(100);
            //QThread::msleep(2000);
        }
    }//end while(done)
//qDebug() << "Thread_485::done";
//power OFF all motors before exit programm
    write_modbus_reg(1, 8501, 0);   write_modbus_reg(2, 8501, 0);
    write_modbus_reg(3, 8501, 0);   write_modbus_reg(4, 8501, 0);
    write_modbus_reg(5, 8501, 0);   write_modbus_reg(6, 8501, 0);

    Posix_File->close();// закроем порт перед выходом
//qDebug() << "Thread_485::Posix_File->close()";
}
//===============================================================================
void Thread_485::quit(){
    wnd->done = false;// переменная для завершения цикла опроса
    qDebug() << "Thread_485::quit";
}
