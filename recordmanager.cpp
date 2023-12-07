#include "recordmanager.h"
#include "ui_recordmanager.h"

RecordManager::RecordManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecordManager)
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

void RecordManager::setHeader()
{
    QStringList str;
    str<<"借阅号"<<"书籍号"<<"书名"<<"类型"<<"读者ID"<<"读者"<<"借阅时间"<<"归还时间"<<"逾期罚款"<<"借阅期限";
    model->setHorizontalHeaderLabels(str);
}

void RecordManager::Init(QString condition)
{
    //重新加载表信息之前要清空model里的信息，不然会出现信息重复
    //但是调用clear函数清空model信息会导致设置好的表头信息也被清空，所以要在这里重新设置表头信息
    model->clear();
    setHeader();
    QVector<QVector<QVariant>> vector=MyDataBase::getDataBase()->getRecord(condition);
    while(!vector.empty())
    {
        auto data=vector.front();
        vector.pop_front();
        QList<QStandardItem*> items;
        for(int i=0;i<10;i++)
        {
            QStandardItem* item=new QStandardItem(data[i].toString());
            items<<item;
        }
        model->appendRow(items);
    }
    ui->tableView->resizeColumnToContents(0);
    ui->tableView->resizeColumnToContents(2);
    ui->tableView->resizeColumnToContents(4);
    ui->tableView->resizeColumnToContents(6);
    //因为归还时间可能全部为空，所以将归还时间那一列的宽度设置为跟借阅时间一致，而不是直接设置为被内容撑开
    ui->tableView->setColumnWidth(7,ui->tableView->columnWidth(6));
    //根据种类设置颜色
    int num=model->rowCount();
    for(int i=0;i<num;i++)
    {
        QString kind=model->item(i,3)->data(Qt::DisplayRole).toString();
        switch(getKind(kind))
        {
        case 0:
            for(int j=0;j<10;j++)
            {
                //两种方法都可以，在初始化QColor时直接使用rgb或者十六进制数字，不要将十六进制数值转换成字符串
                model->item(i,j)->setBackground(QBrush(QColor(0xFFFFCD)));
                //model->item(i,j)->setData(QBrush(QColor(0xFFFFCD)),Qt::BackgroundRole);
            }
            break;
        case 1:
            for(int j=0;j<10;j++)
            {
                //model->item(i,j)->setData(QBrush(QColor(0xFCE6C9)),Qt::BackgroundRole);
                model->item(i,j)->setBackground(QColor(0xFCE6C9));
            }
            break;
        case 2:
            for(int j=0;j<10;j++)
            {
                model->item(i,j)->setData(QBrush(QColor(0xB0E0E6)),Qt::BackgroundRole);
                //model->item(i,j)->setBackground(QColor(0xB0E0E6));
            }
            break;
        case 3:
            for(int j=0;j<10;j++)
            {
                model->item(i,j)->setData(QBrush(QColor(0xFF6100)),Qt::BackgroundRole);
                //model->item(i,j)->setBackground(QColor(0xFF6100));
            }
            break;
        case 4:
            for(int j=0;j<10;j++)
            {
                model->item(i,j)->setData(QBrush(QColor(0x708069)),Qt::BackgroundRole);
                //model->item(i,j)->setBackground(QColor(0x708069));
            }
            break;
        case 5:
            for(int j=0;j<10;j++)
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

void RecordManager::setConnect()
{
    connect(ui->search,&QLineEdit::textChanged,this,&RecordManager::Init);
    connect(ui->deleteOne,&QPushButton::clicked,this,&RecordManager::deleteOne);
    connect(ui->deleteAll,&QPushButton::clicked,this,&RecordManager::deleteAll);
}

int RecordManager::getKind(QString str1)
{
    QString str[6]={"古典文学","信息技术","美食杂志","现代散文","科幻小说","学术论文"};
    for(int i=0;i<6;i++)
    {
        if(str1==str[i])
            return i;
    }
}

RecordManager::~RecordManager()
{
    delete ui;
}

Ui::RecordManager *RecordManager::getUi()
{
    return ui;
}

void RecordManager::deleteOne()
{
    auto selections=ui->tableView->selectionModel()->selectedRows();
    MyDataBase::getDataBase()->db.transaction();
    for(auto selection:selections)
    {
        if(model->item(selection.row(),7)->data(Qt::DisplayRole).toString().isEmpty())
        {
            QMessageBox::information(nullptr,"提示",model->item(selection.row(),2)->data(Qt::DisplayRole).toString()+"尚未归还，记录无法删除");
            //如果只选中了一条记录，且该记录没有归还时间，直接在此处退出，防止触发下方的消息弹窗
            if(selections.size()==1)
                return;
            continue;
        }
        QString str1=model->item(selection.row(),0)->data(Qt::DisplayRole).toString();
        QString str2=model->item(selection.row(),4)->data(Qt::DisplayRole).toString();
        MyDataBase::getDataBase()->deleteRecord(str1,str2);
    }
    if(!MyDataBase::getDataBase()->db.commit())
    {
        QMessageBox::information(nullptr,"提示","删除成功");
    }
    else
    {
        QMessageBox::information(nullptr,"提示","删除失败");
    }
    Init(ui->search->text());
}

void RecordManager::deleteAll()
{
    MyDataBase::getDataBase()->db.transaction();
    for(int i=0;i<model->rowCount();i++)
    {
        if(model->item(i,7)->data(Qt::DisplayRole).toString().isEmpty())
        {
            continue;
        }
        else
        {
            QString str1=model->item(i,0)->data(Qt::DisplayRole).toString();
            QString str2=model->item(i,4)->data(Qt::DisplayRole).toString();
            MyDataBase::getDataBase()->deleteRecord(str1,str2);
        }
    }
    if(!MyDataBase::getDataBase()->db.commit())
    {
        QMessageBox::information(nullptr,"提示","记录已清空");
    }
    else
    {
        QMessageBox::information(nullptr,"提示","清空记录失败");
    }
    Init(ui->search->text());
}
