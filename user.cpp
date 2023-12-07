#include "user.h"
#include "ui_user.h"

User::User(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::User)
{
    ui->setupUi(this);
    ui->registerTime->setText(ui->calendar->selectedDate().toString("yyyy.M.d"));
    connect(ui->calendar,&QCalendarWidget::clicked,this,&User::setRegisterTime);
    //添加用户时，为OK按键绑定添加用户功能
    connect(ui->OK,&QPushButton::clicked,this,&User::addUser);
}

User::User(QString id, QString myPassword, QString name, QString sex, QString age, QString registerTime, QString money, QString contact, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::User)
{
    ui->setupUi(this);
    ui->userId->setText(id);
    ui->password->setText(myPassword);
    ui->name->setText(name);
    ui->sex->setCurrentText(sex);
    ui->age->setValue(age.toInt());
    ui->registerTime->setText(registerTime);
    ui->money->setValue(money.toDouble());
    ui->contact->setText(contact);
    //用户账号只有在建立的时候才能设置，一旦确定，不可修改
    ui->userId->setEnabled(false);
    connect(ui->calendar,&QCalendarWidget::clicked,this,&User::setRegisterTime);
    //修改用户时，为OK按键绑定修改用户功能
    connect(ui->OK,&QPushButton::clicked,this,&User::updateUser);
}

User::~User()
{
    delete ui;
}

void User::setRegisterTime(const QDate &data)
{
    ui->registerTime->setText(data.toString("yyyy.M.d"));
}

void User::addUser()
{
    if(ui->userId->text().isEmpty()||ui->password->text().isEmpty()||ui->name->text().isEmpty()||ui->contact->text().isEmpty())
    {
        QMessageBox::information(nullptr,"提示","数据不全，添加用户失败");
    }
    else if(ui->userId->text()[0]!='U'||ui->userId->text().length()!=8)
    {
        QMessageBox::information(nullptr,"提示","用户账号必须是以U开头加七位数字的八位字符串");
    }
    else if(ui->password->text().length()<8||ui->password->text().length()>16)
    {
        QMessageBox::information(nullptr,"提示","用户密码必须是八至十六位的数字或者英文字母");
    }
    else
    {
        //bool first=false,second=false;
        for(int i=1;i<ui->userId->text().length();i++)
        {
            if(ui->userId->text()[i]<'0'||ui->userId->text()[i]>'9')
            {
                QMessageBox::information(nullptr,"提示","用户账号必须是以U开头加七位数字的八位字符串");
                close();
                return ;
            }
        }
        for(int i=0;i<ui->password->text().length();i++)
        {
            if(ui->password->text()[i]>='0'&&ui->password->text()[i]<='9'||ui->password->text()[i]>='a'&&ui->password->text()[i]<='z'||ui->password->text()[i]>='A'&&ui->password->text()[i]<='Z')
            {
                ;
            }
            else
            {
                QMessageBox::information(nullptr,"提示","用户密码必须是八至十六位的数字或者英文字母");
                close();
                return;
            }
        }
        MyDataBase::getDataBase()->addUser(ui->userId->text(),ui->password->text(),ui->name->text(),ui->sex->currentText(),QString::number(ui->age->value()),ui->registerTime->text(),QString::number(ui->money->value()),ui->contact->text());
    }
    close();
}

void User::updateUser()
{
    if(ui->userId->text().isEmpty()||ui->password->text().isEmpty()||ui->name->text().isEmpty()||ui->contact->text().isEmpty())
    {
        QMessageBox::information(nullptr,"提示","数据不全，修改用户失败");
    }
    else if(ui->password->text().length()<8||ui->password->text().length()>16)
    {
        QMessageBox::information(nullptr,"提示","用户密码必须是八至十六位的数字或者英文字母");
    }
    else
    {
        for(int i=0;i<ui->password->text().length();i++)
        {
            if(ui->password->text()[i]>='0'&&ui->password->text()[i]<='9'||ui->password->text()[i]>='a'&&ui->password->text()[i]<='z'||ui->password->text()[i]>='A'&&ui->password->text()[i]<='Z')
            {
                ;
            }
            else
            {
                QMessageBox::information(nullptr,"提示","用户密码必须是八至十六位的数字或者英文字母");
                close();
                return;
            }
        }
        MyDataBase::getDataBase()->updateUser(ui->userId->text(),ui->password->text(),ui->name->text(),ui->sex->currentText(),QString::number(ui->age->value()),ui->registerTime->text(),QString::number(ui->money->value()),ui->contact->text());
    }
    close();
}

