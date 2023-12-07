#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include "usermanager.h"
#include "bookmanager.h"
#include "recordmanager.h"
#include "borrowbook.h"
#include "returnbook.h"
#include "ui_usermanager.h"
#include "ui_bookmanager.h"
#include "ui_bookmanager.h"
#include "ui_recordmanager.h"
#include "ui_borrowbook.h"
#include "ui_returnbook.h"
#include "login.h"
#include "ui_login.h"
#include "personalmessage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class mainPage; }
QT_END_NAMESPACE

class MainPage : public QWidget
{
    Q_OBJECT

public:
    MainPage(Login *login=nullptr,QWidget *parent = nullptr);
    UserManager* userManager=nullptr;
    BookManager* bookManager=nullptr;
    PersonalMessage* personalMessage=nullptr;
    RecordManager* recordManager=nullptr;
    BorrowBook* borrowBook=nullptr;
    ReturnBook* returnBook=nullptr;
    Login* login=nullptr;
    void readerLogin();
    void managerLogin();
    void userKind(int kind);
    void setButtonId();
    void connectPage();
    ~MainPage();
public slots:
    void changePage();
private:
    Ui::mainPage *ui;
};
#endif // MAINPAGE_H
