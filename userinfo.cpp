#include "userinfo.h"
#include <QDebug>
#include <QListWidget>

userInfo::userInfo()
{
}

void userInfo::message(QString alert)
{
    QDialog user_alert;
    QVBoxLayout* layout = new QVBoxLayout;
    //QString Err;
    //int ErrStackSize = ErrorStack.getStackSize();

    QLabel* title = new QLabel("WARNING");
    title->setAlignment(Qt::AlignHCenter);
    title->setFont(QFont("Helvetica", 16, QFont::Bold));

    QLabel* message = new QLabel(alert);
    message->setAlignment(Qt::AlignHCenter);
    message->setFont(QFont("Helvetica", 12));

    //QListWidget* errors = new QListWidget();

    //for(int iter = 0; iter < ErrStackSize; iter++)
    //{
    //    Err = ErrorStack.getStack();
    //    errors->addItem();
    //}

    layout->addWidget(title);
    layout->addSpacing(15);
    layout->addWidget(message);
    layout->addSpacing(25);
    //layout->addWidget(errors);
    //layout->addSpacing(25);

  //  layout->addWidget(accept);

    user_alert.setLayout(layout);

    user_alert.exec();
}

void userInfo::message(QSqlError alert)
{
    QDialog user_alert;
    QVBoxLayout* layout = new QVBoxLayout;

    QLabel* title = new QLabel("WARNING");
    title->setAlignment(Qt::AlignHCenter);
    title->setFont(QFont("Helvetica", 16, QFont::Bold));

    QLabel* message = new QLabel(alert.text());
    message->setAlignment(Qt::AlignHCenter);
    message->setFont(QFont("Helvetica", 12));

    layout->addWidget(title);
    layout->addSpacing(15);
    layout->addWidget(message);
    layout->addSpacing(25);

    user_alert.setLayout(layout);

    user_alert.exec();
}

void userInfo::message(QString alert, QVector<QString> errorlist)
{
    QDialog user_alert;
    QVBoxLayout* layout = new QVBoxLayout;

    QLabel* title = new QLabel("WARNING");
    title->setAlignment(Qt::AlignHCenter);
    title->setFont(QFont("Helvetica", 16, QFont::Bold));

   // QPushButton* accept = new QPushButton("Continue");

    QLabel* message = new QLabel(alert);
    message->setAlignment(Qt::AlignHCenter);
    message->setFont(QFont("Helvetica", 12));

    QLabel* err = new QLabel("List of Errors");
    err->setAlignment(Qt::AlignLeft);
    err->setFont(QFont("Helvetica",12));

    QListWidget* errors = new QListWidget();

    QString temp;
    while(!errorlist.empty())
    {
        temp = errorlist.takeFirst();
        errors->addItem(temp);
    }


    layout->addWidget(title);
    layout->addSpacing(15);
    layout->addWidget(message);
    layout->addSpacing(25);
    layout->addWidget(err);
    layout->addSpacing(5);
    layout->addWidget(errors);
    layout->addSpacing(25);

   //  layout->addWidget(accept);

    user_alert.setLayout(layout);

    user_alert.exec();
}
