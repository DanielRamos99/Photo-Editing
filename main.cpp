#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;   //Arranque de ventana 'w'
    w.show();       //Se encuentra descrito en mainwindow .cpp y .h
    return a.exec();
}
