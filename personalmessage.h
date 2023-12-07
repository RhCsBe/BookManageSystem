#ifndef PERSONALMESSAGE_H
#define PERSONALMESSAGE_H

#include <QWidget>
#include "mydatabase.h"

namespace Ui {
class PersonalMessage;
}

class PersonalMessage : public QWidget
{
    Q_OBJECT

public:
    explicit PersonalMessage(QWidget *parent = nullptr);
    bool power=false;
    void Init();
    void setEnabled(bool result);
    ~PersonalMessage();
public slots:
    void updatePersonalMessage();
private:
    Ui::PersonalMessage *ui;
};

#endif // PERSONALMESSAGE_H
