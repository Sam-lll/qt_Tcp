#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include "threadobject.h"

class ThreadObject;

namespace Ui {
class ServerWidget;
}

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWidget(QWidget *parent = 0);
    ~ServerWidget();
    void DealConnByThread();
signals:
    void startObjThreadWork1();
    void startObjThreadWork2();

private slots:

    void on_pushButton_2_clicked();

    void progress(int val);
    void receiveMessage(const QString& str);
    void heartTimeOut();
    void on_pushButtonSnd_clicked();

    void on_pushButtonClose_clicked();

private:
    void startObjThread();
private:
    Ui::ServerWidget *ui;
    QTcpSocket *p_socket = NULL;
    QTcpServer *ptcpServer = NULL;
    ThreadObject* m_obj = NULL;
    QThread* m_objThread = NULL;
};

#endif // WIDGET_H
