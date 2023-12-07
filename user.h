#ifndef USER_H
#define USER_H

#include <QDialog>
#include "mydatabase.h"

namespace Ui {
class User;
}

class User : public QDialog
{
    Q_OBJECT

public:
    explicit User(QWidget *parent = nullptr);
    User(QString id,QString myPassword,QString name,QString sex,QString age,QString registerTime,QString money,QString contact,QWidget *parent = nullptr);
    ~User();

public slots:
    void setRegisterTime(const QDate& data);
    void addUser();
    void updateUser();
private:
    Ui::User *ui;
};

#endif // USER_H
