#include "bookmanager.h"
#include "ui_bookmanager.h"

BookManager::BookManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookManager)
{
    ui->setupUi(this);
    model=new QStandardItemModel(this);
    ui->tableView->setModel(model);
    setHeader();
    //设置无法编辑视图
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置视图的代理只能选中一行或者多行
    //哪怕选中整行依然可以准确的取得currentIndex，但是selectedIndexes会获取所有被选中的单元格的index
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    setConnect();
    Init(ui->search->text());
}

void BookManager::Init(QString condition)
{
    //重新加载表信息之前要清空model里的信息，不然会出现信息重复
    //但是调用clear函数清空model信息会导致设置好的表头信息也被清空，所以要在这里重新设置表头信息
    model->clear();
    setHeader();
    QVector<QVector<QVariant>> vector=MyDataBase::getDataBase()->getBook(condition);
    while(!vector.empty())
    {
        auto data=vector.front();
        vector.pop_front();
        QList<QStandardItem*> items;
        for(int i=0;i<7;i++)
        {
            QStandardItem* item=new QStandardItem(data[i].toString());
            items<<item;
        }
        model->appendRow(items);
    }
    //该函数只会根据当前model内的数据设置宽度，并不会随model内数据长度的变化而变化
    ui->tableView->resizeColumnToContents(1);
    ui->tableView->resizeColumnToContents(3);
    ui->tableView->resizeColumnToContents(4);
    //根据种类设置颜色
    int num=model->rowCount();
    for(int i=0;i<num;i++)
    {
        QString kind=model->item(i,5)->data(Qt::DisplayRole).toString();
        switch(getKind(kind))
        {
        case 0:
            for(int j=0;j<7;j++)
            {
                //两种方法都可以，在初始化QColor时直接使用rgb或者十六进制数字，不要将十六进制数值转换成字符串
                model->item(i,j)->setBackground(QBrush(QColor(0xFFFFCD)));
                //model->item(i,j)->setData(QBrush(QColor(0xFFFFCD)),Qt::BackgroundRole);
            }
            break;
        case 1:
            for(int j=0;j<7;j++)
            {
                //model->item(i,j)->setData(QBrush(QColor(0xFCE6C9)),Qt::BackgroundRole);
                model->item(i,j)->setBackground(QColor(0xFCE6C9));
            }
            break;
        case 2:
            for(int j=0;j<7;j++)
            {
                model->item(i,j)->setData(QBrush(QColor(0xB0E0E6)),Qt::BackgroundRole);
                //model->item(i,j)->setBackground(QColor(0xB0E0E6));
            }
            break;
        case 3:
            for(int j=0;j<7;j++)
            {
                model->item(i,j)->setData(QBrush(QColor(0xFF6100)),Qt::BackgroundRole);
                //model->item(i,j)->setBackground(QColor(0xFF6100));
            }
            break;
        case 4:
            for(int j=0;j<7;j++)
            {
                model->item(i,j)->setData(QBrush(QColor(0x708069)),Qt::BackgroundRole);
                //model->item(i,j)->setBackground(QColor(0x708069));
            }
            break;
        case 5:
            for(int j=0;j<7;j++)
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

void BookManager::setConnect()
{
    connect(ui->search,&QLineEdit::textChanged,this,&BookManager::Init);
    connect(ui->addBook,&QPushButton::clicked,this,&BookManager::addBook);
    connect(ui->updateBook,&QPushButton::clicked,this,&BookManager::updateBook);
    connect(ui->deleteBook,&QPushButton::clicked,this,&BookManager::deleteBook);
}

int BookManager::getKind(QString str1)
{
    QString str[6]={"古典文学","信息技术","美食杂志","现代散文","科幻小说","学术论文"};
    for(int i=0;i<6;i++)
    {
        if(str1==str[i])
            return i;
    }
}

void BookManager::setHeader()
{
    QStringList str;
    str<<"书籍号"<<"书名"<<"作者"<<"出版社"<<"出版日期"<<"分类"<<"数量";
    model->setHorizontalHeaderLabels(str);
}

BookManager::~BookManager()
{
    delete ui;
}

Ui::BookManager *BookManager::getUi()
{
    return ui;
}

//出了点问题，主键为空也能添加，目前原因未知
//空串和null不一样，设置为空串依旧可以创建，此时主键为空串，在book的对应函数中设置了条件限制来解决这个问题
void BookManager::addBook()
{
    Book book;
    book.exec();
    Init(ui->search->text());
}

void BookManager::updateBook()
{
//    auto outdata=ui->tableView->selectionModel()->selectedRows();
//    for(auto data:outdata)
//    {
//        qDebug()<<"row="<<data.row();
//        qDebug()<<"column="<<data.column();
//    }
//    qDebug()<<"row="<<ui->tableView->currentIndex().row();
//    qDebug()<<"column="<<ui->tableView->currentIndex().column();
    auto select=ui->tableView->selectionModel()->currentIndex();
    if(select.row()==-1)
    {
        QMessageBox::information(nullptr,"提示","没有选中要修改的图书");
        return;
    }
    QString str[7];
    for(int i=0;i<7;i++)
    {
        str[i]=model->item(select.row(),i)->data(Qt::DisplayRole).toString();
    }
    Book book(str[0],str[1],str[2],str[3],str[4],str[5],str[6]);
    book.exec();
    Init(ui->search->text());
}

//同样有点问题，有没有主键都能删除
//破案了，在myDataBase里没执行sql语句
void BookManager::deleteBook()
{
    auto selections=ui->tableView->selectionModel()->selectedRows();
    if(!selections.size())
    {
        QMessageBox::information(nullptr,"提示","没有选中要删除的图书");
        return;
    }
    MyDataBase::getDataBase()->db.transaction();
    for(auto selection:selections)
    {
        QString str=model->item(selection.row(),0)->data(Qt::DisplayRole).toString();
        MyDataBase::getDataBase()->deleteBook(str);
    }
    if(!MyDataBase::getDataBase()->db.commit())
    {
        QMessageBox::information(nullptr,"提示","删除图书成功");
    }
    else
    {
        QMessageBox::information(nullptr,"提示","删除图书失败");
    }
    Init(ui->search->text());
}
