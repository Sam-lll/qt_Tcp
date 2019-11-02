#ifndef THREADOBJECT_H
#define THREADOBJECT_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>

//QMutex mutex;
//QWaitCondition signal;

class ThreadObject : public QObject
{
    Q_OBJECT
public:
    ThreadObject(QObject* parent = NULL);
    ~ThreadObject();
    void setRunCount(int count);
    void stop();
signals:
    void message(const QString& info);
    void progress(int present);
public slots:
    void DealSubConnect();
    void runSomeBigWork2();
private:
    int m_runCount;
    int m_runCount2;
    bool m_isStop;
    QMutex m_stopMutex;
    QMutex m_Mutex;
    QWaitCondition m_signal;
};

#endif // THREADOBJECT_H
