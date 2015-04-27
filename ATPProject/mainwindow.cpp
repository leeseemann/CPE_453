#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include <QListWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sql_pavelow(); // allow the user to create a custom connection or connect to pavelow
    connect(ui->btnTest,SIGNAL(clicked()),this,SLOT(acceptance_test()));
    connect(ui->btnTest, SIGNAL(clicked()), this, SLOT(next_test()));

    init_database();

}

MainWindow::~MainWindow()
{
    QString queryPrep2 = "DROP TABLE track_ds";
    QSqlQuery query299 (queryPrep2, fqt_test);
    if(!query299.exec())
        qDebug() << "Query Error: " << query299.lastError();

    queryPrep2 = "DROP TABLE track_power";
    QSqlQuery query300 (queryPrep2, fqt_test);
    if(!query300.exec())
        qDebug() << "Query Error: " << query300.lastError();

    queryPrep2 = "DROP TABLE track_switch_5a";
    QSqlQuery query301 (queryPrep2, fqt_test);
    if(!query301.exec())
        qDebug() << "Query Error: " << query301.lastError();

    queryPrep2 = "DROP TABLE scheduled_train_info";
    QSqlQuery query302 (queryPrep2, fqt_test);
    if(!query302.exec())
        qDebug() << "Query Error: " << query302.lastError();

    queryPrep2 = "DROP TABLE scheduled_routes";
    QSqlQuery query303 (queryPrep2, fqt_test);
    if(!query303.exec())
        qDebug() << "Query Error: " << query303.lastError();

    team4b.close();
    fqt_test.close();

    delete ui;
}

void MainWindow::sql_pavelow()
{
    // this dialog allows the user to select the default connection to pavelow or use a custom sql database connection
    QDialog* pavelow = new QDialog;
    QVBoxLayout* layout = new QVBoxLayout;

    QPushButton* use_pavelow = new QPushButton;
    use_pavelow->setText("Connect to Pavelow");

    QPushButton* custom_sql = new QPushButton;
    custom_sql->setText("Create Custom Connection");

    QLabel* title = new QLabel;
    title->setText("Choose an SQL Database Connection");

    // connect the custom_sql button to the dialog that allows the user to enter database info
    connect(custom_sql, SIGNAL(clicked()), this, SLOT(sql_information()));
    connect(custom_sql, SIGNAL(clicked()), pavelow, SLOT(close()));

    // connect the use_pavelow button to the function which will create the needed connections to pavelow automatically
    connect(use_pavelow, SIGNAL(clicked()), this, SLOT(connect_pavelow()));
    connect(use_pavelow, SIGNAL(clicked()), pavelow, SLOT(close()));

  //  connect(ui->btnTest, )

    layout->addWidget(title);
    layout->addWidget(use_pavelow);
    layout->addWidget(custom_sql);

    pavelow->setLayout(layout);
    pavelow->exec();
}

void MainWindow::connect_pavelow()
{
    // create connections to the individual sql databases that are needed to populate the display
    team4b = QSqlDatabase::addDatabase("QMYSQL", "4b");
    team4b.setHostName("pavelow.eng.uah.edu");
    team4b.setPort(33158);
    team4b.setDatabaseName("team4b");
    team4b.setUserName("team3b");
    team4b.setPassword("ulimbese");

    if(!team4b.open())
    {
        qDebug() << "Database was not opened successfully";
        qDebug() << team4b.lastError();
        message("Database was not opened successfully.\n Closing the application!");
        exit(2);
    }

    fqt_test = QSqlDatabase::addDatabase("QMYSQL", "3b");
    fqt_test.setHostName("pavelow.eng.uah.edu");
    fqt_test.setPort(33157);
    fqt_test.setDatabaseName("fqt_test");
    fqt_test.setUserName("team3b");
    fqt_test.setPassword("ulimbese");

    if(!fqt_test.open())
    {
        qDebug() << "Database was not opened successfully";
        qDebug() << fqt_test.lastError();
        message("Database was not opened successfully.\n Closing the application!");
        exit(2);
    }
}

// this function prompts the user for database information
void MainWindow::sql_information()
{
    QDialog* sql_info = new QDialog;
    QGridLayout* layout = new QGridLayout;

    QLabel* title = new QLabel ("SQL Database Information");
    layout->addWidget(title);

    // create line edit and add it to the layout
    type = new QLineEdit;
    QLabel* type_label = new QLabel("Database Type");
    layout->addWidget(type_label, 1, 0);
    layout->addWidget(type, 1, 1);

    // create line edit and add it to the layout
    host = new QLineEdit;
    QLabel* host_label = new QLabel("Host");
    layout->addWidget(host_label, 2, 0);
    layout->addWidget(host, 2, 1);

    // create line edit and add it to the layout
    port = new QLineEdit;
    QLabel* port_label = new QLabel("Port");
    layout->addWidget(port_label, 3,0);
    layout->addWidget(port, 3,1);

    // create line edit and add it to the layout
    name = new QLineEdit;
    QLabel* name_label = new QLabel("Name");
    layout->addWidget(name_label, 4,0);
    layout->addWidget(name, 4, 1);

    // create line edit and add it to the layout
    username = new QLineEdit;
    QLabel* username_label = new QLabel("Username");
    layout->addWidget(username_label, 5, 0);
    layout->addWidget(username, 5, 1);

    // create line edit and add it to the layout
    password = new QLineEdit;
    QLabel* password_label = new QLabel("Password");
    layout->addWidget(password_label, 6, 0);
    layout->addWidget(password, 6, 1);

    // create QPushButton and add it to the layout
    QPushButton* default_db = new QPushButton;
    default_db->setText("Use Default Database");
    layout->addWidget(default_db, 7, 0);

    // create QPushButton and add it to the layout
    QPushButton* submit = new QPushButton;
    submit->setText("Submit");
    layout->addWidget(submit, 7,1);

    sql_info->setLayout(layout);

   connect(submit, SIGNAL(clicked()), this, SLOT(sql_submit())); // user submits data
   connect(submit, SIGNAL(clicked()), sql_info, SLOT(close()));  // close the dialog when user submits data
   connect(default_db, SIGNAL(clicked()), this, SLOT(sql_default())); // user selects default database information

   sql_info->exec();

}

// this function stores the user's database info and passes it to sql_server
void MainWindow::sql_submit()
{
    db_type = type->text().toUpper();
    db_host = host->text();
    db_port = port->text();
    db_name = name->text();
    db_username = username->text();
    db_password = password->text();

    sql_connect(db_type, db_host, db_port.toInt(), db_name, db_username, db_password); // use the information to connect to the desired database
}

// this function populates the QDialog with default data if the user chooses to use the default database
void MainWindow::sql_default()
{
    type->setText("QMYSQL");
    host->setText("pavelow");
    port->setText(QString::number(33157));
    name->setText("CPE453");
    username->setText("Team_3B");
    password->setText("Team_3B");
}

// this function creates a connection to the desired SQL Database
void MainWindow::sql_connect(QString db_type, QString db_host, int db_port, QString db_name, QString db_username, QString db_password)
{
    QString path_to_database = "C:/Users/Eric/Documents/GitHub/build-CPE_453-Desktop_Qt_5_3_MSVC2013_64bit-Debug"+db_name;
    // create a QSqlDatabase and set login data
    fqt_test = QSqlDatabase::addDatabase(db_type, "4b");
    fqt_test.setHostName(db_host);
    fqt_test.setPort(db_port);
    fqt_test.setDatabaseName(path_to_database);
    fqt_test.setUserName(db_username);
    fqt_test.setPassword(db_password);

    team4b = QSqlDatabase::addDatabase(db_type, "3b");
    team4b.setHostName(db_host);
    team4b.setPort(db_port);
    team4b.setDatabaseName(path_to_database);
    team4b.setUserName(db_username);
    team4b.setPassword(db_password);

    // verify that database was opened successfully
    // if failure occurs, alert the user and exit the program
    if(!fqt_test.open())
    {
        qDebug() << "Database was not opened successfully";
        qDebug() << fqt_test.lastError();
        message("Database was not opened successfully.\n Closing the application!");
        exit(2);
    }

    else
    {
        qDebug() << "Database was opened successfully"; // if database opens successfully
    }

    // verify that database was opened successfully
    // if failure occurs, alert the user and exit the program
    if(!team4b.open())
    {
        qDebug() << "Database was not opened successfully";
        qDebug() << team4b.lastError();
        message("Database was not opened successfully.\n Closing the application!");
        exit(2);
    }

    else
    {
        qDebug() << "Database was opened successfully"; // if database opens successfully
    }

   // db.close(); // close the database connection

}

//This function is where changes are made to the database and the user verifies the changes in the display software
void MainWindow::acceptance_test()
{

    QSqlQuery query("UPDATE Occupancy SET Occupied = 'TRUE' WHERE Segment = '2_7'", fqt_test);

    bool badDB = false;

    // verify that the query is valid and executes properly
    if(!query.exec())
    {
        qDebug() << "Query Error: " << query.lastError();
        //pushError(query.lastError().text()); // add error to error list
        badDB = true; // 'true' indicates failure
    }
    if(!query.isActive())
    {
        qDebug() << "Query Error: " << query.lastError();
        //pushError(query.lastError().text()); // add error to error list
        badDB = true; // 'true' indicates failure
    }
}

void MainWindow::message(QString alert)
{
    QDialog* user_alert = new QDialog;
    QVBoxLayout* layout = new QVBoxLayout;
    //QString Err;
    //int ErrStackSize = ErrorStack.getStackSize();

    QLabel* title = new QLabel("Acceptance Tests");
    title->setAlignment(Qt::AlignHCenter);
    title->setFont(QFont("Helvetica", 16, QFont::Bold));

    QLabel* message = new QLabel(alert);
    message->setAlignment(Qt::AlignHCenter);
    message->setFont(QFont("Helvetica", 12));

    QPushButton* ok = new QPushButton;
    ok->setText("OK");
   connect(ok, SIGNAL(clicked()), user_alert, SLOT(close()));


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
    layout->addWidget(ok);
    //layout->addWidget(errors);
    //layout->addSpacing(25);

  //  layout->addWidget(accept);

    user_alert->setLayout(layout);

    user_alert->exec();
}

 void MainWindow::init_database()
 {

     QString queryPrep = "CREATE TABLE track_ds (id TEXT, status BOOLEAN, ds_from TEXT, ds_to TEXT)";
     QSqlQuery query (queryPrep, fqt_test);
     if(!query.exec())
         qDebug() << "Query Error: " << query.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?, ?, ?, ?)";
     QSqlQuery query2 (queryPrep, fqt_test);
     query2.addBindValue("1_1");
     query2.addBindValue("FALSE");
     query2.addBindValue("NULL");
     query2.addBindValue("NULL");


     if(!query2.exec())
         qDebug() << "Query Error: " << query2.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('1_10', '0', 'NULL', 'NULL')";
     QSqlQuery query3 (queryPrep, fqt_test);
     if(!query3.exec())
         qDebug() << "Query Error: " << query3.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('1_11', '0', 'NULL', 'NULL')";
     QSqlQuery query4 (queryPrep, fqt_test);
     if(!query4.exec())
         qDebug() << "Query Error: " << query4.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('1_12', '0', 'NULL', 'NULL')";
     QSqlQuery query5 (queryPrep, fqt_test);
     if(!query5.exec())
         qDebug() << "Query Error: " << query5.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('1_14', '0', 'NULL', 'NULL')";
     QSqlQuery query6 (queryPrep, fqt_test);
     if(!query6.exec())
         qDebug() << "Query Error: " << query6.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('1_15', '0', 'NULL', 'NULL')";
     QSqlQuery query7 (queryPrep, fqt_test);
     if(!query7.exec())
         qDebug() << "Query Error: " << query7.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('1_16', '0', 'NULL', 'NULL')";
     QSqlQuery query8 (queryPrep, fqt_test);
     if(!query8.exec())
         qDebug() << "Query Error: " << query8.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('1_2', '0', 'NULL', 'NULL')";
     QSqlQuery query9 (queryPrep, fqt_test);
     if(!query9.exec())
         qDebug() << "Query Error: " << query9.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('1_3', '0', 'NULL', 'NULL')";
     QSqlQuery query10 (queryPrep, fqt_test);
     if(!query10.exec())
         qDebug() << "Query Error: " << query10.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('1_4', '0', 'NULL', 'NULL')";
     QSqlQuery query12 (queryPrep, fqt_test);
     if(!query12.exec())
         qDebug() << "Query Error: " << query12.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('1_5', '0', 'NULL', 'NULL')";
     QSqlQuery query13 (queryPrep, fqt_test);
     if(!query13.exec())
         qDebug() << "Query Error: " << query13.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('1_6', '0', 'NULL', 'NULL')";
     QSqlQuery query14 (queryPrep, fqt_test);
     if(!query14.exec())
         qDebug() << "Query Error: " << query14.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('1_7', '0', 'NULL', 'NULL')";
     QSqlQuery query15 (queryPrep, fqt_test);
     if(!query15.exec())
         qDebug() << "Query Error: " << query15.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('1_8', '0', 'NULL', 'NULL')";
     QSqlQuery query16 (queryPrep, fqt_test);
     if(!query16.exec())
         qDebug() << "Query Error: " << query16.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('1_9', '0', 'NULL', 'NULL')";
     QSqlQuery query17 (queryPrep, fqt_test);
     if(!query17.exec())
         qDebug() << "Query Error: " << query17.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('2_1', '1', 'NULL', 'NULL')";
     QSqlQuery query18 (queryPrep, fqt_test);
     if(!query18.exec())
         qDebug() << "Query Error: " << query18.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('2_10', '0', 'NULL', 'NULL')";
     QSqlQuery query19 (queryPrep, fqt_test);
     if(!query19.exec())
         qDebug() << "Query Error: " << query19.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('2_11', '0', 'NULL', 'NULL')";
     QSqlQuery query20 (queryPrep, fqt_test);
     if(!query20.exec())
         qDebug() << "Query Error: " << query20.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('2_12', '0', 'NULL', 'NULL')";
     QSqlQuery query21 (queryPrep, fqt_test);
     if(!query21.exec())
         qDebug() << "Query Error: " << query21.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('2_13', '0', 'NULL', 'NULL')";
     QSqlQuery query22 (queryPrep, fqt_test);
     if(!query22.exec())
         qDebug() << "Query Error: " << query22.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('2_14', '0', 'NULL', 'NULL')";
     QSqlQuery query23 (queryPrep, fqt_test);
     if(!query23.exec())
         qDebug() << "Query Error: " << query23.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('2_15', '1', 'NULL', 'NULL')";
     QSqlQuery query24 (queryPrep, fqt_test);
     if(!query24.exec())
         qDebug() << "Query Error: " << query24.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('2_16', '0', 'NULL', 'NULL')";
     QSqlQuery query25 (queryPrep, fqt_test);
     if(!query25.exec())
         qDebug() << "Query Error: " << query25.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('2_2', '0', 'NULL', 'NULL')";
     QSqlQuery query26 (queryPrep, fqt_test);
     if(!query26.exec())
         qDebug() << "Query Error: " << query26.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('2_3', '0', 'NULL', 'NULL')";
     QSqlQuery query27 (queryPrep, fqt_test);
     if(!query27.exec())
         qDebug() << "Query Error: " << query27.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('2_4', '0', 'NULL', 'NULL')";
     QSqlQuery query28 (queryPrep, fqt_test);
     if(!query28.exec())
         qDebug() << "Query Error: " << query28.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('2_5', '1', 'NULL', 'NULL')";
     QSqlQuery query29 (queryPrep, fqt_test);
     if(!query29.exec())
         qDebug() << "Query Error: " << query29.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('2_6', '0', 'NULL', 'NULL')";
     QSqlQuery query30 (queryPrep, fqt_test);
     if(!query30.exec())
         qDebug() << "Query Error: " << query30.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('2_7', '0', 'NULL', 'NULL')";
     QSqlQuery query31 (queryPrep, fqt_test);
     if(!query31.exec())
         qDebug() << "Query Error: " << query31.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('2_8', '0', 'NULL', 'NULL')";
     QSqlQuery query32 (queryPrep, fqt_test);
     if(!query32.exec())
         qDebug() << "Query Error: " << query32.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('2_9', '0', 'NULL', 'NULL')";
     QSqlQuery query33 (queryPrep, fqt_test);
     if(!query33.exec())
         qDebug() << "Query Error: " << query33.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('3_1', '0', 'NULL', 'NULL')";
     QSqlQuery query34 (queryPrep, fqt_test);
     if(!query34.exec())
         qDebug() << "Query Error: " << query34.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('3_13', '0', 'NULL', 'NULL')";
     QSqlQuery query35 (queryPrep, fqt_test);
     if(!query35.exec())
         qDebug() << "Query Error: " << query35.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('3_14', '0', 'NULL', 'NULL')";
     QSqlQuery query36 (queryPrep, fqt_test);
     if(!query36.exec())
         qDebug() << "Query Error: " << query36.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('3_15', '0', 'NULL', 'NULL')";
     QSqlQuery query37 (queryPrep, fqt_test);
     if(!query37.exec())
         qDebug() << "Query Error: " << query37.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('3_16', '0', 'NULL', 'NULL')";
     QSqlQuery query38 (queryPrep, fqt_test);
     qDebug() << "Query Error: " << query38.lastError();
     if(!query38.exec())
         qDebug() << "Query Error: " << query38.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('3_2', '0', 'NULL', 'NULL')";
     QSqlQuery query39 (queryPrep, fqt_test);
     if(!query39.exec())
         qDebug() << "Query Error: " << query39.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('3_3', '0', 'NULL', 'NULL')";
     QSqlQuery query40 (queryPrep, fqt_test);
     if(!query40.exec())
         qDebug() << "Query Error: " << query40.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('3_4', '0', 'NULL', 'NULL')";
     QSqlQuery query41 (queryPrep, fqt_test);
     if(!query41.exec())
         qDebug() << "Query Error: " << query41.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('3_5', '0', 'NULL', 'NULL')";
     QSqlQuery query42 (queryPrep, fqt_test);
     if(!query42.exec())
         qDebug() << "Query Error: " << query42.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('3_6', '0', 'NULL', 'NULL')";
     QSqlQuery query43 (queryPrep, fqt_test);
     if(!query43.exec())
         qDebug() << "Query Error: " << query43.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('3_7', '0', 'NULL', 'NULL')";
     QSqlQuery query44 (queryPrep, fqt_test);
     if(!query44.exec())
         qDebug() << "Query Error: " << query44.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('3_8', '0', 'NULL', 'NULL')";
     QSqlQuery query45 (queryPrep, fqt_test);
     if(!query45.exec())
         qDebug() << "Query Error: " << query45.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('4_10', '0', 'NULL', 'NULL')";
     QSqlQuery query46 (queryPrep, fqt_test);
     if(!query46.exec())
         qDebug() << "Query Error: " << query46.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('4_11', '0', 'NULL', 'NULL')";
     QSqlQuery query47 (queryPrep, fqt_test);
     if(!query47.exec())
         qDebug() << "Query Error: " << query47.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('4_12', '0', 'NULL', 'NULL')";
     QSqlQuery query48 (queryPrep, fqt_test);
     if(!query48.exec())
         qDebug() << "Query Error: " << query48.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('4_13', '0', 'NULL', 'NULL')";
     QSqlQuery query49 (queryPrep, fqt_test);
     if(!query49.exec())
         qDebug() << "Query Error: " << query49.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('4_14', '0', 'NULL', 'NULL')";
     QSqlQuery query50 (queryPrep, fqt_test);
     if(!query50.exec())
         qDebug() << "Query Error: " << query50.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('4_15', '0', 'NULL', 'NULL')";
     QSqlQuery query51 (queryPrep, fqt_test);
     if(!query51.exec())
         qDebug() << "Query Error: " << query51.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('4_16', '0', 'NULL', 'NULL')";
     QSqlQuery query52 (queryPrep, fqt_test);
     if(!query52.exec())
         qDebug() << "Query Error: " << query52.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('4_2', '0', 'NULL', 'NULL')";
     QSqlQuery query53 (queryPrep, fqt_test);
     if(!query53.exec())
         qDebug() << "Query Error: " << query53.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('4_3', '0', 'NULL', 'NULL')";
     QSqlQuery query54 (queryPrep, fqt_test);
     if(!query54.exec())
         qDebug() << "Query Error: " << query54.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('4_4', '0', 'NULL', 'NULL')";
     QSqlQuery query55 (queryPrep, fqt_test);
     if(!query55.exec())
         qDebug() << "Query Error: " << query55.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('4_9', '0', 'NULL', 'NULL')";
     QSqlQuery query56 (queryPrep, fqt_test);
     if(!query56.exec())
         qDebug() << "Query Error: " << query56.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('5_1', '0', 'NULL', 'NULL')";
     QSqlQuery query57 (queryPrep, fqt_test);
     if(!query57.exec())
         qDebug() << "Query Error: " << query57.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('5_13', '0', 'NULL', 'NULL')";
     QSqlQuery query58 (queryPrep, fqt_test);
     if(!query58.exec())
         qDebug() << "Query Error: " << query58.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('5_14', '0', 'NULL', 'NULL')";
     QSqlQuery query59 (queryPrep, fqt_test);
     if(!query59.exec())
         qDebug() << "Query Error: " << query59.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('5_15', '0', 'NULL', 'NULL')";
     QSqlQuery query60 (queryPrep, fqt_test);
     if(!query60.exec())
         qDebug() << "Query Error: " << query60.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('5_16', '0', 'NULL', 'NULL')";
     QSqlQuery query61 (queryPrep, fqt_test);
     if(!query61.exec())
         qDebug() << "Query Error: " << query61.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('5_2', '0', 'NULL', 'NULL')";
     QSqlQuery query62 (queryPrep, fqt_test);
     if(!query62.exec())
         qDebug() << "Query Error: " << query62.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('5_3', '0', 'NULL', 'NULL')";
     QSqlQuery query63 (queryPrep, fqt_test);
     if(!query63.exec())
         qDebug() << "Query Error: " << query63.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('5_4', '0', 'NULL', 'NULL')";
     QSqlQuery query64 (queryPrep, fqt_test);
     if(!query64.exec())
         qDebug() << "Query Error: " << query64.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('5_5', '0', 'NULL', 'NULL')";
     QSqlQuery query65 (queryPrep, fqt_test);
     if(!query65.exec())
         qDebug() << "Query Error: " << query65.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('5_6', '0', 'NULL', 'NULL')";
     QSqlQuery query66 (queryPrep, fqt_test);
     if(!query66.exec())
         qDebug() << "Query Error: " << query66.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('5_7', '0', 'NULL', 'NULL')";
     QSqlQuery query67 (queryPrep, fqt_test);
     if(!query67.exec())
         qDebug() << "Query Error: " << query67.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('5_8', '0', 'NULL', 'NULL')";
     QSqlQuery query68 (queryPrep, fqt_test);
     if(!query68.exec())
         qDebug() << "Query Error: " << query68.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('6_1', '0', 'NULL', 'NULL')";
     QSqlQuery query69 (queryPrep, fqt_test);
     if(!query69.exec())
         qDebug() << "Query Error: " << query69.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('6_2', '0', 'NULL', 'NULL')";
     QSqlQuery query70 (queryPrep, fqt_test);
     if(!query70.exec())
         qDebug() << "Query Error: " << query70.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('6_3', '0', 'NULL', 'NULL')";
     QSqlQuery query71 (queryPrep, fqt_test);
     if(!query71.exec())
         qDebug() << "Query Error: " << query71.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('6_4', '0', 'NULL', 'NULL')";
     QSqlQuery query72 (queryPrep, fqt_test);
     if(!query72.exec())
         qDebug() << "Query Error: " << query72.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('6_5', '0', 'NULL', 'NULL')";
     QSqlQuery query73 (queryPrep, fqt_test);
     if(!query73.exec())
         qDebug() << "Query Error: " << query73.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('6_6', '0', 'NULL', 'NULL')";
     QSqlQuery query74 (queryPrep, fqt_test);
     if(!query74.exec())
         qDebug() << "Query Error: " << query74.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('6_7', '0', 'NULL', 'NULL')";
     QSqlQuery query75 (queryPrep, fqt_test);
     if(!query75.exec())
         qDebug() << "Query Error: " << query75.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('6_8', '0', 'NULL', 'NULL')";
     QSqlQuery query76 (queryPrep, fqt_test);
     if(!query76.exec())
         qDebug() << "Query Error: " << query76.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('7_1', '0', 'NULL', 'NULL')";
     QSqlQuery query77 (queryPrep, fqt_test);
     if(!query77.exec())
         qDebug() << "Query Error: " << query77.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('7_13', '0', 'NULL', 'NULL')";
     QSqlQuery query78 (queryPrep, fqt_test);
     if(!query78.exec())
         qDebug() << "Query Error: " << query78.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('7_2', '0', 'NULL', 'NULL')";
     QSqlQuery query79 (queryPrep, fqt_test);
     if(!query79.exec())
         qDebug() << "Query Error: " << query79.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('7_3', '0', 'NULL', 'NULL')";
     QSqlQuery query80 (queryPrep, fqt_test);
     if(!query80.exec())
         qDebug() << "Query Error: " << query80.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('7_4', '0', 'NULL', 'NULL')";
     QSqlQuery query81 (queryPrep, fqt_test);
     if(!query81.exec())
         qDebug() << "Query Error: " << query81.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('7_5', '0', 'NULL', 'NULL')";
     QSqlQuery query82 (queryPrep, fqt_test);
     if(!query82.exec())
         qDebug() << "Query Error: " << query82.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('7_6', '0', 'NULL', 'NULL')";
     QSqlQuery query83 (queryPrep, fqt_test);
     if(!query83.exec())
         qDebug() << "Query Error: " << query83.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('7_7', '0', 'NULL', 'NULL')";
     QSqlQuery query84 (queryPrep, fqt_test);
     if(!query84.exec())
         qDebug() << "Query Error: " << query84.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('7_8', '0', 'NULL', 'NULL')";
     QSqlQuery query85 (queryPrep, fqt_test);
     if(!query85.exec())
         qDebug() << "Query Error: " << query85.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('8_1', '0', 'NULL', 'NULL')";
     QSqlQuery query86 (queryPrep, fqt_test);
     if(!query86.exec())
         qDebug() << "Query Error: " << query86.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('8_10', '0', 'NULL', 'NULL')";
     QSqlQuery query87 (queryPrep, fqt_test);
     if(!query87.exec())
         qDebug() << "Query Error: " << query87.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('8_11', '0', 'NULL', 'NULL')";
     QSqlQuery query88 (queryPrep, fqt_test);
     if(!query88.exec())
         qDebug() << "Query Error: " << query88.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('8_12', '0', 'NULL', 'NULL')";
     QSqlQuery query89 (queryPrep, fqt_test);
     if(!query89.exec())
         qDebug() << "Query Error: " << query89.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('8_13', '0', 'NULL', 'NULL')";
     QSqlQuery query90 (queryPrep, fqt_test);
     if(!query90.exec())
         qDebug() << "Query Error: " << query90.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('8_14', '0', 'NULL', 'NULL')";
     QSqlQuery query91 (queryPrep, fqt_test);
     if(!query91.exec())
         qDebug() << "Query Error: " << query91.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('8_15', '0', 'NULL', 'NULL')";
     QSqlQuery query92 (queryPrep, fqt_test);
     if(!query92.exec())
         qDebug() << "Query Error: " << query92.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('8_16', '0', 'NULL', 'NULL')";
     QSqlQuery query93 (queryPrep, fqt_test);
     if(!query93.exec())
         qDebug() << "Query Error: " << query93.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('8_2', '0', 'NULL', 'NULL')";
     QSqlQuery query94 (queryPrep, fqt_test);
     if(!query94.exec())
         qDebug() << "Query Error: " << query94.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('8_3', '0', 'NULL', 'NULL')";
     QSqlQuery query95 (queryPrep, fqt_test);
     if(!query95.exec())
         qDebug() << "Query Error: " << query95.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('8_4', '0', 'NULL', 'NULL')";
     QSqlQuery query96 (queryPrep, fqt_test);
     if(!query96.exec())
         qDebug() << "Query Error: " << query96.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('8_5', '0', 'NULL', 'NULL')";
     QSqlQuery query97 (queryPrep, fqt_test);
     if(!query97.exec())
         qDebug() << "Query Error: " << query97.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('8_6', '0', 'NULL', 'NULL')";
     QSqlQuery query98 (queryPrep, fqt_test);
     if(!query98.exec())
         qDebug() << "Query Error: " << query98.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('8_7', '0', 'NULL', 'NULL')";
     QSqlQuery query99 (queryPrep, fqt_test);
     if(!query99.exec())
         qDebug() << "Query Error: " << query99.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('8_8', '0', 'NULL', 'NULL')";
     QSqlQuery query100 (queryPrep, fqt_test);
     if(!query100.exec())
         qDebug() << "Query Error: " << query100.lastError();

     queryPrep = "INSERT INTO track_ds VALUES('8_9', '0', 'NULL', 'NULL')";
     QSqlQuery query101 (queryPrep, fqt_test);
     if(!query101.exec())
         qDebug() << "Query Error: " << query101.lastError();


     queryPrep = "CREATE TABLE track_power (ds TEXT, state BOOLEAN)";
     QSqlQuery query110 (queryPrep, fqt_test);
     if(!query110.exec())
         qDebug() << "Query Error: " << query110.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?, ?)";
     QSqlQuery query111 (queryPrep, fqt_test);
     query111.addBindValue("1_1");
     query111.addBindValue(1);
     if(!query111.exec())
         qDebug() << "Query Error: " << query111.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('1_10', 1)";
     QSqlQuery query112 (queryPrep, fqt_test);
     if(!query112.exec())
         qDebug() << "Query Error: " << query112.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('1_11', 1)";
     QSqlQuery query113 (queryPrep, fqt_test);
     if(!query113.exec())
         qDebug() << "Query Error: " << query113.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('1_12', 1)";
     QSqlQuery query114 (queryPrep, fqt_test);
     if(!query114.exec())
         qDebug() << "Query Error: " << query114.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('1_14', 1)";
     QSqlQuery query115 (queryPrep, fqt_test);
     if(!query115.exec())
         qDebug() << "Query Error: " << query115.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('1_15', 1)";
     QSqlQuery query116 (queryPrep, fqt_test);
     if(!query116.exec())
         qDebug() << "Query Error: " << query116.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('1_16', 1)";
     QSqlQuery query117 (queryPrep, fqt_test);
     if(!query117.exec())
         qDebug() << "Query Error: " << query117.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('1_2', 1)";
     QSqlQuery query118 (queryPrep, fqt_test);
     if(!query118.exec())
         qDebug() << "Query Error: " << query118.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('1_3', 1)";
     QSqlQuery query119 (queryPrep, fqt_test);
     if(!query119.exec())
         qDebug() << "Query Error: " << query119.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('1_4', 1)";
     QSqlQuery query120 (queryPrep, fqt_test);
     if(!query120.exec())
         qDebug() << "Query Error: " << query120.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('1_5', 1)";
     QSqlQuery query121 (queryPrep, fqt_test);
     if(!query121.exec())
         qDebug() << "Query Error: " << query121.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('1_6', 1)";
     QSqlQuery query122 (queryPrep, fqt_test);
     if(!query122.exec())
         qDebug() << "Query Error: " << query122.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('1_7', 1)";
     QSqlQuery query123 (queryPrep, fqt_test);
     if(!query123.exec())
         qDebug() << "Query Error: " << query123.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('1_8', 1)";
     QSqlQuery query124 (queryPrep, fqt_test);
     if(!query124.exec())
         qDebug() << "Query Error: " << query124.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('1_9', 1)";
     QSqlQuery query125 (queryPrep, fqt_test);
     if(!query125.exec())
         qDebug() << "Query Error: " << query125.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('2_1', 1)";
     QSqlQuery query126 (queryPrep, fqt_test);
     if(!query126.exec())
         qDebug() << "Query Error: " << query126.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('2_10', 1)";
     QSqlQuery query127 (queryPrep, fqt_test);
     if(!query127.exec())
         qDebug() << "Query Error: " << query127.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('2_11', 1)";
     QSqlQuery query128 (queryPrep, fqt_test);
     if(!query128.exec())
         qDebug() << "Query Error: " << query128.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('2_12', 1)";
     QSqlQuery query129 (queryPrep, fqt_test);
     if(!query129.exec())
         qDebug() << "Query Error: " << query129.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('2_13', 1)";
     QSqlQuery query130 (queryPrep, fqt_test);
     if(!query130.exec())
         qDebug() << "Query Error: " << query130.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('2_14', 1)";
     QSqlQuery query131 (queryPrep, fqt_test);
     if(!query131.exec())
         qDebug() << "Query Error: " << query131.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('2_15', 1)";
     QSqlQuery query132 (queryPrep, fqt_test);
     if(!query132.exec())
         qDebug() << "Query Error: " << query132.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('2_16', 1)";
     QSqlQuery query133 (queryPrep, fqt_test);
     if(!query133.exec())
         qDebug() << "Query Error: " << query133.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('2_2', 1)";
     QSqlQuery query134 (queryPrep, fqt_test);
     if(!query134.exec())
         qDebug() << "Query Error: " << query134.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('2_3', 1)";
     QSqlQuery query135 (queryPrep, fqt_test);
     if(!query135.exec())
         qDebug() << "Query Error: " << query135.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('2_4', 1)";
     QSqlQuery query136 (queryPrep, fqt_test);
     if(!query136.exec())
         qDebug() << "Query Error: " << query136.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('2_5', 1)";
     QSqlQuery query137 (queryPrep, fqt_test);
     if(!query137.exec())
         qDebug() << "Query Error: " << query137.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('2_6', 1)";
     QSqlQuery query138 (queryPrep, fqt_test);
     if(!query138.exec())
         qDebug() << "Query Error: " << query138.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('2_7', 1)";
     QSqlQuery query139 (queryPrep, fqt_test);
     if(!query139.exec())
         qDebug() << "Query Error: " << query139.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('2_8', 1)";
     QSqlQuery query140 (queryPrep, fqt_test);
     if(!query140.exec())
         qDebug() << "Query Error: " << query140.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('2_9', 1)";
     QSqlQuery query141 (queryPrep, fqt_test);
     if(!query141.exec())
         qDebug() << "Query Error: " << query141.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('3_1', 1)";
     QSqlQuery query142 (queryPrep, fqt_test);
     if(!query142.exec())
         qDebug() << "Query Error: " << query142.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('3_13', 0)";
     QSqlQuery query143 (queryPrep, fqt_test);
     if(!query143.exec())
         qDebug() << "Query Error: " << query143.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('3_14', 1)";
     QSqlQuery query144 (queryPrep, fqt_test);
     if(!query144.exec())
         qDebug() << "Query Error: " << query144.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('3_15', 1)";
     QSqlQuery query145 (queryPrep, fqt_test);
     if(!query145.exec())
         qDebug() << "Query Error: " << query145.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('3_16', 1)";
     QSqlQuery query146 (queryPrep, fqt_test);
     if(!query146.exec())
         qDebug() << "Query Error: " << query146.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('3_2', 1)";
     QSqlQuery query147 (queryPrep, fqt_test);
     if(!query147.exec())
         qDebug() << "Query Error: " << query147.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('3_3', 1)";
     QSqlQuery query148 (queryPrep, fqt_test);
     if(!query148.exec())
         qDebug() << "Query Error: " << query148.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('3_4', 1)";
     QSqlQuery query149 (queryPrep, fqt_test);
     if(!query149.exec())
         qDebug() << "Query Error: " << query149.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('3_5', 1)";
     QSqlQuery query150 (queryPrep, fqt_test);
     if(!query150.exec())
         qDebug() << "Query Error: " << query150.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('3_6', 1)";
     QSqlQuery query151 (queryPrep, fqt_test);
     if(!query151.exec())
         qDebug() << "Query Error: " << query151.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('3_7', 1)";
     QSqlQuery query152 (queryPrep, fqt_test);
     if(!query152.exec())
         qDebug() << "Query Error: " << query152.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('3_8', 0)";
     QSqlQuery query153 (queryPrep, fqt_test);
     if(!query153.exec())
         qDebug() << "Query Error: " << query153.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('4_10', 1)";
     QSqlQuery query154 (queryPrep, fqt_test);
     if(!query154.exec())
         qDebug() << "Query Error: " << query154.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('4_11', 1)";
     QSqlQuery query155 (queryPrep, fqt_test);
     if(!query155.exec())
         qDebug() << "Query Error: " << query155.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('4_12', 1)";
     QSqlQuery query156 (queryPrep, fqt_test);
     if(!query156.exec())
         qDebug() << "Query Error: " << query156.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('4_13', 1)";
     QSqlQuery query157 (queryPrep, fqt_test);
     if(!query157.exec())
         qDebug() << "Query Error: " << query157.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('4_14', 1)";
     QSqlQuery query158 (queryPrep, fqt_test);
     if(!query158.exec())
         qDebug() << "Query Error: " << query158.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('4_15', 1)";
     QSqlQuery query159 (queryPrep, fqt_test);
     if(!query159.exec())
         qDebug() << "Query Error: " << query159.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('4_16', 1)";
     QSqlQuery query160 (queryPrep, fqt_test);
     if(!query160.exec())
         qDebug() << "Query Error: " << query160.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('4_2', 1)";
     QSqlQuery query161 (queryPrep, fqt_test);
     if(!query161.exec())
         qDebug() << "Query Error: " << query161.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('4_3', 1)";
     QSqlQuery query162 (queryPrep, fqt_test);
     if(!query162.exec())
         qDebug() << "Query Error: " << query162.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('4_4', 1)";
     QSqlQuery query163 (queryPrep, fqt_test);
     if(!query163.exec())
         qDebug() << "Query Error: " << query163.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('4_9', 1)";
     QSqlQuery query164 (queryPrep, fqt_test);
     if(!query164.exec())
         qDebug() << "Query Error: " << query164.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('5_1', 1)";
     QSqlQuery query165 (queryPrep, fqt_test);
     if(!query165.exec())
         qDebug() << "Query Error: " << query165.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('5_13', 1)";
     QSqlQuery query166 (queryPrep, fqt_test);
     if(!query166.exec())
         qDebug() << "Query Error: " << query166.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('5_14', 1)";
     QSqlQuery query167 (queryPrep, fqt_test);
     if(!query167.exec())
         qDebug() << "Query Error: " << query167.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('5_15', 1)";
     QSqlQuery query168 (queryPrep, fqt_test);
     if(!query168.exec())
         qDebug() << "Query Error: " << query168.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('5_16', 1)";
     QSqlQuery query169 (queryPrep, fqt_test);
     if(!query169.exec())
         qDebug() << "Query Error: " << query169.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('5_2', 1)";
     QSqlQuery query170 (queryPrep, fqt_test);
     if(!query170.exec())
         qDebug() << "Query Error: " << query170.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('5_3', 1)";
     QSqlQuery query171 (queryPrep, fqt_test);
     if(!query171.exec())
         qDebug() << "Query Error: " << query171.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('5_4', 0)";
     QSqlQuery query172 (queryPrep, fqt_test);
     if(!query172.exec())
         qDebug() << "Query Error: " << query172.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('5_5', 1)";
     QSqlQuery query173 (queryPrep, fqt_test);
     if(!query173.exec())
         qDebug() << "Query Error: " << query173.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('5_6', 1)";
     QSqlQuery query174 (queryPrep, fqt_test);
     if(!query174.exec())
         qDebug() << "Query Error: " << query174.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('5_7', 1)";
     QSqlQuery query175 (queryPrep, fqt_test);
     if(!query175.exec())
         qDebug() << "Query Error: " << query175.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('5_8', 1)";
     QSqlQuery query176 (queryPrep, fqt_test);
     if(!query176.exec())
         qDebug() << "Query Error: " << query176.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('6_1', 1)";
     QSqlQuery query177 (queryPrep, fqt_test);
     if(!query177.exec())
         qDebug() << "Query Error: " << query177.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('6_2', 1)";
     QSqlQuery query178 (queryPrep, fqt_test);
     if(!query178.exec())
         qDebug() << "Query Error: " << query178.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('6_3', 1)";
     QSqlQuery query179 (queryPrep, fqt_test);
     if(!query179.exec())
         qDebug() << "Query Error: " << query179.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('6_4', 1)";
     QSqlQuery query180 (queryPrep, fqt_test);
     if(!query180.exec())
         qDebug() << "Query Error: " << query180.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('6_5', 1)";
     QSqlQuery query181 (queryPrep, fqt_test);
     if(!query181.exec())
         qDebug() << "Query Error: " << query181.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('6_6', 1)";
     QSqlQuery query182 (queryPrep, fqt_test);
     if(!query182.exec())
         qDebug() << "Query Error: " << query182.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('6_7', 1)";
     QSqlQuery query183 (queryPrep, fqt_test);
     if(!query183.exec())
         qDebug() << "Query Error: " << query183.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('6_8', 1)";
     QSqlQuery query184 (queryPrep, fqt_test);
     if(!query184.exec())
         qDebug() << "Query Error: " << query184.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('7_1', 1)";
     QSqlQuery query185 (queryPrep, fqt_test);
     if(!query185.exec())
         qDebug() << "Query Error: " << query185.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('7_13', 1)";
     QSqlQuery query186 (queryPrep, fqt_test);
     if(!query186.exec())
         qDebug() << "Query Error: " << query186.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('7_2', 1)";
     QSqlQuery query187 (queryPrep, fqt_test);
     if(!query187.exec())
         qDebug() << "Query Error: " << query187.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('7_3', 1)";
     QSqlQuery query188 (queryPrep, fqt_test);
     if(!query188.exec())
         qDebug() << "Query Error: " << query188.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('7_4', 1)";
     QSqlQuery query189 (queryPrep, fqt_test);
     if(!query189.exec())
         qDebug() << "Query Error: " << query189.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('7_5', 1)";
     QSqlQuery query190 (queryPrep, fqt_test);
     if(!query190.exec())
         qDebug() << "Query Error: " << query190.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('7_6', 1)";
     QSqlQuery query191 (queryPrep, fqt_test);
     if(!query191.exec())
         qDebug() << "Query Error: " << query191.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('7_7', 1)";
     QSqlQuery query192 (queryPrep, fqt_test);
     if(!query192.exec())
         qDebug() << "Query Error: " << query192.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('7_8', 1)";
     QSqlQuery query193 (queryPrep, fqt_test);
     if(!query193.exec())
         qDebug() << "Query Error: " << query193.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('8_1', 0)";
     QSqlQuery query194 (queryPrep, fqt_test);
     if(!query194.exec())
         qDebug() << "Query Error: " << query194.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('8_10', 1)";
     QSqlQuery query195 (queryPrep, fqt_test);
     if(!query195.exec())
         qDebug() << "Query Error: " << query195.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('8_11', 1)";
     QSqlQuery query196 (queryPrep, fqt_test);
     if(!query196.exec())
         qDebug() << "Query Error: " << query196.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('8_12', 1)";
     QSqlQuery query197 (queryPrep, fqt_test);
     if(!query197.exec())
         qDebug() << "Query Error: " << query197.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('8_13', 1)";
     QSqlQuery query198 (queryPrep, fqt_test);
     if(!query198.exec())
         qDebug() << "Query Error: " << query198.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('8_14', 1)";
     QSqlQuery query199 (queryPrep, fqt_test);
     if(!query199.exec())
         qDebug() << "Query Error: " << query199.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('8_15', 1)";
     QSqlQuery query200 (queryPrep, fqt_test);
     if(!query200.exec())
         qDebug() << "Query Error: " << query200.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('8_16', 1)";
     QSqlQuery query201 (queryPrep, fqt_test);
     if(!query201.exec())
         qDebug() << "Query Error: " << query201.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('8_2', 1)";
     QSqlQuery query202 (queryPrep, fqt_test);
     if(!query202.exec())
         qDebug() << "Query Error: " << query202.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('8_3', 1)";
     QSqlQuery query203 (queryPrep, fqt_test);
     if(!query203.exec())
         qDebug() << "Query Error: " << query203.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('8_4', 1)";
     QSqlQuery query204 (queryPrep, fqt_test);
     if(!query204.exec())
         qDebug() << "Query Error: " << query204.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('8_5', 1)";
     QSqlQuery query205 (queryPrep, fqt_test);
     if(!query205.exec())
         qDebug() << "Query Error: " << query205.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('8_6', 1)";
     QSqlQuery query206 (queryPrep, fqt_test);
     if(!query206.exec())
         qDebug() << "Query Error: " << query206.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('8_7', 1)";
     QSqlQuery query207 (queryPrep, fqt_test);
     if(!query207.exec())
         qDebug() << "Query Error: " << query207.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('8_8', 1)";
     QSqlQuery query208 (queryPrep, fqt_test);
     if(!query208.exec())
         qDebug() << "Query Error: " << query208.lastError();

     queryPrep = "INSERT INTO track_power VALUES ('8_9', 1)";
     QSqlQuery query209 (queryPrep, fqt_test);
     if(!query209.exec())
         qDebug() << "Query Error: " << query209.lastError();


     queryPrep = "CREATE TABLE track_switch_5a (id INTEGER, vertex INTEGER, position TINYINT)";
     QSqlQuery query210 (queryPrep, fqt_test);
     if(!query210.exec())
         qDebug() << "Query Error: " << query210.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (11, 14, 0)";
         QSqlQuery query211 (queryPrep, fqt_test);
     if(!query211.exec())
         qDebug() << "Query Error: " << query211.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (12, 45, 1)";
         QSqlQuery query212 (queryPrep, fqt_test);
     if(!query212.exec())
         qDebug() << "Query Error: " << query212.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (13, 25, 1)";
         QSqlQuery query213 (queryPrep, fqt_test);
     if(!query213.exec())
         qDebug() << "Query Error: " << query213.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (14, 14, 1)";
         QSqlQuery query214 (queryPrep, fqt_test);
     if(!query214.exec())
         qDebug() << "Query Error: " << query214.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (42, 42, 1)";
         QSqlQuery query215 (queryPrep, fqt_test);
     if(!query215.exec())
         qDebug() << "Query Error: " << query215.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (43, 43, 1)";
         QSqlQuery query216 (queryPrep, fqt_test);
     if(!query216.exec())
         qDebug() << "Query Error: " << query216.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (21, 21, 1)";
         QSqlQuery query217 (queryPrep, fqt_test);
     if(!query217.exec())
         qDebug() << "Query Error: " << query217.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (22, 22, 1)";
         QSqlQuery query218 (queryPrep, fqt_test);
     if(!query218.exec())
         qDebug() << "Query Error: " << query218.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (23, 23, 1)";
         QSqlQuery query219 (queryPrep, fqt_test);
     if(!query219.exec())
         qDebug() << "Query Error: " << query219.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (24, 24, 1)";
         QSqlQuery query220 (queryPrep, fqt_test);
     if(!query220.exec())
         qDebug() << "Query Error: " << query220.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (31, 31, 1)";
         QSqlQuery query221 (queryPrep, fqt_test);
     if(!query221.exec())
         qDebug() << "Query Error: " << query221.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (32, 32, 1)";
         QSqlQuery query222 (queryPrep, fqt_test);
     if(!query222.exec())
         qDebug() << "Query Error: " << query222.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (33, 33, 1)";
         QSqlQuery query223 (queryPrep, fqt_test);
     if(!query223.exec())
         qDebug() << "Query Error: " << query223.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (34, 34, 1)";
         QSqlQuery query224 (queryPrep, fqt_test);
     if(!query224.exec())
         qDebug() << "Query Error: " << query224.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (41, 41, 1)";
         QSqlQuery query225 (queryPrep, fqt_test);
     if(!query225.exec())
         qDebug() << "Query Error: " << query225.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (61, 61, 1)";
         QSqlQuery query226 (queryPrep, fqt_test);
     if(!query226.exec())
         qDebug() << "Query Error: " << query226.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (62, 62, 1)";
         QSqlQuery query227 (queryPrep, fqt_test);
     if(!query227.exec())
         qDebug() << "Query Error: " << query227.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (63, 63, 1)";
         QSqlQuery query228 (queryPrep, fqt_test);
     if(!query228.exec())
         qDebug() << "Query Error: " << query228.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (64, 64, 1)";
         QSqlQuery query229 (queryPrep, fqt_test);
     if(!query229.exec())
         qDebug() << "Query Error: " << query229.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (72, 72, 1)";
         QSqlQuery query230 (queryPrep, fqt_test);
     if(!query230.exec())
         qDebug() << "Query Error: " << query230.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (73, 73, 1)";
         QSqlQuery query231 (queryPrep, fqt_test);
     if(!query231.exec())
         qDebug() << "Query Error: " << query231.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (74, 74, 1)";
         QSqlQuery query232 (queryPrep, fqt_test);
     if(!query232.exec())
         qDebug() << "Query Error: " << query232.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (84, 84, 1)";
         QSqlQuery query233 (queryPrep, fqt_test);
     if(!query233.exec())
         qDebug() << "Query Error: " << query233.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (82, 82, 1)";
         QSqlQuery query234 (queryPrep, fqt_test);
     if(!query234.exec())
         qDebug() << "Query Error: " << query234.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (83, 83, 1)";
         QSqlQuery query235 (queryPrep, fqt_test);
     if(!query235.exec())
         qDebug() << "Query Error: " << query235.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (51, 51, 1)";
         QSqlQuery query236 (queryPrep, fqt_test);
     if(!query236.exec())
         qDebug() << "Query Error: " << query236.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (52, 52, 1)";
         QSqlQuery query237 (queryPrep, fqt_test);
     if(!query237.exec())
         qDebug() << "Query Error: " << query237.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (71, 71, 1)";
         QSqlQuery query238 (queryPrep, fqt_test);
     if(!query238.exec())
         qDebug() << "Query Error: " << query238.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (81, 81, 1)";
         QSqlQuery query239 (queryPrep, fqt_test);
     if(!query239.exec())
         qDebug() << "Query Error: " << query239.lastError();



     queryPrep = "CREATE TABLE scheduled_train_info (id TEXT, current TEXT, destination TEXT, next TEXT, path INTEGER)";
     QSqlQuery query102 (queryPrep, fqt_test);
     if(!query102.exec())
         qDebug() << "Query Error: " << query102.lastError();

     queryPrep = "INSERT INTO scheduled_train_info VALUES ('1', '2_5', '2_9', '2_6', 1)";
     QSqlQuery query103 (queryPrep, fqt_test);
     if(!query103.exec())
         qDebug() << "Query Error: " << query103.lastError();

     queryPrep = "INSERT INTO scheduled_train_info VALUES ('2', '3_1', '3_14', '3_13', 2)";
     QSqlQuery query104 (queryPrep, fqt_test);
     if(!query104.exec())
         qDebug() << "Query Error: " << query104.lastError();

     queryPrep = "INSERT INTO scheduled_train_info VALUES ('3', '4_11', '4_14', '4_12', 3)";
     QSqlQuery query105 (queryPrep, fqt_test);
     if(!query105.exec())
         qDebug() << "Query Error: " << query105.lastError();

     queryPrep = "CREATE TABLE scheduled_routes (pathid INTEGER, nextpath INTEGER, next2 TEXT,next3 TEXT,next4 TEXT,next5 TEXT,next6 TEXT,next7 TEXT,next8 TEXT,next9 TEXT,next10 TEXT,next11 TEXT)";
     QSqlQuery query106 (queryPrep, fqt_test);
     if(!query106.exec())
         qDebug() << "Query Error: " << query106.lastError();

     queryPrep = "INSERT INTO scheduled_routes VALUES (1, NULL, 4_14, 1_16, 1_15, 1_14, 1_8, NULL, NULL, NULL, NULL, NULL)";
     QSqlQuery query107 (queryPrep, fqt_test);
     if(!query107.exec())
         qDebug() << "Query Error: " << query107.lastError();

     queryPrep = "INSERT INTO scheduled_routes VALUES (2, NULL, 2_1, 2_3, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)";
     QSqlQuery query108 (queryPrep, fqt_test);
     if(!query108.exec())
         qDebug() << "Query Error: " << query108.lastError();

     queryPrep = "INSERT INTO scheduled_routes VALUES (3, NULL, 2_15, 2_14, 8_4, 8_3, 8_2, NULL, NULL, NULL, NULL, NULL)";
     QSqlQuery query109 (queryPrep, fqt_test);
     if(!query109.exec())
         qDebug() << "Query Error: " << query109.lastError();


 }

 void MainWindow::next_test()
 {
    // Test #1 both
    QString query = "UPDATE track_power SET state = '0' WHERE ds = '1_1'";
    message("Change the status of track section with ID = '1_1' from “Active” to “Inactive”");
    QSqlQuery query2(query, fqt_test);
    query2.exec();
    message("Verify that track with ID = '1_1' is now red");

    //Test #2 both
    query = "UPDATE track_ds SET status = '1' WHERE id = '6_5'";
    message("Change the status of track section with ID = '6_5' from “Active” to “Occupied”");
    QSqlQuery query3(query, fqt_test);
    query3.exec();
    message("Verify that track with ID = '6_5' is now blue");

    // Test #3 both
    query = "UPDATE track_ds SET status = '0' WHERE id = '6_5'";
    message("Change the status of track section with ID = '6_5' from “Occupied” to “Active”");
    QSqlQuery query4(query, fqt_test);
    query4.exec();
    message("Verify that track with ID = '6_5' is now green");

    //Test #4 both
    query = "UPDATE track_ds SET status = '0' WHERE id = '2_15'";
    message("Change the status of track section with ID = '2_15' from “Occupied” to “Inactive”");
    QSqlQuery query5(query, fqt_test);
    query5.exec();
    query = "UPDATE track_power SET state = '0' WHERE ds = '2_15'";
    QSqlQuery query6(query, fqt_test);
    query6.exec();
    message("Verify that track with ID = '2_15' is now red");

    // Test #5 both
    query = "UPDATE track_power SET state = '1' WHERE ds = '3_13'";
    message("Change the status of track section with ID = '3_13' from “Inactive” to “Active”");
    QSqlQuery query7(query, fqt_test);
    query7.exec();
    message("Verify that track with ID = '3_13' is now green");

    // Test #6 both
    query = "UPDATE track_ds SET status = '1' WHERE id = '2_15'";
    message("Change the status of track section with ID = '2_15' from “Inactive” to “Occupied”");
    QSqlQuery query8(query, fqt_test);
    query8.exec();
    message("Verify that track with ID = '2_15' is now blue");

    //Test #6 both
    query = "UPDATE scheduled_train_info SET current = '1_16' WHERE path = 1";
    message("Change the location of Locomotive #1 to the track segment with ID = '1_16'");
    QSqlQuery query9(query, fqt_test);
    query = "UPDATE track_ds SET status = '1' WHERE id = '1_16'";
    query9.exec();
    QSqlQuery query10(query, fqt_test);\
    query10.exec();
    message("Verify that track with ID = '1_16' is blue and Locomotive #1 is now located on track segment with ID = '1_16'");


    //Test#7 both
    query = "UPDATE scheduled_train_info SET current = '2_3' WHERE path = 2";
    message("Change the location of Locomotive #2 to the track segment with ID = '2_3'");
    QSqlQuery query11(query, fqt_test);
    query = "UPDATE track_ds SET status = '1' WHERE id = '2_3'";
    query11.exec();
    QSqlQuery query12(query, fqt_test);\
    query12.exec();
    message("Verify that track with ID = '2_3' is blue and Locomotive #2 is now located on track segment with ID = '2_3'");

    // Test #8 both
    query = "UPDATE scheduled_train_info SET current = '2_14' WHERE path = 3";
    message("Change the location of Locomotive #3 to the track segment with ID = '2_14'");
    QSqlQuery query13(query, fqt_test);
    query = "UPDATE track_ds SET status = '1' WHERE id = '2_14'";
    query13.exec();
    QSqlQuery query14(query, fqt_test);\
    query14.exec();
    message("Verify that track with ID = '2_14' is blue and Locomotive #3 is now located on track segment with ID = '2_14'");

    // Test #9 both
    query = "UPDATE track_switch_5a SET position = '1' WHERE id = 11";
    message("Change the position of the switch with ID = '11' from 'Pass' to 'Bypass'");
    QSqlQuery query16(query, fqt_test);
    query16.exec();
    message("Verify that switch with ID = '11' is now black");

    // Test #10 database, code cant be verified
    query = "UPDATE track_switch_5a SET position = '0' WHERE id = 12";
    message("Change the position of the switch with ID = '12' from 'Bypass' to 'Pass'");
    QSqlQuery query15(query, fqt_test);
    query15.exec();
    message("Verify that switch with ID = '12' is now purple");


 }


