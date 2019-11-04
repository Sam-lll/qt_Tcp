#include "threadobject.h"
#include <QThread>
#include <QDebug>
#include <QMutexLocker>
#include <QElapsedTimer>
#include <limits>
#include <unistd.h>

ThreadObject::ThreadObject(QObject *parent):QObject(parent)
  ,m_runCount(10)
  ,m_runCount2(std::numeric_limits<int>::max())
  ,m_isStop(true)
{
    ptcpServer = new QTcpServer(this);
    ptcpServer->listen(QHostAddress::Any, 8893);

    connect(ptcpServer, &QTcpServer::newConnection, this, &ThreadObject::processClientConnection);
}
ThreadObject::~ThreadObject()
{
    qDebug() << "ThreadObject destroy";
    emit message(QString("Destroy %1->%2,thread id:%3").arg(__FUNCTION__).arg(__FILE__).arg((long)QThread::currentThreadId()));
}
void ThreadObject::setRunCount(int count)
{
    m_runCount = count;
    emit message(QString("%1->%2,thread id:%3").arg(__FUNCTION__).arg(__FILE__).arg((long)QThread::currentThreadId()));
}
void ThreadObject::connectStat()
{
    {
        QMutexLocker locker(&m_stopMutex);
        m_isStop = false;
    }

    m_Mutex.lock();
    while(!m_signal.wait(&m_Mutex, 10000))
    {
        {
            QMutexLocker locker(&m_stopMutex);
            if(m_isStop)
                return;
        }
        m_Mutex.unlock();
    }
}

void ThreadObject::processClientConnection()
{
    QMutexLocker locker(&m_Mutex);
    QWaitCondition signal;
    QTcpSocket *pSocket = ptcpServer->nextPendingConnection();
    m_vecSocket.push_back(pSocket);
    QString str_IP = p_socket->peerAddress().toString();
    int iPort = pSocket->peerPort();

    QString str_tmp = QString("[ip:%1, port:%2, connect success!]").arg(str_IP).arg(iPort);

    //Ui::ui->textEdit->setText(str_tmp);
    //
    //connect(p_socket, &QTcpSocket::readyRead,
    //        [=]()
    //        {
    //            QByteArray byte_tmp = p_socket->readAll();
    //            ui->textEdit->append(byte_tmp);
    //        }
    //        );
    while (pSocket->state() != QAbstractSocket::ClosingState) {
        qDebug() << pSocket->state();
        signal.wait(&m_Mutex, 10000);

    }
}
void ThreadObject::runSomeBigWork2()
{
    {
        QMutexLocker locker(&m_stopMutex);
        m_isStop = false;
    }
    int count = 0;
    QString str = QString("%1->%2,thread id:%3").arg(__FILE__).arg(__FUNCTION__).arg((long)QThread::currentThreadId());
    emit message(str);
    int process = 0;
    QElapsedTimer timer;
    timer.start();
    while(1)
    {
        {
            QMutexLocker locker(&m_stopMutex);
            if(m_isStop)
                return;
        }
        if(m_runCount2 == count)
        {
            break;
        }
        int pro = ((float)count / m_runCount2) * 100;
        if(pro != process)
        {
            process = pro;
            emit progress(pro);
            emit message(QString("%1,%2,%3,%4")
                         .arg(count)
                         .arg(m_runCount2)
                         .arg(pro)
                         .arg(timer.elapsed()));
            timer.restart();
        }
        ++count;
    }
}
void ThreadObject::stop()
{
    //QMutexLocker locker(&m_stopMutex);
    m_signal.wakeAll();
    //emit message(QString("%1->%2,thread id:%3").arg(__FUNCTION__).arg(__FILE__).arg((long)QThread::currentThreadId()));
    //m_isStop = true;
}
