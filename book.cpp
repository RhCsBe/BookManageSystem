#include "book.h"
#include "ui_book.h"

Book::Book(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Book)
{
    ui->setupUi(this);
    ui->publishTime->setText(ui->calendar->selectedDate().toString("yyyy.M.d"));
    connect(ui->calendar,&QCalendarWidget::clicked,this,&Book::setPublishTime);
    //添加图书时，为OK按键绑定添加图书功能
    connect(ui->OK,&QPushButton::clicked,this,&Book::addBook);
}

Book::Book(QString bookId, QString booKName, QString author, QString press, QString publishTime, QString kind, QString quantity, QWidget *parent):
    QDialog(parent),
    ui(new Ui::Book)
{
    ui->setupUi(this);
    ui->bookId->setText(bookId);
    ui->bookName->setText(booKName);
    ui->author->setText(author);
    ui->press->setText(press);
    ui->publishTime->setText(publishTime);
    ui->kind->setCurrentText(kind);
    ui->quantity->setValue(quantity.toInt());
    //书籍号只有在添加图书时才能设置，一旦确定，不可修改
    ui->bookId->setEnabled(false);
    connect(ui->calendar,&QCalendarWidget::clicked,this,&Book::setPublishTime);
    //修改图书时，为OK按键绑定修改图书功能
    connect(ui->OK,&QPushButton::clicked,this,&Book::updateBook);
}


Book::~Book()
{
    delete ui;
}

void Book::setPublishTime(const QDate &date)
{
    ui->publishTime->setText(date.toString("yyyy.M.d"));
}

void Book::updateBook()
{
    if(ui->bookId->text().isEmpty()||ui->bookName->text().isEmpty()||ui->author->text().isEmpty()||ui->press->text().isEmpty())
    {
        QMessageBox::information(nullptr,"提示","数据不全，修改书籍失败");
    }
    else
    {
        MyDataBase::getDataBase()->updateBook(ui->bookId->text(),ui->bookName->text(),ui->author->text(),ui->press->text(),ui->publishTime->text(),ui->kind->currentText(),QString::number(ui->quantity->value()));
    }
    this->close();
}

void Book::addBook()
{
    if(ui->bookId->text().isEmpty()||ui->bookName->text().isEmpty()||ui->author->text().isEmpty()||ui->press->text().isEmpty())
    {
        QMessageBox::information(nullptr,"提示","数据不全，添加书籍失败");
    }
    else
    {
        MyDataBase::getDataBase()->addBook(ui->bookId->text(),ui->bookName->text(),ui->author->text(),ui->press->text(),ui->publishTime->text(),ui->kind->currentText(),QString::number(ui->quantity->value()));
    }
    this->close();
}
