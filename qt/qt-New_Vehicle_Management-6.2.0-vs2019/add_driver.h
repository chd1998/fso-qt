#ifndef ADD_DRIVER_H
#define ADD_DRIVER_H

#include <QDialog>

namespace Ui {
class Add_Driver;
}

class Add_Driver : public QDialog
{
    Q_OBJECT

public:
    explicit Add_Driver(QWidget *parent = nullptr);
    ~Add_Driver();

public slots:
    void Add_Driver_Message();
    void Receive_Message(QString ,QString,QString );



private:
    Ui::Add_Driver *ui;
    QString id;
    QString no;
    QString type;
};

#endif // ADD_DRIVER_H
