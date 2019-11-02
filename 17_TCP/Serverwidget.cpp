#include "Serverwidget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Server");



    ptcpServer = new QTcpServer(this);
    ptcpServer->listen(QHostAddress::Any, 8893);

    connect(ptcpServer, &QTcpServer::newConnection, this, &Widget::DealConnByThread);
}

Widget::~Widget()
{
    delete ui;
    qDebug() << "start destroy widget";
    if(m_objThread)
    {
        m_objThread->quit();
    }
    m_objThread->wait();
    qDebug() << "end destroy widget";

}

void Widget::startObjThread()
{
    if(m_objThread)
    {
        return;
    }
    m_objThread= new QThread();
    m_obj = new ThreadObject();
    m_obj->moveToThread(m_objThread);
    connect(m_objThread,&QThread::finished,m_objThread,&QObject::deleteLater);
    connect(m_objThread,&QThread::finished,m_obj,&QObject::deleteLater);
    connect(this,&Widget::startObjThreadWork1,m_obj,&ThreadObject::DealSubConnect);
    connect(this,&Widget::startObjThreadWork2,m_obj,&ThreadObject::runSomeBigWork2);
    connect(m_obj,&ThreadObject::progress,this,&Widget::progress);
    connect(m_obj,&ThreadObject::message,this,&Widget::receiveMessage);
    m_objThread->start();
}

void Widget::progress(int val)
{
    //do sth.
}

void Widget::receiveMessage(const QString &str)
{
    //do sth.
}

void Widget::heartTimeOut()
{
    //do sth.
}

void Widget::DealConnByThread()
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

void Widget::on_pushButton_2_clicked()
{
    p_socket->connected();
}

void Widget::on_pushButtonSnd_clicked()
{
    if(!p_socket)
        return;

    p_socket->write(ui->textEdit_2->toPlainText().toStdString().c_str());

    if(!m_objThread)
        {
            startObjThread();
        }
        emit startObjThreadWork1();//主线程通过信号换起子线程的槽函数
        ui->textEdit->append("start Obj Thread work 1");
}

void Widget::on_pushButtonClose_clicked()
{
    m_obj->stop();
}
