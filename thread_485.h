#ifndef TREAD_485_H
#define TREAD_485_H

#include <QThread>
#include <QMessageBox>
#include "mainwindow.h"

// для СЕ303
#define SOH 0x01
#define STX 0x02
#define ETX 0x03
// ACK
// NACK

class Thread_485 : public QThread{
     Q_OBJECT
public:
        explicit Thread_485(MainWindow *wnd);

        void run();
        void quit();
        void init();
private:
        void set_8N1(void);
        void set_7P1(void);
        int request(const char s_req[100]);
        int openCE303(void);
        void closeCE303(void);
        int read_modbus_reg(unsigned char dev_address, unsigned short reg);
        int write_modbus_reg(unsigned char dev_address, unsigned short reg, unsigned short value);
//        void read_config(void);
//        void set_defaults(void);

signals:
        void changeDataATV12();
        void changeDataCE303();

private:
        Ui::MainWindow *ui;
        MainWindow *wnd;
};

#endif // TREAD_485_H
