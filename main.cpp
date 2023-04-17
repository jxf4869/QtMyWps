#include "QtMyWps.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtMyWps *w=new QtMyWps;
    w->show();
    return a.exec();
}
