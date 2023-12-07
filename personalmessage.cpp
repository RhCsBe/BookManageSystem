#include "personalmessage.h"
#include "ui_personalmessage.h"

PersonalMessage::PersonalMessage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonalMessage)
{
    ui->setupUi(this);
    connect(ui->OK,&QPushButton::clicked,this,&PersonalMessage::updatePersonalMessage);
}

void PersonalMessage::Init()
{
    ui->id->setText(MyDataBase::getDataBase()->Id);
    ui->password->setText(MyDataBase::getDataBase()->password);
    ui->name->setText(MyDataBase::getDataBase()->name);
    ui->sex->setCurrentText(MyDataBase::getDataBase()->sex);
    ui->age->setValue(MyDataBase::getDataBase()->age);
    power=MyDataBase::getDataBase()->power;
    if(power)
    {
        ui->money->hide();
        ui->moneyLabel->hide();
        ui->power->setText("管理员");
    }
    else
    {
        ui->money->setHidden(false);
        ui->moneyLabel->setHidden(false);
        ui->money->setValue(MyDataBase::getDataBase()->money);
        ui->power->setText("读者");
    }
    ui->registerTime->setText(MyDataBase::getDataBase()->registerTime);
    ui->contact->setText(MyDataBase::getDataBase()->contact);
    setEnabled(false);
    ui->OK->setText("修改信息");
}

void PersonalMessage::setEnabled(bool result)
{
    ui->password->setEnabled(result);
    ui->name->setEnabled(result);
    ui->sex->setEnabled(result);
    ui->age->setEnabled(result);
    ui->money->setEnabled(result);
    ui->contact->setEnabled(result);
}

PersonalMessage::~PersonalMessage()
{
    delete ui;
}

void PersonalMessage::updatePersonalMessage()
{
    if(ui->OK->text()=="修改信息")
    {
        setEnabled(true);
        ui->OK->setText("确认修改");
    }
    else
    {
        if(ui->id->text().isEmpty()||ui->password->text().isEmpty()||ui->name->text().isEmpty()||ui->contact->text().isEmpty())
        {
            QMessageBox::information(nullptr,"提示","数据不全，修改个人信息失败");
            return;
        }
        else if(ui->password->text().length()<8||ui->password->text().length()>16)
        {
            QMessageBox::information(nullptr,"提示","密码必须是八至十六位的数字或者英文字母");
            return;
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
                    QMessageBox::information(nullptr,"提示","密码必须是八至十六位的数字或者英文字母");
                    return;
                }
            }
        }
        if(power)
        {
            MyDataBase::getDataBase()->updateManager(ui->id->text(),ui->password->text(),ui->name->text(),ui->sex->currentText(),QString::number(ui->age->value()),ui->registerTime->text(),ui->contact->text());
        }
        else
        {
            MyDataBase::getDataBase()->updateUser(ui->id->text(),ui->password->text(),ui->name->text(),ui->sex->currentText(),QString::number(ui->age->value()),ui->registerTime->text(),QString::number(ui->money->value()),ui->contact->text());
        }
        //QMessageBox::information(nullptr,"提示","修改个人信息成功");
        setEnabled(false);
        ui->OK->setText("修改信息");
    }
}
