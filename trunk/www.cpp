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


FortuneServer::FortuneServer(QObject *parent)
    : QTcpServer(parent){

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

                QTextStream os(socket);
                os.setAutoDetectUnicode(true);

                os << "HTTP/1.0 200 Ok\r\n"
                    "Content-Type: text/html; charset=\"utf-8\"\r\n"
                    "\r\n";

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
                      os << tr("<td width=\"100\" background=/bmp/knopka_mini.gif><a href=\"/cgi-bin/ustanovki.cgi\">установки<a></td>");
                      //if [ "$cookie_parol" == "$parol_service" ]
                      //then
                      os << tr("<td width=\"100\" background=/bmp/knopka_mini.gif><a href=\"/cgi-bin/service.cgi\">сервис</a></td>");
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
/*
                         if [ "$nasos4" != n ]
                         then
                         vyvod_nasosov "$nasos4"
                         os << tr("<td rowspan="2" align="center" valign="middle">");
                         os << tr("<form action="nasos.cgi?nomer" method=get>");
                         os << tr("<input type="hidden" name="nomer" value="4">");
                          if [ "$nasos4" == 1 ]
                          then
                           vremya_nepr_rab_nasosa nepr_rab_4
                          os << tr("<input type="image" src=/bmp/"$nasos" border="0">4</form>");
                          os << tr("<span class="smallgreen">"$vremya_nepr_rab_4"</span></td>");
                          else
                          os << tr("<input type="image" src=/bmp/"$nasos" border="0">4</form></td>");
                          fi
                         else os << tr("<td></td>");
                         fi

                        os << tr("</tr>");
                        os << tr("<tr>");
                         os << tr("<td align="center" valign="middle" width="70">");
                          os << tr("<a href="/cgi-bin/schetchik.cgi"><img src=/bmp/"$schetchik" border="0"></a></td>");
                        os << tr("</tr>");
                        os << tr("<tr>");
                         os << tr("<td align="center" valign="middle" width="70"><a href="/cgi-bin/log_avarij.cgi" title="лог_аварий">");
                          os << tr("<img src=/bmp/"$log" border="0"></a></td>");

                        if [ "$nasos1" != n ]
                         then
                          vyvod_nasosov "$nasos1"
                          os << tr("<td rowspan="2" align="center" valign="middle">");
                          os << tr("<form action="nasos.cgi?nomer" method=get>");
                          os << tr("<input type="hidden" name="nomer" value="1">");
                          if [ "$nasos1" == 1 ]
                          then
                           vremya_nepr_rab_nasosa nepr_rab_1
                          os << tr("<input type="image" src=/bmp/"$nasos" border="0">1</form>");
                          os << tr("<span class="smallgreen">"$vremya_nepr_rab_1"</span></td>");
                          else
                          os << tr("<input type="image" src=/bmp/"$nasos" border="0">1</form></td>");
                          fi
                         else os << tr("<td></td>");
                         fi
                          if [ "$nasos2" != n ]
                         then
                          vyvod_nasosov "$nasos2"
                          os << tr("<td rowspan="2" align="center" valign="middle">");
                          os << tr("<form action="nasos.cgi?nomer" method=get>");
                          os << tr("<input type="hidden" name="nomer" value="2">");
                          if [ "$nasos2" == 1 ]
                          then
                           vremya_nepr_rab_nasosa nepr_rab_2
                          os << tr("<input type="image" src=/bmp/"$nasos" border="0">2</form>");
                          os << tr("<span class="smallgreen">"$vremya_nepr_rab_2"</span></td>");
                          else
                          os << tr("<input type="image" src=/bmp/"$nasos" border="0">2</form></td>");
                          fi
                         else os << tr("<td></td>");
                         fi
                        os << tr("</tr>");
                        os << tr("<tr>");
                         os << tr("<td align="center" valign="middle" width="70"><img src=/bmp/housegreen.bmp></td>");
                        os << tr("</tr>");
                       os << tr("</table>");
                      os << tr("</td>");
                      os << tr("<td rowspan="4">");
                      os << tr("<table width="228" height="222" ellpadding="0" cellspacing="0" bgcolor="000000">");
                       os << tr("<tr>");
                        os << tr("<td background=/bmp/"$poplavki" align="right" valign="bottom">");
                         os << tr("<a href="/cgi-bin/ustanovki.cgi">");
                         os << tr("<font size="7">""$uroven""</font>");
                         if [ "$avariya_datchika_urovnya" == 1 -a "$vremya_rab" != 0 ]
                         then
                         vremya_nepr_rab_nasosa vremya_rab
                         os << tr(""$vremya_rab_popl_perepolneniya"</a></td>");
                         else
                         os << tr("<sub>" cm"</sub><font color="000000">"__"</font></a></td>");
                         fi
                       os << tr("</tr>");
                       os << tr("<tr height="18"><td></td></tr>");
                      os << tr("</table>");
                      os << tr("</td>");
                     os << tr("</tr>");
                    os << tr("</table>");

*/
                }
                //alarm page
                if ( tokens[1].contains("page=alarm") ){
                    os << tr("alarm page<p><a href=\"/\">Back to main</a></p>");
                }
                //setup page
                if ( tokens[1].contains("page=setup") ){
                    os << tr("setup page<p><a href=\"/\">Back to main</a></p>");
                }
                //service page
                if ( tokens[1].contains("page=service") ){
                    os << tr("service page<p><a href=\"/\">Back to main</a></p>");
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
