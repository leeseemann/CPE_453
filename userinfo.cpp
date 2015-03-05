#include "userinfo.h"
#include <QDebug>

userInfo::userInfo()
{
}

void userInfo::message(QString alert)
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

    layout->addWidget(title);
    layout->addSpacing(15);
    layout->addWidget(message);
    layout->addSpacing(25);
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
