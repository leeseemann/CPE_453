#ifndef USERINFO_H
#define USERINFO_H

#include <QString>
#include <QVector>
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
    void message(QString,QVector<QString>);

};

#endif // USERINFO_H
