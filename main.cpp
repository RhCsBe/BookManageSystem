#include "mainpage.h"
#include "login.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login login;
    MainPage w(&login);
    QObject::connect(&login,&Login::userKind,&w,&MainPage::userKind);
    login.exec();
    //w.show();
    return a.exec();
}
