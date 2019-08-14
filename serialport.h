#ifndef SER_COM_PORT_TX_H
#define SER_COM_PORT_TX_H
#include <QSocketNotifier>
#include <linux/fs.h>
#include<fcntl.h>
#include<errno.h>
#include<termio.h>
#include<sys/ioctl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include <stdio.h>
#include<QTimer>
#include<QMutex>
#include<QWidget>

enum BaudRateType
{
    BAUD50,                //POSIX ONLY
    BAUD75,                //POSIX ONLY
    BAUD110,
    BAUD134,               //POSIX ONLY
    BAUD150,               //POSIX ONLY
    BAUD200,               //POSIX ONLY
    BAUD300,
    BAUD600,
    BAUD1200,
    BAUD1800,              //POSIX ONLY
    BAUD2400,
    BAUD4800,
    BAUD9600,
    BAUD14400,             //WINDOWS ONLY
    BAUD19200,
    BAUD38400,
    BAUD56000,             //WINDOWS ONLY
    BAUD57600,
    BAUD76800,             //POSIX ONLY
    BAUD115200,
    BAUD128000,            //WINDOWS ONLY
    BAUD256000             //WINDOWS ONLY
};

enum DataBitsType
{
    DATA_5,
    DATA_6,
    DATA_7,
    DATA_8
};

enum ParityType
{
    PAR_NONE,
    PAR_ODD,
    PAR_EVEN,
    PAR_MARK,               //WINDOWS ONLY
    PAR_SPACE
};

enum StopBitsType
{
    STOP_1,
    STOP_1_5,               //WINDOWS ONLY
    STOP_2
};

enum FlowType
{
    FLOW_OFF,
    FLOW_HARDWARE,
    FLOW_XONXOFF
};


class SerialPort:public QObject
{
    Q_OBJECT

public:
    explicit SerialPort(QWidget *parent = 0);
private:
    int m_fd;
    termios new_serialArry;
    QSocketNotifier *m_notifier;
    QByteArray  *rev_buf;
    QMutex mutex_r;

public :  

    //open
    bool openPort(QString portName,BaudRateType baundRate,DataBitsType dataBits,ParityType parity,
              StopBitsType stopBits, FlowType flow ,int time_out);
    //close
    bool close();

    //write
    int  write(QByteArray buf);

    //read
    QByteArray read();


signals:
    void hasdata();

private slots:
    void remoteDateInComing();





};
#endif // SER_COM_PORT_TX_H
