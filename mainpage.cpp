#include "mainpage.h"
#include "ui_mainpage.h"

MainPage::MainPage(Login *login,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainPage)
{
    ui->setupUi(this);
    userManager=new UserManager(this);
    bookManager=new BookManager(this);
    personalMessage=new PersonalMessage(this);
    recordManager=new RecordManager(this);
    borrowBook=new BorrowBook(this);
    returnBook=new ReturnBook(this);
    ui->stack->addWidget(userManager);
    ui->stack->addWidget(bookManager);
    ui->stack->addWidget(personalMessage);
    ui->stack->addWidget(recordManager);
    ui->stack->addWidget(borrowBook);
    ui->stack->addWidget(returnBook);
    this->login=login;
    QPixmap pixmap(":/img/AHU.png");
    ui->label->setPixmap(pixmap.scaled(ui->label->width(),ui->label->height()));
    //stackWidget默认包含两个Page，如果不删除，要从2开始计数
    ui->stack->setCurrentIndex(0);
    //ui->stack->setCurrentWidget(userManager);
    setButtonId();
    connectPage();
}

void MainPage::readerLogin()
{
    ui->bookManage->setEnabled(false);
    ui->userManage->setEnabled(false);
    ui->personalMessage->click();
}

void MainPage::managerLogin()
{
    ui->bookManage->setEnabled(true);
    ui->userManage->setEnabled(true);
    ui->userManage->click();
}

void MainPage::userKind(int kind)
{
    if(kind)
        managerLogin();
    else
        readerLogin();
    userManager->Init("");
    bookManager->Init("");
    personalMessage->Init();
    recordManager->Init("");
    borrowBook->Init("");
    returnBook->Init("");
    this->show();
}

void MainPage::setButtonId()
{
    //在QTDesigner中添加进按键组的按键的ID全都是负数，从-2开始自减，在代码中添加的从-1开始自增
    auto btns=ui->buttonGroup->buttons();
    int i=0;
    for(auto btn:btns)
    {
        ui->buttonGroup->setId(btn,i);
        i++;
    }
}

void MainPage::connectPage()
{
    auto btns=ui->buttonGroup->buttons();
    for(auto btn:btns)
    {
        connect(static_cast<QPushButton*>(btn),&QPushButton::clicked,this,&MainPage::changePage);
    }
    connect(ui->returnLogin,&QPushButton::clicked,this,[this](){this->close();login->getUi()->password->setText("");login->exec();});
}

MainPage::~MainPage()
{
    delete ui;
}

void MainPage::changePage()
{
    ui->stack->setCurrentIndex(ui->buttonGroup->checkedId());
    switch(ui->stack->currentIndex())
    {
    case 0:
        //调用setText如果没有修改Text是不会触发textChanged信号的
        userManager->getUi()->search->setText("");
        userManager->Init("");
        break;
    case 1:
        bookManager->getUi()->search->setText("");
        bookManager->Init("");
        break;
    case 2:
        personalMessage->Init();
        break;
    case 3:
        recordManager->getUi()->search->setText("");
        recordManager->Init("");
        break;
    case 4:
        borrowBook->getUi()->search->setText("");
        borrowBook->Init("");
        break;
    case 5:
        returnBook->getUi()->search->setText("");
        returnBook->Init("");
        break;
    default:
        break;
    }
}

