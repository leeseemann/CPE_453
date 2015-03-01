#ifndef USERINFO_H
#define USERINFO_H

#include <QString>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QVBoxLayout>
#include <QSqlError>

class userInfo
{
public:
    userInfo();
    void message(QString);
    void message(QSqlError);

};

#endif // USERINFO_H
