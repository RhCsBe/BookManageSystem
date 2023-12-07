#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "mydatabase.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    Ui::Login *getUi();
    ~Login();
signals:
    void userKind(int kind);
public slots:
    void login();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
