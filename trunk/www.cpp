#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>          //is defined open and close
#include <unistd.h>         //is defined read and write
#include <QDebug>
#include <QtNetwork>
#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QHostAddress>
#include "www.h"
#include "mainwindow.h"
#include "data_struct.h"


FortuneServer::FortuneServer(data_struct * dataStruct, QObject *parent)
    : QTcpServer(parent),
      data(dataStruct) {

}

void FortuneServer::readClient(){

    // This slot is called when the client sent data to the server. The
    // server looks if it was a get request and sends a very simple HTML
    // document back.
    QTcpSocket* socket = (QTcpSocket*)sender();
    if (socket->canReadLine()) {//если можно что-то получить от сокета
        //получим кучу строковых параметров
        QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
        //интересует только метод GET
        if (tokens[0] == "GET") {
            //qDebug() << "QStringList ---------";
            //покажем эти строчки
            //for (QList<QString>::iterator i = tokens.begin(); i != tokens.end(); ++i){qDebug() << (*i); }

            //покажем ответ браузеру

            //ёбана..... прийдется еще и картинки самому обрабатывать чтоле?
            if ( tokens[1].contains(".gif") ){
/* не работает
                // To load a QImage
                QImage image;        // See the documentation on how to use QImage objects
                if(image.load("pict/pult_fon.gif", "GIF") == false){qDebug() << "ERROR image.load";}
                // To set-up the socket
                //socket->connectToHost(address, port, QIODevice::WriteOnly);
                QByteArray ba;              // Construct a QByteArray object
                QBuffer buffer(&ba);        // Construct a QBuffer object using the QbyteArray
                image.save(&buffer, "GIF"); // Save the QImage data into the QBuffer
                socket->write(ba);          // Send the QBuffer (QbyteArray) over a socket
*/
                QFile in; in.setFileName(QString("%1").arg(tokens[1].remove(0,1)));
                in.open(QFile::ReadOnly); QByteArray imageBytes = in.readAll();
                socket->write(imageBytes);
            }else{
            //будем считать, что тут точно просят текст
                //посканировать на предмет наличия нужных переменных метода GET


                //тут запихать state машину для генерации странички
                //main page
                if (tokens[1] == "/"){/*
                    os << "<h1>Nothing to see here</h1>\n"
                      << QDateTime::currentDateTime().toString() << "<BR>\n";
                    os << "<table bordercolor=\"red\" border=\"1\" background=\"pict/pult_fon.gif\"width=\"480\" height=\"272\">\n"
                          "<tr height=\"20\">\n";
                    os << tr("<td width=\"100\"></td>\n"
                             "<td width=\"100\"><form><button name=\"page\" formaction=\"\" formmethod=\"GET\" type=\"submit\" value=\"setup\">Установки</button></form></td>\n"
                             "<td width=\"100\"><form><button name=\"page\" formaction=\"\" formmethod=\"GET\" type=\"submit\" value=\"service\">Сервис</button></form></td>\n"
                             "<td></td>\n");
                    os << "</tr>"
                          "<tr>"
                            "<td><a href=\"/?page=alarm\"><img src=\"pict/log_avarij_red.gif\"></a></td> <td>...</td>"
                          "</tr>"
                          "</table>"
                      "<BR>\n";
                    os << tr("<p><a href=\"/?page=alarm\">Как сделать такое же фото?</a></p>\n");*/
                    main_window(socket);
                }
                //================================================================================================
                //alarm page
                if ( tokens[1].contains("page=alarm") ){
                    alarm_window(socket);
                }
                //setup page
                if ( tokens[1].contains("page=setup") ){
                    setup_window(socket);
                }
                //service page
                if ( tokens[1].contains("page=service") ){
                    //os << tr("service page<p><a href=\"/\">Back to main</a></p>");
                }
            }
            //закроем соединение
            socket->close();

            if (socket->state() == QTcpSocket::UnconnectedState) {
                delete socket;
            }
        }//if (tokens[0] == "GET") {
        else{
            qDebug() << "unknown " << tokens[0];
            for (QList<QString>::iterator i = tokens.begin(); i != tokens.end(); ++i){qDebug() << (*i); }
        }
    }
}
void FortuneServer::incomingConnection(int socketDescriptor){
    // When a new client connects, the server constructs a QTcpSocket and all
    // communication with the client is done over this QTcpSocket. QTcpSocket
    // works asynchronously, this means that all the communication is done
    // in the two slots readClient() and discardClient().
    QTcpSocket* s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
    s->setSocketDescriptor(socketDescriptor);
}
void FortuneServer::discardClient(){
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
}
//=================================================================================================================
void FortuneServer::setup_window(QTcpSocket* socket){
    QTextStream os(socket);
    os.setAutoDetectUnicode(true);
    os << "HTTP/1.0 200 Ok\r\n"
        "Content-Type: text/html; charset=\"utf-8\"\r\n"
        "\r\n";

os << tr("<HTML><title>Установки</title><BODY text=ffffff alink=ffffff link=ffffff vlink=ffffff>");
os << tr("<style href>a {text-decoration: none} </style>");
os << tr("<style type=text/css>.red {color: #ff0000; font-size: 11px;} </style>");
os << tr("<style type=text/css>td.ramka {border: 1px solid #247eae; font-size: 12px;} </style>");
os << tr("<style type=text/css>table{font-size: 12px; text-align: center; vertical-align: middle; border-spacing: 0; padding: 0} </style>");
os << tr("<style type=text/css>table.menu {font-size: 15px; background-color: #ffffff; border-spacing: 1; padding: 0} </style>");
os << tr("<style type=text/css>td.right{text-align: right} </style>");
os << tr("<style type=text/css>form {margin: 0;} </style>");
os << tr("<style type=text/css>input {background-image: url(/pict/knopka_mini.gif); border: 0px; cursor: pointer; width: 30px; height: 17px; font-size: 12px; color: #ffffff; text-align: center;} </style>");

os << tr("<table width=480 height=29 cellspacing=1 class=menu>");
 os << tr("<tr>");
  os << tr("<td background=/bmp/knopka_mini.gif width=100><a href=\"/\">главная</a></td>");
  os << tr("<td width=100 bgcolor=000000>установки</td>");
  os << tr("<td bgcolor=000000 width=280></td>");
 os << tr("</tr>");
os << tr("</table>");

 os << tr("<table width=480 height=245 background=/bmp/ustanovki_fon.gif cecllpadding=0 cellspacing=0>");
 os << tr("<tr>");
  os << tr("<td width=130 height=20></td>");
  os << tr("<td width=32></td>");
  os << tr("<td width=70></td>");
  os << tr("<td width=30></td>");
  os << tr("<td width=50></td>");
  os << tr("<td width=30></td>");
  os << tr("<td></td>");
 os << tr("</tr>");
 os << tr("<tr>");
  os << tr("<td width=130 height=17></td>");
  os << tr("<td width=32></td>");
  os << tr("<td width=70></td>");
  os << tr("<td width=30 class=ramka>");
  if(this->data->servicemode){
      os << tr("<a href=\"/cgi-bin/ustanovki_izmen.cgi?tip=diametr\" title=\"диаметр_ёмкости\">\"$diametr\"</a></td>");
  }else{
    os << tr("\"$diametr\"</td>");
  }
  os << tr("<td width=50></td>");
  os << tr("<td width=30></td>");
  os << tr("<td></td>");
 os << tr("</tr>");
 os << tr("<tr>");
  os << tr("<td width=130 height=17></td>");
  os << tr("<td width=32></td>");
  os << tr("<td width=70></td>");
  os << tr("<td width=30></td>");
  os << tr("<td width=50></td>");
  os << tr("<td width=30 class=ramka>");
  if(this->data->servicemode){
   os << tr("<a href=\"/cgi-bin/ustanovki_izmen.cgi?tip=nomer_nasosa1\">\"$nomer_nasosa1\"</a></td>");
  }else{
   os << tr("$nomer_nasosa1</td>");
  }
  os << tr("<td></td>");
 os << tr("</tr>");
os << tr("<tr>");
  os << tr("<td width=130 height=17></td>");
  os << tr("<td width=32 class=ramka>");
  if(this->data->servicemode){
   os << tr("<a href=\"/cgi-bin/ustanovki_izmen.cgi?tip=uroven4\" title=\"уровень_четвёртого_поплавка\">$uroven4</a></td>");
  }else{
   os << tr("$uroven4</td>");
  }
  os << tr("<td width=70></td>");
  os << tr("<td width=30></td>");
  os << tr("<td width=50></td>");
  os << tr("<td width=30></td>");
  os << tr("<td></td>");
 os << tr("</tr>");
 os << tr("<tr>");
  os << tr("<td width=130 height=8></td>");
  os << tr("<td width=32></td>");
  os << tr("<td width=70></td>");
  os << tr("<td width=30></td>");
  os << tr("<td width=50></td>");
  os << tr("<td width=30></td>");
  os << tr("<td></td>");
 os << tr("</tr>");
 os << tr("<tr>");
  os << tr("<td width=130 height=17></td>");
  os << tr("<td width=32 class=ramka>");
  if(this->data->servicemode){
   os << tr("<a href=\"/cgi-bin/ustanovki_izmen.cgi?tip=uroven3\" title=\"уровень_третьего_поплавка\">$uroven3</a></td>");
  }else{
   os << tr("$uroven3</td>");
  }
  os << tr("<td width=70></td>");
  os << tr("<td width=30></td>");
  os << tr("<td width=50></td>");
  os << tr("<td width=30 class=ramka>");
  if(this->data->servicemode){
   os << tr("<a href=\"/cgi-bin/ustanovki_izmen.cgi?tip=nomer_nasosa2\">$nomer_nasosa2</a></td>");
  }else{
   os << tr("$nomer_nasosa2</td>");
  }
  os << tr("<td></td>");
 os << tr("</tr>");
 os << tr("<tr>");
  os << tr("<td width=130 height=8></td>");
  os << tr("<td width=32></td>");
  os << tr("<td width=70></td>");
  os << tr("<td width=30></td>");
  os << tr("<td width=50></td>");
  os << tr("<td width=30></td>");
  os << tr("<td></td>");
 os << tr("</tr>");
 os << tr("<tr>");
  os << tr("<td width=130 height=17></td>");
  os << tr("<td width=32 class=ramka>");
  if(this->data->servicemode){
   os << tr("<a href=\"/cgi-bin/ustanovki_izmen.cgi?tip=uroven2\" title=\"уровень_второго_поплавка\">$uroven2</a></td>");
  }else{
   os << tr("$uroven2</td>");
  }
  os << tr("<td width=70></td>");
  os << tr("<td width=30></td>");
  os << tr("<td width=50></td>");
  os << tr("<td width=30></td>");
  os << tr("<td></td>");
 os << tr("</tr>");
 os << tr("<tr>");
  os << tr("<td width=130 height=8></td>");
  os << tr("<td width=32></td>");
  os << tr("<td width=70></td>");
  os << tr("<td width=30></td>");
  os << tr("<td width=50></td>");
  os << tr("<td width=30></td>");
  os << tr("<td></td>");
 os << tr("</tr>");
os << tr("<tr>");
  os << tr("<td width=130 height=17></td>");
  os << tr("<td width=32></form></td>");
  os << tr("<td width=70></td>");
  os << tr("<td width=30></td>");
  os << tr("<td width=50></td>");
  os << tr("<td width=30 class=ramka>");
  if(this->data->servicemode){
   os << tr("<a href=\"/cgi-bin/ustanovki_izmen.cgi?tip=nomer_nasosa3\">$nomer_nasosa3</a></td>");
  }else{
   os << tr("$nomer_nasosa3</td>");
  }
  os << tr("<td></td>");
 os << tr("</tr>");
os << tr("<tr>");
  os << tr("<td width=130 height=25></td>");
  os << tr("<td width=32></td>");
  os << tr("<td width=70></td>");
  os << tr("<td width=30></td>");
  os << tr("<td width=50></td>");
  os << tr("<td width=30></td>");
  os << tr("<td></td>");
 os << tr("</tr>");
os << tr("<tr>");
  os << tr("<td width=130 height=17></td>");
  os << tr("<td width=32 class=ramka>");
  if(this->data->servicemode){
   os << tr("<a href=\"/cgi-bin/ustanovki_izmen.cgi?tip=uroven1\" title=\"уровень_первого_поплавка\">$uroven1</a></td>");
  }else{
   os << tr("$uroven1</td>");
  }
  os << tr("<td width=70></td>");  os << tr("<td width=30></td>");
  os << tr("<td width=50></td>");  os << tr("<td width=30 class=ramka>");
  if(this->data->servicemode){
    os << tr("<a href=\"/cgi-bin/ustanovki_izmen.cgi?tip=nomer_nasosa4\">$nomer_nasosa4</a></td>");
  }else{
  os << tr("$nomer_nasosa4</td>");
}
  os << tr("<td></td>"); os << tr("</tr>");
os << tr("<tr>");
  os << tr("<td width=130 height=8></td>");  os << tr("<td width=32></td>");
  os << tr("<td width=70></td>");  os << tr("<td width=30></td>");  os << tr("<td width=50></td>");  os << tr("<td width=30></td>");
  os << tr("<td></td>"); os << tr("</tr>");
os << tr("<tr>");
  os << tr("<td width=130 height=17></td>");
  os << tr("<td width=32 class=ramka>");
  if(this->data->servicemode){
   os << tr("<a href=\"/cgi-bin/ustanovki_izmen.cgi?tip=uroven_ulitki\" title=\"уровень_улитки\">$uroven_ulitki</a></td>");
}else{
   os << tr("$uroven_ulitki</td>");
  }
  os << tr("<td width=70 class=right>Итого:&nbsp</td>");
  os << tr("<td width=30 class=ramka>");
  if(this->data->servicemode){
   os << tr("<a href=/cgi-bin/ustanovki_izmen.cgi?tip=nasosy title=\"общее_количество_насосов_на_станции\">$nasosy</a></td>");
}else{
   os << tr("$nasosy</td>");
  }
  os << tr("<td width=50>&nbspнасоса</td>");
  os << tr("<td width=30></td>");
  os << tr("<td></td>");
 os << tr("</tr>");
 os << tr("<tr>");
  os << tr("<td width=130 height=17></td>");
  os << tr("<td width=32></td>");
  os << tr("<td width=70></td>");
  os << tr("<td width=30></td>");
  os << tr("<td width=50></td>");
  os << tr("<td width=30 class=ramka></td>");
  os << tr("<td>&nbsp</td>");
 os << tr("</tr>");
os << tr("<tr>");
  os << tr("<td width=130></td>");
  os << tr("<td width=32></td>");
  os << tr("<td width=70></td>");
  os << tr("<td width=30></td>");
  os << tr("<td width=50></td>");
  os << tr("<td width=30></td>");
  os << tr("<td></td>");
 os << tr("</tr>");
os << tr("</table>");

os << tr("<table width=480 height=29 cellpadding=0 cellspacing=1 class=menu>");
 os << tr("<tr>");
  os << tr("<td width=100 bgcolor=000000>колодец</a></td>");
  os << tr("<td width=100 background=/bmp/knopka_mini.gif><a href=/cgi-bin/ustanovki_obschie.cgi>общие</a></td>");
  os << tr("<td width=100 background=/bmp/knopka_mini.gif><a href=/cgi-bin/ustanovki_set.cgi>сетевые</a></td>");
  os << tr("<td width=180 bgcolor=000000 class=red>");
  ///{   read perezagruzka
  //} < /nand/www/cgi-bin/perezagruzka
  //if [ "$perezagruzka" == 1 ]  then
   //os << tr("! Изменения вступят в силу после перезагрузки !"
  //fi
  os << tr("</td>");
os << tr("</tr></table>");
os << tr("</BODY></HTML>");
}
//=================================================================================================================
void FortuneServer::alarm_window(QTcpSocket* socket){
    QTextStream os(socket);
    os.setAutoDetectUnicode(true);
    os << "HTTP/1.0 200 Ok\r\n"
        "Content-Type: text/html; charset=\"utf-8\"\r\n"
        "\r\n";

    os << tr("<HTML><title>Лог аварий</title><BODY text=ffffff alink=ffffff link=ffffff vlink=ffffff>");
    os << tr("<style type=text/css>.cursor {cursor: pointer} </style>");
    os << tr("<style href>a {text-decoration: none} </style>");
    os << tr("<style type=text/css>table {font-size: 15px; text-align: center; vertical-align: middle; border-spacing: 0; padding: 0} </style>");
    os << tr("<style type=text/css>table.menu {font-size: 15px; border-spacing: 1; background-color: #ffffff} </style>");
    os << tr("<style type=text/css>td.ramka {border: 1px solid #ffffff} </style>");
    os << tr("<style type=text/css>textarea {font-size: 12px; color: #ffffff; background-color: #000000} </style>");
    os << tr("<table width=480 height=29 cellpadding=0 cellspacing=1 class=menu>");
     os << tr("<tr>");
      os << tr("<td width=100 background=/bmp/knopka_mini.gif><a href=\"/\">главная</a></td>");
      os << tr("<td width=100 background=/bmp/knopka_mini.gif><a href=\"/cgi-bin/ustanovki.cgi\">установки</a></td>");
      os << tr("<td width=280 bgcolor=000000></td>");
     os << tr("</tr>");
    os << tr("</table>");

    os << tr("<table width=480 height=200 bgcolor=000000 cellpadding=0 cellspacing=0>");
     os << tr("<tr>");
      os << tr("<td height=40 colspan=3>Лог аварий</td>");
     os << tr("</tr>");
     os << tr("<tr>");
      //os << tr("<td colspan=3><textarea rows=6 cols=50 readonly>");
     os << tr("<td colspan=3><textarea rows=10 cols=60 readonly>");
      QFile config_file("messages.log");// имя  файла
      QTextStream config_in(&config_file);// создадим поток для чтения
      QString config_line;
      int c=0;
    if( config_file.open(QIODevice::ReadOnly) ){ // откроем файл для чтения
  do {
        // берем очередную строку из файла
        config_line = config_in.readLine();
//        if(config_line.contains("EE")){
//            os << "<font color=FF8080>";
//            os << config_line; os << "\n";
//            os << "</font>";
//        }else{
          os << config_line; os << "\n";
//        }
        c++;
      } while (!config_line.isNull() && c<100);             // последней строки достигли в файле
    config_file.close();                           // закроем конфигурационный файл
  }
      os << tr("</textarea></td>");
     os << tr("</tr>");
     os << tr("<tr>");
      os << tr("<td height=10 colspan=3></td>");
     os << tr("</tr>");
     os << tr("<tr>");
      os << tr("<td width=170 height=29></td>");
      if(this->data->servicemode){
        os << tr("<td width=140 background=/bmp/knopka_menu.gif class=ramka>");
        os << tr("<a href=\"/cgi-bin/log_avarij.cgi?parametr=udalit\">удалить записи</a></td>");
      }else{
        os << tr("<td width=140></td>");
      }
      os << tr("<td width=170></td>");
     os << tr("</tr>");
     os << tr("<tr>");
      os << tr("<td height=10 colspan=3></td>");
     os << tr("</tr>");
     os << tr("</table>");

    os << tr("<table width=480 height=29 cellpadding=0 cellspacing=1 class=menu>");
     os << tr("<tr>");
      os << tr("<td width=480 bgcolor=000000></td>");
    os << tr("</tr></table>");

    os << tr("</BODY></HTML>");
}
//=================================================================================================================
void FortuneServer::main_window(QTcpSocket* socket){
    QTextStream os(socket);
    os.setAutoDetectUnicode(true);
    os << "HTTP/1.0 200 Ok\r\n"
        "Content-Type: text/html; charset=\"utf-8\"\r\n"
        "\r\n";

    os << tr("<HTML><title>Главная</title><BODY text=\"ffffff\" alink=\"ffffff\" link=\"ffffff\" vlink=\"ffffff\">");
    os << tr("<style href>a {text-decoration: none} </style>");
    os << tr("<style type=\"text/css\">form {margin: 0;} </style>");
    os << tr("<style type=\"text/css\">.red {color: #ff0000;} </style>");
    os << tr("<style type=\"text/css\">.smallgreen {font-size: 11px; color: #00ff00;} </style>");
    os << tr("<style type=\"text/css\">.red {color: #ff0000;} </style>");
    os << tr("<style type=\"text/css\">table.menu {font-size: 15px; text-align: center; vertical-align: middle; border-spacing: 1; background-color: #ffffff} </style>");
    os << tr("<style type=\"text/css\">table {border-spacing: 0; padding: 0} </style>");
    os << tr("<meta http-equiv=\"refresh\" content=5>");

    os << tr("<table width=\"480\" height=\"29\" cellpadding=\"0\" cellspacing=\"1\" class=\"menu\">");
     os << tr("<tr>");
      os << tr("<td bgcolor=\"000000\" width=\"100\">главная</td>");
      os << tr("<td width=\"100\" background=/bmp/knopka_mini.gif><a href=\"page=setup\">установки<a></td>");
      //if [ "$cookie_parol" == "$parol_service" ]
      //then
      os << tr("<td width=\"100\" background=/bmp/knopka_mini.gif><a href=\"page=service\">сервис</a></td>");
      os << tr("<td bgcolor=\"000000\" width=\"180\"></td>");
      //else
      //os << tr("<td bgcolor=\"000000\" width=\"280\"></td>");
      //fi
     os << tr("</tr>");
    os << tr("</table>");

    os << tr("<table width=\"480\" height=\"222\" cellpadding=\"0\" cellspacing=\"0\" bgcolor=\"000000\">");
     os << tr("<tr width=\"480\" height=\"7\"><td></td></tr>");
     os << tr("<tr><td>");

       os << tr("<table width=\"232\" height=\"222\" cellpadding=\"0\" cellspacing=\"0\" bgcolor=\"000000\">");
        os << tr("<tr>");
         os << tr("<td align=\"center\" valign=\"middle\" width=\"70\">");
          os << tr("<a href=\"/cgi-bin/fazy_set.cgi\"><img src=/bmp/setred.gif border=\"0\"></a></td>");
         os << tr("<td rowspan=\"4\" width=\"4\"></td>");
            //покажем насос №3
         os << tr("<td rowspan=2 align=center valign=middle>");
         os << tr("<form action=\"nasos.cgi?nomer\" method=get>");
         os << tr("<input type=hidden name=nomer value=3>");
         switch(this->data->nasos[3-1]){
         case 0://нет насоса
             os << tr("<input type=image src=/bmp/nasosgray_netu.gif border=0>3</form>");
             break;
         case 1:// не включен, готов
             os << tr("<input type=image src=/pict/nasosgray.gif border=0>3</form>");
             break;
         case 2://включен, работает
             os << tr("<input type=image src=/pict/nasosgreen.gif border=0>3</form>");
             int h,m,s;
             m = this->data->nasos_time_to_overtime[3-1] / 60;
             s = this->data->nasos_time_to_overtime[3-1] - (m * 60);
             h = m / 24;
             m = m - (h * 24);
             if(this->data->isATV12){
                os << QString("<span class=smallgreen>%1:%2:%3\n%4Hz\n%5A</span></td>").arg(h).arg(m).arg(s).arg((double)this->data->freq[3-1]/10).arg((double)this->data->current[3-1]/10) ;
             }else
             if(this->data->isSoftStart){
                 os << QString("<span class=smallgreen>%1:%2:%3\n%4A</span></td>").arg(h).arg(m).arg(s).arg(this->data->current[3-1]) ;
             }else{
                os << QString("<span class=smallgreen>%1:%2:%3</span></td>").arg(h).arg(m).arg(s) ;
             }
             break;
         case 3://авария по току
             os << tr("<input type=image src=/pict/nasosred_a.gif border=0>3</form>");
             break;
         case 4://авария по температуре
             os << tr("<input type=image src=/pict/nasosred_t.gif border=0>3</form>");
             break;
         case 5://авария по влажности
             os << tr("<input type=image src=/pict/nasosred_vl.gif border=0>3</form>");
             break;
         case 6://авария по обрыву
             os << tr("<input type=image src=/pict/nasosred_k.gif border=0>3</form>");
             break;
         }
         //покажем насос №4
      os << tr("<td rowspan=2 align=center valign=middle>");
      os << tr("<form action=\"nasos.cgi?nomer\" method=get>");
      os << tr("<input type=hidden name=nomer value=4>");
         switch(this->data->nasos[4-1]){
         case 0://нет насоса
             os << tr("<input type=image src=/bmp/nasosgray_netu.gif border=0>4</form>");
             break;
         case 1:// не включен, готов
             os << tr("<input type=image src=/pict/nasosgray.gif border=0>4</form>");
             break;
         case 2://включен, работает
             os << tr("<input type=image src=/pict/nasosgreen.gif border=0>4</form>");
             int h,m,s;
             m = this->data->nasos_time_to_overtime[4-1] / 60;
             s = this->data->nasos_time_to_overtime[4-1] - (m * 60);
             h = m / 24;
             m = m - (h * 24);
             if(this->data->isATV12){
                os << QString("<span class=smallgreen>%1:%2:%3\n%4Hz\n%5A</span></td>").arg(h).arg(m).arg(s).arg((double)this->data->freq[4-1]/10).arg((double)this->data->current[4-1]/10) ;
             }else
             if(this->data->isSoftStart){
                 os << QString("<span class=smallgreen>%1:%2:%3\n%4A</span></td>").arg(h).arg(m).arg(s).arg(this->data->current[4-1]) ;
             }else{
                os << QString("<span class=smallgreen>%1:%2:%3</span></td>").arg(h).arg(m).arg(s) ;
             }
             break;
         case 3://авария по току
             os << tr("<input type=image src=/pict/nasosred_a.gif border=0>4</form>");
             break;
         case 4://авария по температуре
             os << tr("<input type=image src=/pict/nasosred_t.gif border=0>4</form>");
             break;
         case 5://авария по влажности
             os << tr("<input type=image src=/pict/nasosred_vl.gif border=0>4</form>");
             break;
         case 6://авария по обрыву
             os << tr("<input type=image src=/pict/nasosred_k.gif border=0>4</form>");
             break;
         }

        os << tr("</tr>");
        os << tr("<tr>");
         os << tr("<td align=center valign=middle width=70>");
         if(this->data->volta == -1 || this->data->angleAB == -1 || this->data->et0pe == -1)
          os << tr("<a href=\"/cgi-bin/schetchik.cgi\"><img src=/pict/schetchik_red.gif border=0></a></td>");
         else
          os << tr("<a href=\"/cgi-bin/schetchik.cgi\"><img src=/pict/schetchik_green.gif border=0></a></td>");
        os << tr("</tr>");
        os << tr("<tr>");
         os << tr("<td align=center valign=middle width=70><a href=\"page=alarm\" title=\"лог аварий\">");
          os << tr("<img src=/pict/log_avarij_red.gif border=0></a></td>");
          //покажем насос №1
       os << tr("<td rowspan=2 align=center valign=middle>");
       os << tr("<form action=\"nasos.cgi?nomer\" method=get>");
       os << tr("<input type=hidden name=nomer value=1>");
       switch(this->data->nasos[1-1]){
       case 0://нет насоса
           os << tr("<input type=image src=/bmp/nasosgray_netu.gif border=0>1</form>");
           break;
       case 1:// не включен, готов
           os << tr("<input type=image src=/pict/nasosgray.gif border=0>1</form>");
           break;
       case 2://включен, работает
           os << tr("<input type=image src=/pict/nasosgreen.gif border=0>1</form>");
           int h,m,s;
           m = this->data->nasos_time_to_overtime[1-1] / 60;
           s = this->data->nasos_time_to_overtime[1-1] - (m * 60);
           h = m / 24;
           m = m - (h * 24);
           if(this->data->isATV12){
              os << QString("<span class=smallgreen>%1:%2:%3\n%4Hz\n%5A</span></td>").arg(h).arg(m).arg(s).arg((double)this->data->freq[1-1]/10).arg((double)this->data->current[1-1]/10) ;
           }else
           if(this->data->isSoftStart){
               os << QString("<span class=smallgreen>%1:%2:%3\n%4A</span></td>").arg(h).arg(m).arg(s).arg(this->data->current[1-1]) ;
           }else{
              os << QString("<span class=smallgreen>%1:%2:%3</span></td>").arg(h).arg(m).arg(s) ;
           }
           break;
       case 3://авария по току
           os << tr("<input type=image src=/pict/nasosred_a.gif border=0>1</form>");
           break;
       case 4://авария по температуре
           os << tr("<input type=image src=/pict/nasosred_t.gif border=0>1</form>");
           break;
       case 5://авария по влажности
           os << tr("<input type=image src=/pict/nasosred_vl.gif border=0>1</form>");
           break;
       case 6://авария по обрыву
           os << tr("<input type=image src=/pict/nasosred_k.gif border=0>1</form>");
           break;
       }
       //покажем насос №2
    os << tr("<td rowspan=2 align=center valign=middle>");
    os << tr("<form action=\"nasos.cgi?nomer\" method=get>");
    os << tr("<input type=hidden name=nomer value=2>");
    switch(this->data->nasos[2-1]){
    case 0://нет насоса
        os << tr("<input type=image src=/bmp/nasosgray_netu.gif border=0>2</form>");
        break;
    case 1:// не включен, готов
        os << tr("<input type=image src=/pict/nasosgray.gif border=0>2</form>");
        break;
    case 2://включен, работает
        os << tr("<input type=image src=/pict/nasosgreen.gif border=0>2</form>");
        int h,m,s;
        m = this->data->nasos_time_to_overtime[2-1] / 60;
        s = this->data->nasos_time_to_overtime[2-1] - (m * 60);
        h = m / 24;
        m = m - (h * 24);
        if(this->data->isATV12){
           os << QString("<span class=smallgreen>%1:%2:%3\n%4Hz\n%5A</span></td>").arg(h).arg(m).arg(s).arg((double)this->data->freq[2-1]/10).arg((double)this->data->current[2-1]/10) ;
        }else
        if(this->data->isSoftStart){
            os << QString("<span class=smallgreen>%1:%2:%3\n%4A</span></td>").arg(h).arg(m).arg(s).arg(this->data->current[2-1]) ;
        }else{
           os << QString("<span class=smallgreen>%1:%2:%3</span></td>").arg(h).arg(m).arg(s) ;
        }
        break;
    case 3://авария по току
        os << tr("<input type=image src=/pict/nasosred_a.gif border=0>2</form>");
        break;
    case 4://авария по температуре
        os << tr("<input type=image src=/pict/nasosred_t.gif border=0>2</form>");
        break;
    case 5://авария по влажности
        os << tr("<input type=image src=/pict/nasosred_vl.gif border=0>2</form>");
        break;
    case 6://авария по обрыву
        os << tr("<input type=image src=/pict/nasosred_k.gif border=0>2</form>");
        break;
    }

        os << tr("</tr>");

        os << tr("<tr>");
         os << tr("<td align=center valign=middle width=70><img src=/pict/housegreen.gif></td>");
        os << tr("</tr>");
       os << tr("</table>");
      os << tr("</td>");
      os << tr("<td rowspan=4>");
      os << tr("<table width=228 height=222 cellpadding=0 cellspacing=0 bgcolor=000000>");
       os << tr("<tr>");
        switch(this->data->level_to_show){
        case 0: os << tr("<td background=/pict/popl_4_uroven_avariya.gif align=right valign=bottom>");break;
        case 1: os << tr("<td background=/pict/popl_0_uroven_avariya.gif align=right valign=bottom>");break;
        case 102:
        case 2: os << tr("<td background=/pict/popl_0_uroven.gif align=right valign=bottom>");break;
        case 103:
        case 3: os << tr("<td background=/pict/popl_1_uroven.gif align=right valign=bottom>");break;
        case 104:
        case 4: os << tr("<td background=/pict/popl_1_2_uroven.gif align=right valign=bottom>");break;
        case 105:
        case 5: os << tr("<td background=/pict/popl_1_2_3_uroven.gif align=right valign=bottom>");break;
        case 106:
        case 6: os << tr("<td background=/pict/popl_1_2_3_4_uroven.gif align=right valign=bottom>");break;
        case 10: os << tr("<td background=/pict/popl_1_2_4_uroven.gif align=right valign=bottom>");break;
        case 11: os << tr("<td background=/pict/popl_1_3_4_uroven.gif align=right valign=bottom>");break;
        case 12: os << tr("<td background=/pict/popl_1_3_uroven.gif align=right valign=bottom>");break;
        case 13: os << tr("<td background=/pict/popl_1_4_uroven.gif align=right valign=bottom>");break;
        case 14: os << tr("<td background=/pict/popl_1_2_4_uroven.gif align=right valign=bottom>");break;
        case 15: os << tr("<td background=/pict/popl_2_3_uroven.gif align=right valign=bottom>");break;
        case 16: os << tr("<td background=/pict/popl_2_4_uroven.gif align=right valign=bottom>");break;
        case 17: os << tr("<td background=/pict/popl_2_uroven.gif align=right valign=bottom>");break;
        case 18: os << tr("<td background=/pict/popl_3_4_uroven.gif align=right valign=bottom>");break;
        case 19: os << tr("<td background=/pict/popl_3_uroven.gif align=right valign=bottom>");break;
        case 20: os << tr("<td background=/pict/popl_4_uroven.gif align=right valign=bottom>");break;
        }

        os << tr("<a href=/cgi-bin/ustanovki.cgi>");
        if(this->data->level_to_show >= 2 && this->data->level_to_show <= 6){
           os << QString("<font size=7>%1</font>").arg(this->data->level_to_show_sm);
        }
         //if [ "$avariya_datchika_urovnya" == 1 -a "$vremya_rab" != 0 ]
         //then
         //vremya_nepr_rab_nasosa vremya_rab
         //os << tr("$vremya_rab_popl_perepolneniya</a></td>");
         //else
         os << tr("<sub> cm</sub><font color=000000>__</font></a></td>");
         //fi
       os << tr("</tr>");
       os << tr("<tr height=18><td></td></tr>");
      os << tr("</table>");
      os << tr("</td>");
     os << tr("</tr>");
    os << tr("</table>");
    os << tr("<table width=480 height=29 cellpadding=0 cellspacing=1 class=menu>");
     os << tr("<tr>");
      os << tr("<td width=100 bgcolor=000000>колодец</td>");
      os << tr("<td width=380 bgcolor=000000 class=red>");
       //read perezagruzka
      //} < /nand/www/cgi-bin/perezagruzka
      //if [ "$perezagruzka" == 1 ]
      //then
       //echo "! Изменения вступят в силу после перезагрузки !"
      //fi
      os << tr("</td>");
    os << tr("</tr></table>");
    os << tr("</BODY></HTML>");
}
//=================================================================================================================
