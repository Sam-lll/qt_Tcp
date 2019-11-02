#include "Serverwidget.h"
#include "clientwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerWidget w;
    ClientWidget cw;

    w.show();
    cw.show();

    return a.exec();
}
