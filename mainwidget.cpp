#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::MainWidget)
{
        ui->setupUi(this);

        socket = new QTcpSocket(this);

        //�����׽��ֶ���������źŲۣ�һ��Ϊ�ɶ���ʱ�򣬿��Գ����ʱ��
        connect(socket,SIGNAL(readyRead()),SLOT(readMessage()));
        connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),SLOT(displayError(QAbstractSocket::SocketError)));
        connect(ui->connect_pushButton,SIGNAL(clicked()),this,SLOT(newConnect()));
        connect(ui->send_pushButton,SIGNAL(clicked()),this,SLOT(sendMessage()));
}

MainWidget::~MainWidget()
{
        delete ui;
}


void MainWidget::readMessage()//�Ͳ���ʹ��read/write����д���ݵģ�ֱ��ʹ��QDataStream����IO�豸Ȼ���ڽ��ж�д������
{
        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_4_8);
        quint16 blocksize = 0;

        if(blocksize == 0){
                if(socket->bytesAvailable() < sizeof(quint16))//�жϴӷ������Ͻ��ܵ������Ƿ��������ֽڣ����û���򷵻أ������ȴ����ܡ�      ���ж��Ƿ��������ֽڣ�ԭ����Ҫ�������ֽڳ���ͷ���õ���Ȼ�����ж����ݵ��ܳ����Ƿ�Ϊblocksize������������Ƿ��ء�
                        return ;
                in >> blocksize;
        }
        if(socket->bytesAvailable() < blocksize)//bytesAvailable���صȴ���ȡ���ֽ���,������Ϊ�����������û����ȫ��ȡ
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
        socket->abort();//��ֹ��ǰ���Ӳ��������socket����disconnectFromHost��ͬ���ǣ����������Ͽ����socket����������δд�뵽�������е����ݡ�
        socket->connectToHost("10.1.37.44",
                              ui->port_lineEdit->text().toUShort());//ͨ��connectToHost�����ʷ�����
}

void MainWidget::sendMessage()
{
        //��ʼ�����ݽṹ
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        //�����ݽṹ��д������
        out << (quint16)0;
        out << ui->send_lineEdit->text();
        out.device()->seek(0);
        out << quint16(block.size() - sizeof(quint16));

        socket->write(block);
        qDebug("Send message successful");

}
