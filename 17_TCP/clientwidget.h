#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class ClientWidget;
}

class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientWidget(QWidget *parent = 0);
    ~ClientWidget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_Close_clicked();

    void on_pushButton_Snd_clicked();

private:
    Ui::ClientWidget *ui;
    QTcpSocket *p_socket;
};

#endif // CLIENTWIDGET_H
