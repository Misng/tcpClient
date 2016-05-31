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
        void sendMessage();
        
private:
        Ui::MainWidget *ui;
        QString message;//��Ŵӷ������н��յ����ַ���
        QTcpSocket * socket;
};

#endif // MAINWIDGET_H
