#ifndef WWW_H
#define WWW_H

#include <QStringList>
#include <QTcpServer>
#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
#include <QObject>
#include "data_struct.h"
#include "mainwindow.h"

//struct data_struct;

class FortuneServer : public QTcpServer{
    Q_OBJECT

public:
    FortuneServer(data_struct * dataStruct, QObject *parent = 0);
public slots:
            void readClient();
            void discardClient();
            void main_window(QTcpSocket* socket);
            void alarm_window(QTcpSocket* socket);
            void setup_window(QTcpSocket* socket);

protected:
    void incomingConnection(int socketDescriptor);
private:
    //QStringList fortunes;
    data_struct * data;
};
#endif // WWW_H
