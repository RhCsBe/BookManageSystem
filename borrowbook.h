#ifndef BORROWBOOK_H
#define BORROWBOOK_H

#include <QWidget>
#include <QStandardItemModel>
#include "mydatabase.h"

namespace Ui {
class BorrowBook;
}

class BorrowBook : public QWidget
{
    Q_OBJECT

public:
    explicit BorrowBook(QWidget *parent = nullptr);
    QStandardItemModel* model=nullptr;
    void setHeader();
    void Init(QString condition);
    void setConnect();
    int getKind(QString str1);
    ~BorrowBook();
    Ui::BorrowBook *getUi();

public slots:
    void borrowBook();
private:
    Ui::BorrowBook *ui;
};

#endif // BORROWBOOK_H
