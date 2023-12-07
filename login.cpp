#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    connect(ui->login,&QPushButton::clicked,this,&Login::login);
}

Ui::Login *Login::getUi()
{
    return ui;
}

Login::~Login()
{
    delete ui;
}

void Login::login()
{
    if(ui->acount->text().isEmpty()||ui->password->text().isEmpty())
    {
        QMessageBox::information(nullptr,"提示","账号和密码不能为空");
        return;
    }
    else
    {
        int result=MyDataBase::getDataBase()->login(ui->acount->text(),ui->password->text());
        if(result==-1)
        {
            QMessageBox::information(nullptr,"提示","账号或密码不正确");
            return;
        }
        else
        {
            emit userKind(result);
            done(result);
        }
    }
}
