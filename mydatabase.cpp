#include "mydatabase.h"

//静态变量不能在头文件中进行初始化，哪怕是在头文件的类外中也不行，因为这会造成头文件被多次包含时静态变量被多次定义，所以只能在源文件中初始化
MyDataBase* MyDataBase::myDataBase=nullptr;
MyDataBase *MyDataBase::getDataBase()
{
    if(!myDataBase)
    {
        myDataBase=new MyDataBase();
        return myDataBase;
    }
    else
    {
        return myDataBase;
    }
}

QVector<QVector<QVariant> > MyDataBase::getReader(QString condition)
{
    QString str="";
    if(power)
    {
        str="select * from Reader where Id like '%"+condition+"%' or ReaderName like '%"+condition
              +"%' or sex = '"+condition+"' or age = '"+condition+"' or contact like '%"+condition+"%'";
    }
    else
    {
        str="select * from Reader where Id = '"+Id+"'";
    }
//    qDebug()<<condition;
//    qDebug()<<str;
    QSqlQuery sql(db);
    sql.exec(str);
    QVector<QVector<QVariant>> vector_1;
    while(sql.next())
    {
        QVector<QVariant> vector_2;
        for(int i=0;i<8;i++)
        {
            vector_2.push_back(sql.value(i));
        }
        vector_1.push_back(vector_2);
    }
    return vector_1;
}

QVector<QVector<QVariant>> MyDataBase::getBook(QString condition)
{
    //这里不能使用QString::asprintf函数来创建QString，因为condition为数字字符串时，%%转义为%后会把填充的数字字符串作为占位符
    QString str="select * from Book where BookId like '%"+condition+"%' or BookName like '%"+condition+"%' or Author like '%"+condition+"%' "
                                    "or Press like '%"+condition+"%' or Kind like '%"+condition+"%'";
//    qDebug()<<condition;
//    qDebug()<<str;
    QSqlQuery sql(db);
    sql.exec(str);
    QVector<QVector<QVariant>> vector_1;
    while(sql.next())
    {
        QVector<QVariant> vector_2;
        for(int i=0;i<7;i++)
        {
            vector_2.push_back(sql.value(i));
        }
        vector_1.push_back(vector_2);
    }
    return vector_1;
}

//存在问题，应该区分读者和管理员获取数据
//已解决一部分，还需解决获取管理员记录数据的问题
QVector<QVector<QVariant>> MyDataBase::getRecord(QString condition)
{
    QString str="";
    if(power)
    {
        //一次执行多条搜索语句，只会返回第一条的搜索结果，所以要分开搜索
        str="SELECT * FROM readerrecord WHERE RecordId LIKE '%"+condition+"%' OR BookId LIKE '%"
              +condition+"%' OR BookName LIKE '%"+condition+"%' OR Kind LIKE '%"
              +condition+"%' OR ReaderId LIKE '%"+condition+"%' OR ReaderName LIKE '%"
              +condition+"%' OR BorrowTime LIKE '%"+condition+"%' OR ReturnTime LIKE '%"+condition+"%';";
    }
    else
    {
        str="SELECT * FROM readerrecord WHERE ( RecordId LIKE '%"+condition+"%' OR BookId LIKE '%"
              +condition+"%' OR BookName LIKE '%"+condition+"%' OR Kind LIKE '%"
              +condition+"%') and ReaderId = '"+Id+"' and ReaderName = '"
              +name+"' and ( BorrowTime LIKE '%"+condition+"%' OR ReturnTime LIKE '%"+condition+"%' )";
    }
//    qDebug()<<condition;
//    qDebug()<<str;
    QSqlQuery sql(db);
    sql.exec(str);
    QVector<QVector<QVariant>> vector_1;
    while(sql.next())
    {
        QVector<QVariant> vector_2;
        for(int i=0;i<10;i++)
        {
            vector_2.push_back(sql.value(i));
        }
        vector_1.push_back(vector_2);
    }
    //如果是管理员，还得获取管理员记录表数据
    if(power)
    {
        str="SELECT * FROM managerrecord WHERE RecordId LIKE '%"+condition+"%' OR BookId LIKE '%"
              +condition+"%' OR BookName LIKE '%"+condition+"%' OR Kind LIKE '%"
              +condition+"%' OR ManagerId LIKE '%"+condition+"%' OR ManagerName LIKE '%"
              +condition+"%' OR BorrowTime LIKE '%"+condition+"%' OR ReturnTime LIKE '%"+condition+"%';";
        //str="select * from managerrecord;";
        sql.exec(str);
        qDebug()<<"AAA";
        while(sql.next())
        {
            QVector<QVariant> vector_2;
            for(int i=0;i<10;i++)
            {
                vector_2.push_back(sql.value(i));
            }
            vector_1.push_back(vector_2);
        }
    }
    return vector_1;
}

//后期完善登录后可以换不同用户来检测
QVector<QVector<QVariant> > MyDataBase::getReturnRecord(QString condition)
{
    QString str="select RecordId,BookId,BookName,Kind,ReaderId,ReaderName,BorrowTime,Deadline from ReaderRecord ";
    //sql语句中and的优先级高于or
    if(!power)
    {
        str+="where ReaderId = '"+Id+"' and ReturnTime = '' and (RecordId like '%"+condition+"%' or BookId like '%"
               +condition+"%' or BookName like '%"+condition+"%' or kind like '%"+condition+"%' or BorrowTime like '%"+condition+"%')";
    }
    else
    {
        str+="where ReturnTime = '' and (ReaderId like '%"+condition+"%' or RecordId like '%"+condition+"%' or BookId like '%"+condition+"%' or ReaderName like '%"
               +condition+"%' or BookName like '%"+condition+"%' or kind like '%"+condition+"%' or BorrowTime like '%"+condition+"%')";
    }
    //qDebug()<<str;
    QSqlQuery sql(db);
    sql.exec(str);
    QVector<QVector<QVariant>> vector_1;
    while(sql.next())
    {
        QVector<QVariant> vector_2;
        for(int i=0;i<8;i++)
        {
            vector_2.push_back(sql.value(i));
        }
        vector_1.push_back(vector_2);
    }
    //获取管理员的还书信息
    if(power)
    {
        str="select RecordId,BookId,BookName,Kind,ManagerId,ManagerName,BorrowTime,Deadline from ManagerRecord "
              "where ReturnTime = '' and (ManagerId like '%"+condition+"%' or RecordId like '%"+condition+"%' or BookId like '%"+condition+"%' or ManagerName like '%"
              +condition+"%' or BookName like '%"+condition+"%' or kind like '%"+condition+"%' or BorrowTime like '%"+condition+"%')";
        sql.exec(str);
        while(sql.next())
        {
            QVector<QVariant> vector_2;
            for(int i=0;i<8;i++)
            {
                vector_2.push_back(sql.value(i));
            }
            vector_1.push_back(vector_2);
        }
    }
        return vector_1;
}

void MyDataBase::borrowBook(QString str1,QString str2,QString str3,QString str4)
{
    QString str5=Id;
    QString str6=name;
    QString str7=QDate::currentDate().toString("yyyy.M.d");
    QString str8="";
    QString str9="0";
    QString str10="30";
    QString str="";
    if(power)
    {
        str=QString::asprintf("insert into ManagerRecord values('%1','%2','%3','%4','%5','%6','%7','%8',%9,%10);"
                          "update Book set quantity=quantity-1 where BookId='%11';")
            .arg(str1).arg(str2).arg(str3).arg(str4).arg(str5).arg(str6).arg(str7)
            .arg(str8).arg(str9).arg(str10).arg(str2);
    }
    else
    {
        str=QString::asprintf("insert into ReaderRecord values('%1','%2','%3','%4','%5','%6','%7','%8',%9,%10);"
                          "update Book set quantity=quantity-1 where BookId='%11';")
            .arg(str1).arg(str2).arg(str3).arg(str4).arg(str5).arg(str6).arg(str7)
            .arg(str8).arg(str9).arg(str10).arg(str2);
    }
    QSqlQuery sql(db);
    sql.exec(str);
//    if(sql.exec(str))
//        qDebug()<<"借阅成功";
//    else
//    {
//        qDebug()<<"借阅失败";
//        qDebug()<<db.lastError().text();
//        qDebug()<<str;
//    }
}

void MyDataBase::returnBook(QString str1,QString str2,QString str3)
{
    QString str="update ";
    QString returnTime=QDate::currentDate().toString("yyyy.MM.dd");
    QSqlQuery sql(db);
    if(str3[0]=='U')
    {
        str+="ReaderRecord set ReturnTime = '"+returnTime+"' where RecordId = '"+str1+"'; update Book set quantity=quantity+1 where BookId='"+str2+"';";
    }
    else
    {
        str+="ManagerRecord set ReturnTime = '"+returnTime+"' where RecordId = '"+str1+"'; update Book set quantity=quantity+1 where BookId='"+str2+"';";
    }
    sql.exec(str);
//    if(sql.exec(str))
//        qDebug()<<"归还成功";
//    else
    //        qDebug()<<"归还失败";
}

void MyDataBase::deleteRecord(QString str1,QString str2)
{
    QString str="delete from ";
    QSqlQuery sql(db);
    if(str2[0]=='U')
    {
        str+="ReaderRecord where RecordId = '"+str1+"'";
    }
    else
    {
        str+="ManagerRecord where RecordId = '"+str1+"'";
    }
    sql.exec(str);
}

void MyDataBase::updateBook(QString str1, QString str2, QString str3, QString str4, QString str5, QString str6, QString str7)
{
    QString str=QString::asprintf("update book set bookName = '%2', author = '%3', press = '%4',"
                                    "publishTime = '%5', kind = '%6', quantity = '%7' "
                                    "where bookId = '%1';").arg(str1).arg(str2).arg(str3).arg(str4).arg(str5).arg(str6).arg(str7);
    QSqlQuery sql(db);
    if(sql.exec(str))
    {
        QMessageBox::information(nullptr,"提示","修改图书成功");
    }
    else
    {
        QMessageBox::information(nullptr,"提示","修改图书失败");
    }
}

void MyDataBase::addBook(QString str1, QString str2, QString str3, QString str4, QString str5, QString str6, QString str7)
{
    QString str=QString::asprintf("insert into book values ('%1','%2','%3','%4','%5','%6',%7);")
                      .arg(str1).arg(str2).arg(str3).arg(str4).arg(str5).arg(str6).arg(str7);
    QSqlQuery sql(db);
    if(sql.exec(str))
    {
        QMessageBox::information(nullptr,"提示","添加图书成功");
    }
    else
    {
        QMessageBox::information(nullptr,"提示","添加图书失败");
    }
}

void MyDataBase::deleteBook(QString str1)
{
    QString str=QString::asprintf("delete from book where bookId = '%1';").arg(str1);
    QSqlQuery sql(db);
    sql.exec(str);
}

void MyDataBase::addUser(QString str1, QString str2, QString str3, QString str4, QString str5, QString str6, QString str7, QString str8)
{
    QString str=QString::asprintf("insert into reader values ('%1','%2','%3','%4',%5,'%6',%7,'%8');")
                      .arg(str1).arg(str2).arg(str3).arg(str4).arg(str5).arg(str6).arg(str7).arg(str8);
    QSqlQuery sql(db);
    if(sql.exec(str))
    {
        QMessageBox::information(nullptr,"提示","添加用户成功");
    }
    else
    {
        QMessageBox::information(nullptr,"提示","添加用户失败");
    }
}

void MyDataBase::updateUser(QString str1, QString str2, QString str3, QString str4, QString str5, QString str6, QString str7, QString str8)
{
    QString str=QString::asprintf("update reader set myPassword = '%2',readerName = '%3',sex = '%4',"
                                "age = %5,registerTime = '%6',money = %7,contact = '%8' "
                                    "where id = '%1';").arg(str1).arg(str2).arg(str3).arg(str4).arg(str5).arg(str6).arg(str7).arg(str8);
    QSqlQuery sql(db);
    qDebug()<<str;
    if(sql.exec(str))
    {
        QMessageBox::information(nullptr,"提示","修改用户信息成功");
        if(str1==Id)
        {
            password=str2;
            name=str3;
            sex=str4;
            age=str5.toInt();
            registerTime=str6;
            money=str7.toDouble();
            contact=str8;
        }
    }
    else
    {
        QMessageBox::information(nullptr,"提示","修改用户信息失败");
    }
}

void MyDataBase::deleteUser(QString str1)
{
    QString str=QString::asprintf("delete from reader where id = '%1';").arg(str1);
    QSqlQuery sql(db);
    sql.exec(str);
}

void MyDataBase::updateManager(QString str1, QString str2, QString str3, QString str4, QString str5, QString str6, QString str7)
{
    QString str=QString::asprintf("update manager set myPassword = '%2',managerName = '%3',sex = '%4',"
                                    "age = %5,registerTime = '%6',contact = '%7' "
                                    "where id = '%1';").arg(str1).arg(str2).arg(str3).arg(str4).arg(str5).arg(str6).arg(str7);
    QSqlQuery sql(db);
    qDebug()<<str;
    if(sql.exec(str))
    {
        QMessageBox::information(nullptr,"提示","修改用户信息成功");
        if(str1==Id)
        {
            password=str2;
            name=str3;
            sex=str4;
            age=str5.toInt();
            registerTime=str6;
            contact=str7;
        }
    }
    else
    {
        QMessageBox::information(nullptr,"提示","修改用户信息失败");
    }
}

int MyDataBase::login(QString str1,QString str2)
{
    QString str="select * from reader where Id = '"+str1+"' and MyPassword = '"+str2+"';";
    QSqlQuery sql(db);
    sql.exec(str);
    //读者身份验证正确
    if(sql.next())
    {
        Id=sql.value(0).toString();
        password=sql.value(1).toString();
        name=sql.value(2).toString();
        sex=sql.value(3).toString();
        age=sql.value(4).toInt();
        registerTime=sql.value(5).toString();
        money=sql.value(6).toDouble();
        power=false;
        contact=sql.value(7).toString();
        return 0;
    }
    //读者身份验证失败
    else
    {
        str="select * from manager where Id = '"+str1+"' and MyPassword = '"+str2+"';";
        sql.exec(str);
        //管理员身份验证正确
        if(sql.next())
        {
            Id=sql.value(0).toString();
            password=sql.value(1).toString();
            name=sql.value(2).toString();
            sex=sql.value(3).toString();
            age=sql.value(4).toInt();
            registerTime=sql.value(5).toString();
            power=true;
            contact=sql.value(6).toString();
            return 1;
        }
        //既不是读者，也不是管理员
        else
            return -1;
    }
}

MyDataBase::MyDataBase()
{
    db = QSqlDatabase::addDatabase("QMYSQL");//添加MySql驱动
    db.setHostName("127.0.0.1"); //连接本地主机
    db.setPort(3306);//设置端口
    db.setDatabaseName("bookmanagesystem");//设置数据库名
    db.setUserName("root");//设置用户名
    db.setPassword("LJH030825");
    if (!db.open()){
        //QMessage::information函数的parent指针参数必须是QWidget及其子类的指针，而MyDataBase只继承了QObject类，所以这里不能使用this指针
        QMessageBox::information(nullptr, "提示", "link failed");
        qDebug()<<"error open database because"<<db.lastError().text();
        return;
    }
    //QMessageBox::information(nullptr, "提示", "link success");
}
