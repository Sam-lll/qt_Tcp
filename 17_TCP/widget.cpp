#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Server");
    p_socket = NULL;

    QTcpServer *ptcpServer = new QTcpServer(this);
    ptcpServer->listen(QHostAddress::Any, 8893);

    connect(ptcpServer, &QTcpServer::newConnection,
            [=]()
            {
                p_socket = ptcpServer->nextPendingConnection();
                QString str_IP = p_socket->peerAddress().toString();
                int iPort = p_socket->peerPort();

                QString str_tmp = QString("[ip:%1, port:%2, connect success!]").arg(str_IP).arg(iPort);

                ui->textEdit->setText(str_tmp);

                connect(p_socket, &QTcpSocket::readyRead,
                        [=]()
                        {
                            QByteArray byte_tmp = p_socket->readAll();
                            ui->textEdit->append(byte_tmp);

                        }
                        );
            }
            );
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    if(!p_socket)
        return;

    p_socket->write(ui->textEdit_2->toPlainText().toStdString().c_str());

}

void Widget::on_pushButton_2_clicked()
{
    p_socket->connected();
}
