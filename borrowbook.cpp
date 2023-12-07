#include "borrowbook.h"
#include "ui_borrowbook.h"

BorrowBook::BorrowBook(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BorrowBook)
{
    ui->setupUi(this);
    model=new QStandardItemModel(this);
    ui->tableView->setModel(model);
    //设置无法编辑视图
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置视图的代理只能选中一行或者多行
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    setConnect();
    Init(ui->search->text());
}

void BorrowBook::setHeader()
{
    QStringList str;
    str<<"书籍号"<<"书名"<<"作者"<<"出版社"<<"出版日期"<<"分类"<<"数量";
        model->setHorizontalHeaderLabels(str);
}

void BorrowBook::Init(QString condition)
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

void BorrowBook::setConnect()
{
    connect(ui->borrowBook,&QPushButton::clicked,this,&BorrowBook::borrowBook);
    //因为
    connect(ui->search,&QLineEdit::textChanged,this,&BorrowBook::Init);
}

int BorrowBook::getKind(QString str1)
{
    QString str[6]={"古典文学","信息技术","美食杂志","现代散文","科幻小说","学术论文"};
    for(int i=0;i<6;i++)
    {
        if(str1==str[i])
            return i;
    }
}

BorrowBook::~BorrowBook()
{
    delete ui;
}

Ui::BorrowBook *BorrowBook::getUi()
{
    return ui;
}

void BorrowBook::borrowBook()
{
    auto selections=ui->tableView->selectionModel()->selectedRows();
    //qDebug()<<"AAA";
    if(selections.empty())
    {
        QMessageBox::information(nullptr,"提示","没有选中可以借阅的书籍");
    }
    MyDataBase::getDataBase()->db.transaction();
    for(auto selection:selections)
    {
//        qDebug()<<selection.row();
//        qDebug()<<selection.column();
//        qDebug()<<model->data(selection);
//        qDebug()<<model->item(selection.row(),1)->data(Qt::DisplayRole).toString();
//        qDebug()<<model->item(selection.row(),2)->data(Qt::DisplayRole).toString();
//        qDebug()<<model->item(selection.row(),3)->data(Qt::DisplayRole).toString();
//        qDebug()<<model->item(selection.row(),4)->data(Qt::DisplayRole).toString();
//        qDebug()<<model->item(selection.row(),5)->data(Qt::DisplayRole).toString();
//        qDebug()<<model->item(selection.row(),6)->data(Qt::DisplayRole).toString();
        //model的data函数的Role参数默认是QT::DisplayRole，而item的data函数的Role参数的默认参数是QT::UserRole+1
        if(model->item(selection.row(),6)->data(Qt::DisplayRole).toInt()==0)
            QMessageBox::information(nullptr,"提示",model->item(selection.row(),1)->data(Qt::DisplayRole).toString()+"当前馆藏量为0");
        else
        {
            //想想怎么从内部组件里获取外部组件的信息
            //不用想了，老子直接把user信息放到myDataBase里，每个组件都能直接访问
            std::srand(QTime::currentTime().msec());
            //QDate::currentDate().toString("yyyyMMdd");
            QString str1=QDate::currentDate().toString("yyyyMMdd")+QString::number(std::rand()%100000);//借阅号
            QString str2=(model->item(selection.row(),0)->data(Qt::DisplayRole).toString());//书籍号
            QString str3=(model->item(selection.row(),1)->data(Qt::DisplayRole).toString());//书名
            QString str4=(model->item(selection.row(),5)->data(Qt::DisplayRole).toString());//类型
            MyDataBase::getDataBase()->borrowBook(str1,str2,str3,str4);
        }
    }
    //这段傻逼代码不知道又出了傻逼问题，明明已经成功提交了，并且数据库数据也修改完毕，结果返回了false
    if(!MyDataBase::getDataBase()->db.commit())
        QMessageBox::information(nullptr,"提示","借阅成功");
    else
        QMessageBox::information(nullptr,"提示","借阅失败");
    Init(ui->search->text());
}

//void BorrowBook::searchBook(QString condition)
//{
//    Init(condition);
//}
