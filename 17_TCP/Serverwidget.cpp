#include "Serverwidget.h"
#include "ui_widget.h"


ServerWidget::ServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("Server");



    ptcpServer = new QTcpServer(this);
    ptcpServer->listen(QHostAddress::Any, 8893);

    connect(ptcpServer, &QTcpServer::newConnection, this, &ServerWidget::DealConnByThread);
}

ServerWidget::~ServerWidget()
{
    delete ui;
    qDebug() << "start destroy ServerWidget";
    if(m_objThread)
    {
        m_objThread->quit();
    }
    m_objThread->wait();
    qDebug() << "end destroy ServerWidget";

}

void ServerWidget::startObjThread()
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
    connect(this,&ServerWidget::startObjThreadWork1,m_obj,&ThreadObject::DealSubConnect);
    connect(this,&ServerWidget::startObjThreadWork2,m_obj,&ThreadObject::runSomeBigWork2);
    connect(m_obj,&ThreadObject::progress,this,&ServerWidget::progress);
    connect(m_obj,&ThreadObject::message,this,&ServerWidget::receiveMessage);
    m_objThread->start();
}

void ServerWidget::progress(int val)
{
    //do sth.
}

void ServerWidget::receiveMessage(const QString &str)
{
    //do sth.
}

void ServerWidget::heartTimeOut()
{
    //do sth.
}

void ServerWidget::DealConnByThread()
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

void ServerWidget::on_pushButton_2_clicked()
{
    p_socket->connected();
}

void ServerWidget::on_pushButtonSnd_clicked()
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

void ServerWidget::on_pushButtonClose_clicked()
{
    m_obj->stop();
}
