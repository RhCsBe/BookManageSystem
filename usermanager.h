#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QWidget>
#include <QStandardItemModel>
#include "mydatabase.h"
#include "mydatabase.h"
#include "user.h"

namespace Ui {
class UserManager;
}

class UserManager : public QWidget
{
    Q_OBJECT

public:
    explicit UserManager(QWidget *parent = nullptr);
    QStandardItemModel* model=nullptr;
    void setHeader();
    void Init(QString condition);
    void setConnect();
    ~UserManager();
    Ui::UserManager * getUi();
public slots:
    void addUser();
    void updateUser();
    void deleteUser();
private:
    Ui::UserManager *ui;
};

#endif // USERMANAGER_H
