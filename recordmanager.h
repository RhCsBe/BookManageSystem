#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H

#include <QWidget>
#include <QStandardItemModel>
#include "mydatabase.h"

namespace Ui {
class RecordManager;
}

class RecordManager : public QWidget
{
    Q_OBJECT

public:
    explicit RecordManager(QWidget *parent = nullptr);
    QStandardItemModel* model=nullptr;
    void setHeader();
    void Init(QString condition);
    void setConnect();
    int getKind(QString str1);
    ~RecordManager();
    Ui::RecordManager * getUi();

public slots:
    void deleteOne();
    void deleteAll();

private:
    Ui::RecordManager *ui;
};

#endif // RECORDMANAGER_H
