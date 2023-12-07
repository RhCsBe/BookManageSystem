#ifndef BOOK_H
#define BOOK_H

#include <QDialog>
#include "mydatabase.h"

namespace Ui {
class Book;
}

class Book : public QDialog
{
    Q_OBJECT

public:
    explicit Book(QWidget *parent = nullptr);
    Book(QString bookId,QString booKName,QString author,QString press,QString publishTime,QString kind,QString quantiy,QWidget *parent = nullptr);
    ~Book();
public slots:
    void setPublishTime(const QDate& date);
    void updateBook();
    void addBook();

private:
    Ui::Book *ui;
};

#endif // BOOK_H
