#include "mainwindow.h"
#include <QApplication>
#include"login_management.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    MainWindow w;
//    w.show();
    login_Management s;
    s.show();
    return a.exec();
}
