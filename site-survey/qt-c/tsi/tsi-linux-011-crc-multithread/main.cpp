#include "tsiGUI.h"
#include "myserver.h"
#include "mythread.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //MyServer gps01;
    //gps01.startServer();
    //connect(gps01,&MyServer::sendMsg2UI,w,&MainWindow::displayMsg);
    return a.exec();
}
