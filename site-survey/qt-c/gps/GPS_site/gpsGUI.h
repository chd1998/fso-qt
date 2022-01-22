#ifndef GPSGUI_H
#define GPSGUI_H

#include <QMainWindow>
#include <QObject>
#include <QComboBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QStringList>
#include <QTimer>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void gpsDisplay();
    void gpsDisplay_2();
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTimer         *readTimer;
    //#define time_interval 1000
    uint time_interval;
    QByteArray GPS_RMC;
    QByteArray GPS_RMC_2;
    QList<QByteArray> GPS_list;
    QList<QByteArray> GPS_list_2;
    QString& latitude(QString& lat_str);
    QString& longitude(QString& lon_str);
    QString& time(QString& time_str);
    QString& date(QString& date_str);
    QString& direction(QString& direction_str);
    QString& satellite(QString& satellite_str);
    QString& getAlt(QString& alt_str);

private:
    Ui::MainWindow *ui;
    QSerialPort    *serialPortset;



private slots:
    void on_searchButton_clicked();
    void on_openComButton_clicked();
    void readData();

    void on_gpsPlainTextEdit_textChanged();
};

#endif // GPSGUI_H
