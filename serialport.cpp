#include "serialport.h"
#include<QDebug>
SerialPort::SerialPort(QWidget *parent):
    QObject(parent)
{
    m_fd=-1;
    rev_buf=new QByteArray();
}
//open
bool SerialPort::openPort(QString portName, BaudRateType baundRate, DataBitsType dataBits,
                          ParityType parity, StopBitsType stopBits, FlowType flow, int time_out)
{

    if(m_fd!=-1)
    {
        qDebug("is aready opened!");
        return false;
    }
    rev_buf->clear();
    memset(&new_serialArry,0,sizeof new_serialArry);
    m_fd=::open(portName.toLatin1(),O_RDWR|O_NONBLOCK);
    if( m_fd==-1)
    {
        qDebug("serial port  open erro!");

        return false;
    }

    switch(baundRate)
    {
    case BAUD50:
        new_serialArry.c_cflag |=  B50;
        break;
    case BAUD75:
        new_serialArry.c_cflag |=  B75;
        break;
    case BAUD110:
        new_serialArry.c_cflag |=  B110;
        break;
    case BAUD300:
        new_serialArry.c_cflag |=  B300;
        break;
    case BAUD600:
        new_serialArry.c_cflag |=  B600;
        break;
    case BAUD1200:
        new_serialArry.c_cflag |=  B1200;
        break;
    case BAUD2400:
        new_serialArry.c_cflag |=  B2400;
        break;
    case BAUD4800:
        new_serialArry.c_cflag |=  B4800;
        break;
    case BAUD9600:
        new_serialArry.c_cflag |=  B9600;
        break;
    case BAUD19200:
        new_serialArry.c_cflag |=  B19200;
        break;
    case BAUD38400:
        new_serialArry.c_cflag |=  B38400;
        break;
    case BAUD57600:
        new_serialArry.c_cflag |=  B57600;
        break;
    case BAUD115200:
        new_serialArry.c_cflag |=  B115200;
        break;
    default:
        new_serialArry.c_cflag |=  B9600;
        break;
    }

    switch(dataBits)
    {
    case DATA_5:
        new_serialArry.c_cflag|=CS5;
        break;
    case DATA_6:
        new_serialArry.c_cflag|=CS6;
        break;
    case DATA_7:
        new_serialArry.c_cflag|=CS7;
        break;
    case DATA_8:
        new_serialArry.c_cflag|=CS8;
        break;
    default:
        new_serialArry.c_cflag|=CS8;
        break;
    }

    switch (parity)
    {

    case PAR_NONE:
        new_serialArry.c_cflag &= (~PARENB);
        break;
    case PAR_EVEN:
        new_serialArry.c_cflag &= (~PARODD);
        new_serialArry.c_cflag |= PARENB;
        break;
    case PAR_ODD:
        new_serialArry.c_cflag |= (PARODD|PARENB);
        break;
    default:
        break;
    }


    switch(stopBits)
    {
    case STOP_1:
        new_serialArry.c_cflag &=(~CSTOPB);
        break;
    case STOP_1_5:
        break;
    case STOP_2:
        new_serialArry.c_cflag |=CSTOPB;
        break;
    default:
        new_serialArry.c_cflag &=(~CSTOPB);
        break;
    }

    switch(flow)
    {
    case FLOW_OFF:
        new_serialArry.c_cflag &=(~CRTSCTS);
        new_serialArry.c_iflag &=(~(IXON|IXOFF|IXANY));
        break;
    case FLOW_XONXOFF:
        new_serialArry.c_cflag &=(~CRTSCTS);
        new_serialArry.c_iflag |=(IXON|IXOFF|IXANY);
        break;
    case FLOW_HARDWARE:
        new_serialArry.c_cflag |=~CRTSCTS;
        new_serialArry.c_iflag &=(~(IXON|IXOFF|IXANY));
        break;
    default:
        break;
    }

    new_serialArry.c_oflag=0;
    new_serialArry.c_cc[VTIME]=time_out/100;
    tcflush(m_fd,TCIFLUSH);
    tcsetattr(m_fd,TCSANOW,&new_serialArry);

    m_notifier= new QSocketNotifier(m_fd,QSocketNotifier::Read,0);
    connect(m_notifier,SIGNAL(activated(int)),this,SLOT(remoteDateInComing()));

    return true;

}
//close
bool SerialPort::close()
{
    mutex_r.lock();

    if(m_fd!=-1)
    {
        disconnect(m_notifier,SIGNAL(activated(int)),this,SLOT(remoteDateInComing()));
        delete m_notifier;
        m_fd=-1;
        mutex_r.unlock();
        return true;
    }
    mutex_r.unlock();
    return false;
}
//recive data
void SerialPort::remoteDateInComing()
{

    unsigned char c[1024];
    int n= ::read(m_fd,&c,sizeof c);
    mutex_r.lock();
    for(int i=0;i<n;i++)
    {
        rev_buf->append(c[i]);

    }
    mutex_r.unlock();
    emit hasdata();


}
//write data
int SerialPort::write(QByteArray buf)
{

    mutex_r.lock();
    int ret;
    ret=0;
    if(m_fd!=-1)
    {        

               ret= :: write(m_fd,buf.data(),buf.length());
    }

    mutex_r.unlock();


    return ret;
}

QByteArray SerialPort::read()
{
    mutex_r.lock();
    QByteArray retByteArray;
    if(rev_buf->isEmpty())
    {
        retByteArray.append("aaa");
        retByteArray.clear();
    }else
    {
        //RECEIVE 00 ERROR 2019/2/19  retByteArray.append(rev_buf->data())
        retByteArray.append(rev_buf->data(),rev_buf->size());
        rev_buf->clear();
    }
    mutex_r.unlock();

    return retByteArray;

}
