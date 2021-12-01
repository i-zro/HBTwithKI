#include "HBTwithKIQt.h"
#include "HBTwithKIQt.ui"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HBTwithKIQt w;
    w.show();
    return a.exec();
}
