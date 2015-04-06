#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sql_pavelow(); // allow the user to create a custom connection or connect to pavelow
    connect(ui->btnTest,SIGNAL(clicked()),this,SLOT(acceptance_test()));
}

MainWindow::~MainWindow()
{
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
        //user_alert->message("Database was not opened successfully.\n Closing the application!");
        exit(2);
    }

    team3b = QSqlDatabase::addDatabase("QMYSQL", "3b");
    team3b.setHostName("pavelow.eng.uah.edu");
    team3b.setPort(33157);
    team3b.setDatabaseName("team3b");
    team3b.setUserName("team3b");
    team3b.setPassword("ulimbese");

    if(!team3b.open())
    {
        qDebug() << "Database was not opened successfully";
        qDebug() << team3b.lastError();
        //user_alert->message("Database was not opened successfully.\n Closing the application!");
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
    team3b = QSqlDatabase::addDatabase(db_type, "4b");
    team3b.setHostName(db_host);
    team3b.setPort(db_port);
    team3b.setDatabaseName(path_to_database);
    team3b.setUserName(db_username);
    team3b.setPassword(db_password);

    team4b = QSqlDatabase::addDatabase(db_type, "3b");
    team4b.setHostName(db_host);
    team4b.setPort(db_port);
    team4b.setDatabaseName(path_to_database);
    team4b.setUserName(db_username);
    team4b.setPassword(db_password);

    // verify that database was opened successfully
    // if failure occurs, alert the user and exit the program
    if(!team3b.open())
    {
        qDebug() << "Database was not opened successfully";
        qDebug() << team3b.lastError();
        //user_alert->message("Database was not opened successfully.\n Closing the application!");
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
        //user_alert->message("Database was not opened successfully.\n Closing the application!");
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

    QSqlQuery query("UPDATE Occupancy SET Occupied = 'TRUE' WHERE Segment = '2_7'", team3b);

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
