#ifndef RETURNBOOK_H
#define RETURNBOOK_H

#include <QWidget>
#include <QStandardItemModel>
#include "mydatabase.h"

namespace Ui {
class ReturnBook;
}

class ReturnBook : public QWidget
{
    Q_OBJECT

public:
    explicit ReturnBook(QWidget *parent = nullptr);
    QStandardItemModel* model=nullptr;
    void setHeader();
    void Init(QString condition);
    void setConnect();
    int getKind(QString str1);
    ~ReturnBook();
    Ui::ReturnBook * getUi();

public slots:
    void returnOne();
    void returnAll();

private:
    Ui::ReturnBook *ui;
};

#endif // RETURNBOOK_H
