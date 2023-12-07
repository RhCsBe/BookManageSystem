QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    book.cpp \
    bookmanager.cpp \
    borrowbook.cpp \
    login.cpp \
    main.cpp \
    mainpage.cpp \
    mydatabase.cpp \
    personalmessage.cpp \
    recordmanager.cpp \
    returnbook.cpp \
    user.cpp \
    usermanager.cpp

HEADERS += \
    book.h \
    bookmanager.h \
    borrowbook.h \
    login.h \
    mainpage.h \
    mydatabase.h \
    personalmessage.h \
    recordmanager.h \
    returnbook.h \
    user.h \
    usermanager.h

FORMS += \
    book.ui \
    bookmanager.ui \
    borrowbook.ui \
    login.ui \
    mainpage.ui \
    personalmessage.ui \
    recordmanager.ui \
    returnbook.ui \
    user.ui \
    usermanager.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc
