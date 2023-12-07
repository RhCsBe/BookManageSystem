#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QObject>
#include <QtSql>
#include <QMessageBox>
#include <QVector>
#include <QStringList>
#include <QDate>

class MyDataBase:public QObject
{
    Q_OBJECT
public:
    QSqlDatabase db;
    static MyDataBase* getDataBase();
    //当前用户信息
    QString Id="";
    QString password="";
    QString name="";
    QString sex="";
    int age=0;
    bool power=false;
    QString registerTime="";
    double money=0;
    QString contact="";
    //功能函数
    QVector<QVector<QVariant>> getReader(QString condition="");
    QVector<QVector<QVariant>> getBook(QString condition="");
    QVector<QVector<QVariant>> getRecord(QString condition="");
    QVector<QVector<QVariant>> getReturnRecord(QString condition="");
    void borrowBook(QString str1,QString str2,QString str3,QString str4);
    void returnBook(QString str1,QString str2,QString str3);
    void deleteRecord(QString str1,QString str2);
    void updateBook(QString str1,QString str2,QString str3,QString str4,QString str5,QString str6,QString str7);
    void addBook(QString str1,QString str2,QString str3,QString str4,QString str5,QString str6,QString str7);
    void deleteBook(QString str1);
    void addUser(QString str1,QString str2,QString str3,QString str4,QString str5,QString str6,QString str7,QString str8);
    void updateUser(QString str1,QString str2,QString str3,QString str4,QString str5,QString str6,QString str7,QString str8);
    void deleteUser(QString str1);
    void updateManager(QString str1,QString str2,QString str3,QString str4,QString str5,QString str6,QString str7);
    int login(QString str1,QString str2);
private:
    MyDataBase();
    //使用单例设计模式，使一个对象实体被多个对象使用
    static MyDataBase* myDataBase;
};

#endif // MYDATABASE_H
