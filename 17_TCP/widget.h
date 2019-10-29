#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include "threadobject.h"

class ThreadObject;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void DealConnByThread();
signals:
    void startObjThreadWork1();
    void startObjThreadWork2();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void progress(int val);
    void receiveMessage(const QString& str);
    void heartTimeOut();
private:
    void startObjThread();
private:
    Ui::Widget *ui;
    QTcpSocket *p_socket = NULL;
    QTcpServer *ptcpServer = NULL;
    ThreadObject* m_obj;
    QThread* m_objThread;
};

#endif // WIDGET_H
