#include "returnbook.h"
#include "ui_returnbook.h"

ReturnBook::ReturnBook(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReturnBook)
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

void ReturnBook::setHeader()
{
    QStringList str;
    str<<"借阅号"<<"书籍号"<<"书名"<<"类型"<<"读者ID"<<"读者"<<"借阅时间"<<"借阅期限";
    model->setHorizontalHeaderLabels(str);
}

void ReturnBook::Init(QString condition)
{
    //重新加载表信息之前要清空model里的信息，不然会出现信息重复
    //但是调用clear函数清空model信息会导致设置好的表头信息也被清空，所以要在这里重新设置表头信息
    model->clear();
    setHeader();
    QVector<QVector<QVariant>> vector=MyDataBase::getDataBase()->getReturnRecord(condition);
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
    ui->tableView->resizeColumnToContents(0);
    //ui->tableView->resizeColumnToContents(1);
    ui->tableView->resizeColumnToContents(2);
    ui->tableView->resizeColumnToContents(4);
    ui->tableView->resizeColumnToContents(6);
    //根据种类设置颜色
    int num=model->rowCount();
    for(int i=0;i<num;i++)
    {
        QString kind=model->item(i,3)->data(Qt::DisplayRole).toString();
        switch(getKind(kind))
        {
        case 0:
            for(int j=0;j<8;j++)
            {
                //两种方法都可以，在初始化QColor时直接使用rgb或者十六进制数字，不要将十六进制数值转换成字符串
                model->item(i,j)->setBackground(QBrush(QColor(0xFFFFCD)));
                //model->item(i,j)->setData(QBrush(QColor(0xFFFFCD)),Qt::BackgroundRole);
            }
            break;
        case 1:
            for(int j=0;j<8;j++)
            {
                //model->item(i,j)->setData(QBrush(QColor(0xFCE6C9)),Qt::BackgroundRole);
                model->item(i,j)->setBackground(QColor(0xFCE6C9));
            }
            break;
        case 2:
            for(int j=0;j<8;j++)
            {
                model->item(i,j)->setData(QBrush(QColor(0xB0E0E6)),Qt::BackgroundRole);
                //model->item(i,j)->setBackground(QColor(0xB0E0E6));
            }
            break;
        case 3:
            for(int j=0;j<8;j++)
            {
                model->item(i,j)->setData(QBrush(QColor(0xFF6100)),Qt::BackgroundRole);
                //model->item(i,j)->setBackground(QColor(0xFF6100));
            }
            break;
        case 4:
            for(int j=0;j<8;j++)
            {
                model->item(i,j)->setData(QBrush(QColor(0x708069)),Qt::BackgroundRole);
                //model->item(i,j)->setBackground(QColor(0x708069));
            }
            break;
        case 5:
            for(int j=0;j<8;j++)
            {
                model->item(i,j)->setData(QBrush(QColor(0xC0C0C0)),Qt::BackgroundRole);
                //model->item(i,j)->setBackground(QColor(0xC0C0C0));
            }
            break;
        default:
            break;
        }
    }
}

void ReturnBook::setConnect()
{
    connect(ui->search,&QLineEdit::textChanged,this,&ReturnBook::Init);
    connect(ui->returnOne,&QPushButton::clicked,this,&ReturnBook::returnOne);
    connect(ui->returnAll,&QPushButton::clicked,this,&ReturnBook::returnAll);
}

int ReturnBook::getKind(QString str1)
{
    QString str[6]={"古典文学","信息技术","美食杂志","现代散文","科幻小说","学术论文"};
    for(int i=0;i<6;i++)
    {
        if(str1==str[i])
            return i;
    }
}

void ReturnBook::returnOne()
{
    auto selections=ui->tableView->selectionModel()->selectedRows();
    if(selections.empty())
    {
        QMessageBox::information(nullptr,"提示","没有选中可以归还的书籍");
    }
    MyDataBase::getDataBase()->db.transaction();
    for(auto selection:selections)
    {
        QString str1=model->item(selection.row(),0)->data(Qt::DisplayRole).toString();
        QString str2=model->item(selection.row(),1)->data(Qt::DisplayRole).toString();
        QString str3=model->item(selection.row(),4)->data(Qt::DisplayRole).toString();
        MyDataBase::getDataBase()->returnBook(str1,str2,str3);
    }
    //还是相同问题
    if(!MyDataBase::getDataBase()->db.commit())
    {
        QMessageBox::information(nullptr,"提示","归还成功");
    }
    else
    {
        QMessageBox::information(nullptr,"提示","归还失败");
    }
    Init(ui->search->text());
}

void ReturnBook::returnAll()
{
    MyDataBase::getDataBase()->db.transaction();
    for(int i=0;i<model->rowCount();i++)
    {
        QString str1=model->item(i,0)->data(Qt::DisplayRole).toString();
        QString str2=model->item(i,1)->data(Qt::DisplayRole).toString();
        QString str3=model->item(i,4)->data(Qt::DisplayRole).toString();
        MyDataBase::getDataBase()->returnBook(str1,str2,str3);
    }
    //还是有相同原因
    if(!MyDataBase::getDataBase()->db.commit())
    {
        QMessageBox::information(nullptr,"提示","归还成功");
    }
    else
    {
        QMessageBox::information(nullptr,"提示","归还失败");
    }
    Init(ui->search->text());
}

ReturnBook::~ReturnBook()
{
    delete ui;
}

Ui::ReturnBook *ReturnBook::getUi()
{
    return ui;
}
