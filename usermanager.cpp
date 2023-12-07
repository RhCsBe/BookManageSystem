#include "usermanager.h"
#include "ui_usermanager.h"

UserManager::UserManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserManager)
{
    ui->setupUi(this);
    model=new QStandardItemModel(this);
    ui->tableView->setModel(model);
    setHeader();
    //设置无法编辑视图
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置视图的代理只能选中一行或者多行
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    setConnect();
    Init(ui->search->text());
}

void UserManager::setHeader()
{
    QStringList str;
    str<<"账号"<<"密码"<<"姓名"<<"性别"<<"年龄"<<"注册时间"<<"余额"<<"联系方式";
    model->setHorizontalHeaderLabels(str);
}

void UserManager::Init(QString condition)
{
    //重新加载表信息之前要清空model里的信息，不然会出现信息重复
    //但是调用clear函数清空model信息会导致设置好的表头信息也被清空，所以要在这里重新设置表头信息
    model->clear();
    setHeader();
    QVector<QVector<QVariant>> vector=MyDataBase::getDataBase()->getReader(condition);
    while(!vector.empty())
    {
        auto data=vector.front();
        vector.pop_front();
        QList<QStandardItem*> items;
        for(int i=0;i<8;i++)
        {
            QStandardItem* item=new QStandardItem(data[i].toString());
            items<<item;
        }
        model->appendRow(items);
    }
    ui->tableView->resizeColumnToContents(7);
}

void UserManager::setConnect()
{
    connect(ui->search,&QLineEdit::textChanged,this,&UserManager::Init);
    connect(ui->addUser,&QPushButton::clicked,this,&UserManager::addUser);
    connect(ui->updateUser,&QPushButton::clicked,this,&UserManager::updateUser);
    connect(ui->deleteUser,&QPushButton::clicked,this,&UserManager::deleteUser);
}

UserManager::~UserManager()
{
    delete ui;
}

Ui::UserManager *UserManager::getUi()
{
    return ui;
}

void UserManager::addUser()
{
    User user;
    user.exec();
    Init(ui->search->text());
}

void UserManager::updateUser()
{
    auto select=ui->tableView->selectionModel()->currentIndex();
    if(select.row()==-1)
    {
        QMessageBox::information(nullptr,"提示","没有选中要修改的用户");
        return;
    }
    QString str[8];
    for(int i=0;i<8;i++)
    {
        str[i]=model->item(select.row(),i)->data(Qt::DisplayRole).toString();
    }
    User user(str[0],str[1],str[2],str[3],str[4],str[5],str[6],str[7]);
    user.exec();
    Init(ui->search->text());
}

void UserManager::deleteUser()
{
    auto selections=ui->tableView->selectionModel()->selectedRows();
    if(!selections.size())
    {
        QMessageBox::information(nullptr,"提示","没有选中要删除的用户");
        return;
    }
    MyDataBase::getDataBase()->db.transaction();
    for(auto selection:selections)
    {
        QString str=model->item(selection.row(),0)->data(Qt::DisplayRole).toString();
        MyDataBase::getDataBase()->deleteUser(str);
    }
    if(!MyDataBase::getDataBase()->db.commit())
    {
        QMessageBox::information(nullptr,"提示","删除用户成功");
    }
    else
    {
        QMessageBox::information(nullptr,"提示","删除用户失败");
    }
    Init(ui->search->text());
}
