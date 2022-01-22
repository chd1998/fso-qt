#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "taserialthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void MWOnoffPort(void);
    void MWSendCmd(void);
    void MWReadData(void);
    void MWClearRecv(void);
    void MWRSNumDisplay(void);

private:
    Ui::MainWindow *ui;
    TA_SerialThread *sPortThread;
    volatile uint32_t m_recvNum;
    volatile uint32_t m_sendNum;
};

#endif // MAINWINDOW_H
