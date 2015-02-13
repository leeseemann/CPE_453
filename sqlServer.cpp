#include "sqlServer.h"

sqlServer::sqlServer(QObject *parent) :
    QObject(parent)
{
}

void sqlServer::sql_connect(QString db_type, QString db_host, int db_port, QString db_name, QString db_username, QString db_password)
{
    QSqlDatabase db = QSqlDatabase::addDatabase(db_type); // possible issue using QString
    db.setHostName(db_host);
    db.setPort(db_port);
    db.setDatabaseName(db_name);
    db.setUserName(db_username);
    db.setPassword(db_password);

    if(!db.open())
        qDebug() << "Database was not opened successfully";
    else
        qDebug() << "Database was opened successfully";

    db.close();

}




