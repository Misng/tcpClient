#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::MainWidget)
{
        ui->setupUi(this);

        socket = new QTcpSocket(this);

        //连接套接字对象的两个信号槽，一个为可读的时候，可以出错的时候。
        connect(socket,SIGNAL(readyRead()),SLOT(readMessage()));
        connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),SLOT(displayError(QAbstractSocket::SocketError)));
        connect(ui->connect_pushButton,SIGNAL(clicked()),this,SLOT(newConnect()));
        connect(ui->send_pushButton,SIGNAL(clicked()),this,SLOT(sendMessage()));
}

MainWidget::~MainWidget()
{
        delete ui;
}


void MainWidget::readMessage()//就不是使用read/write来读写数据的，直接使用QDataStream来绑定IO设备然后，在进行读写操作。
{
        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_4_8);
        quint16 blocksize = 0;

        if(blocksize == 0){
                if(socket->bytesAvailable() < sizeof(quint16))//判断从服务器上接受的数据是否有两个字节，如果没有则返回，继续等待接受。      先判断是否有两个字节，原因是要把两个字节长的头给得到，然后在判断数据的总长度是否为blocksize，如果不够还是返回。
                        return ;
                in >> blocksize;
        }
        if(socket->bytesAvailable() < blocksize)//bytesAvailable返回等待读取的字节数,如果结果为真则表明数据没有完全读取
                return;

        in >> message;
        qDebug() << message;

}

void MainWidget::displayError(QAbstractSocket::SocketError)
{
        qDebug()<<"displayError"<<socket->errorString();
}

void MainWidget::newConnect()
{
        socket->abort();//终止当前连接并重置这个socket，与disconnectFromHost不同的是，这个函数会断开这个socket，并丢弃还未写入到缓冲区中的数据。
        socket->connectToHost("10.1.37.44",
                              ui->port_lineEdit->text().toUShort());//通过connectToHost来访问服务器
}

void MainWidget::sendMessage()
{
        //初始化数据结构
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        //向数据结构中写入数据
        out << (quint16)0;
        out << ui->send_lineEdit->text();
        out.device()->seek(0);
        out << quint16(block.size() - sizeof(quint16));

        socket->write(block);
        qDebug("Send message successful");

}
