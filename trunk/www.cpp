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
                      << QDateTime::currentDateTime().toString() << "<BR>\n";*/
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
                    os << tr("<p><a href=\"/?page=alarm\">Как сделать такое же фото?</a></p>\n");
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
