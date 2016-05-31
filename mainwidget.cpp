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
        connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(newConnect()));
}

MainWidget::~MainWidget()
{
        delete ui;
}


void MainWidget::readMessage()//�Ͳ���ʹ��read/write����д���ݵģ�ֱ��ʹ��QDataStream����IO�豸Ȼ���ڽ��ж�д������
{
        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_4_8);
        if(blocksize == 0){
                if(socket->bytesAvailable() < sizeof(quint16))//�жϴӷ������Ͻ��ܵ������Ƿ��������ֽڣ����û���򷵻أ������ȴ����ܡ�      ���ж��Ƿ��������ֽڣ�ԭ����Ҫ�������ֽڳ���ͷ���õ���Ȼ�����ж����ݵ��ܳ����Ƿ�Ϊblocksize������������Ƿ��ء�
                        return ;
                in >> blocksize;
        }
        if(socket->bytesAvailable() < blocksize)//bytesAvailable���صȴ���ȡ���ֽ���,������Ϊ�����������û����ȫ��ȡ
                return;
        in >> message;
        qDebug() << message;
//        connect(socket,SIGNAL(disconnected()),socket,SLOT(deleteLater()));
        socket->disconnectFromHost();

}

void MainWidget::displayError(QAbstractSocket::SocketError)
{
        qDebug()<<"displayError"<<socket->errorString();
}

void MainWidget::newConnect()
{
        blocksize = 0;
        socket->abort();//��ֹ��ǰ���Ӳ��������socket����disconnectFromHost��ͬ���ǣ����������Ͽ����socket����������δд�뵽�������е����ݡ�
        socket->connectToHost("10.1.37.44",
                              ui->port_lineEdit->text().toUShort());//ͨ��connectToHost�����ʷ�����

}
