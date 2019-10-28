#include "clientwidget.h"
#include "ui_clientwidget.h"

ClientWidget::ClientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("Client");
    p_socket = NULL;

    p_socket = new QTcpSocket(this);
    p_socket->connectToHost("127.0.0.1", 8893);


}

ClientWidget::~ClientWidget()
{
    delete ui;
}

void ClientWidget::on_pushButton_clicked()
{
    if(!p_socket)
        return;

    p_socket->write(ui->textEdit->toPlainText().toStdString().c_str());
}

void ClientWidget::on_pushButton_Close_clicked()
{
    p_socket->close();
}

void ClientWidget::on_pushButton_Snd_clicked()
{
    if(!p_socket)
        return;

    int iRet = p_socket->write(ui->textEdit->toPlainText().toStdString().c_str());

}
