#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QDebug>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
        Q_OBJECT
        
public:
        explicit MainWidget(QWidget *parent = 0);
        ~MainWidget();

private slots:
        void newConnect();
        void readMessage();
        void displayError(QAbstractSocket::SocketError);
        
private:
        Ui::MainWidget *ui;
        QTcpSocket * socket;
        QString message;//��Ŵӷ������н��յ����ַ���
        quint16 blocksize;//����ַ����Ĵ�С
};

#endif // MAINWIDGET_H
