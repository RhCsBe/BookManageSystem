#ifndef BOOKMANAGER_H
#define BOOKMANAGER_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStringList>
#include "mydatabase.h"
#include "book.h"

namespace Ui {
class BookManager;
}

class BookManager : public QWidget
{
    Q_OBJECT

public:
    explicit BookManager(QWidget *parent = nullptr);
    QStandardItemModel* model=nullptr;
    void setHeader();
    void Init(QString condition);
    void setConnect();
    int getKind(QString str1);
    ~BookManager();
    Ui::BookManager *getUi();
public slots:
    void addBook();
    void updateBook();
    void deleteBook();
private:
    Ui::BookManager *ui;
};

#endif // BOOKMANAGER_H
