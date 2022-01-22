#ifndef APPLY_ACCOUNT_H
#define APPLY_ACCOUNT_H

#include <QDialog>

namespace Ui {
class Apply_Account;
}

class Apply_Account : public QDialog
{
    Q_OBJECT

public:
    explicit Apply_Account(QWidget *parent = nullptr);
    ~Apply_Account();
public slots:
    void clicked_Ok();
    void clicked_No();
private:
    Ui::Apply_Account *ui;
};

#endif // APPLY_ACCOUNT_H
