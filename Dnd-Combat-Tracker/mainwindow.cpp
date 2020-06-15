#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->main_stackedWidget->setCurrentIndex(WELCOME);

    db = new Database("../itdb.db", "QSQLITE");

    format_dbEdit_tableView();

    tableManager = new TableModel;

    // Create tablewidgets
    combatTable = new QTableWidget;
    assignInit = new QTableWidget;

    // Pull actor list from database
    db->CreateActorList();

    // Populate list of partymembers
    db->CreatePartyList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Navigates user to combat edit page from welcome page
void MainWindow::on_welcomeStart_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(EDIT);

    // Populate "Actors" TableWidget
        // Initialize/Clear TableWidget
    tableManager->InitializeAddActorTable(ui->actorTable_tableWidget);

        // Get list of all actors from db, store in vector of type actor
    tableManager->PopulateAddActorTable(ui->actorTable_tableWidget, db->GetActorList());

    // Populate "Combat" TableWidget
        // Initialize "combatList" TableWidget
    tableManager->InitializeAddActorTable(ui->combatTable_tableWidget);

        // Populate prepCombatTable tableWidget
    tableManager->PopulateAddActorTable(ui->combatTable_tableWidget, db->GetPartyList());
}

// Navigates user to welcome page from combat edit page
void MainWindow::on_back_editPage_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(WELCOME);
}

// Navigates user to assign initiative page from combat edit page
void MainWindow::on_next_editPage_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(ASSIGN);
}

// Navigates user to combat edit page from assign initiative page
void MainWindow::on_back_assignInit_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(EDIT);
}

// Navigates user to combat page from assign initiative page
void MainWindow::on_fight_assignInit_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(COMBAT);
}

// Navigates user to welcome page from combat page
void MainWindow::on_endCombat_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(WELCOME);

    // This needs to call CreateActorList from db class because the
    // existing one has been altered.

    // And CreatePartyList probably since those have been mixed up
}

// Navigates user to database editor page from welcome page
void MainWindow::on_dbOpt_welcome_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(DB_EDIT);
}

// Navigates user to database editor page from combat edit page
void MainWindow::on_dbOpt_editPage_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(DB_EDIT);
}

// Navigates user to welcome page from database editor page
void MainWindow::on_mainMenu_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(WELCOME);
}

// Navigates user to combat edit page from database editor page
void MainWindow::on_combatEditor_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(EDIT);
}

// Formats db edit table view
void MainWindow::format_dbEdit_tableView()
{
    model = new DbEditTableModel(this, db);

    ui->dbEdit_tableView->setModel(model);
    ui->dbEdit_tableView->setColumnHidden(0, true);
    ui->dbEdit_tableView->setColumnWidth(5, 400);
    ui->dbEdit_tableView->setColumnWidth(1, 200);
}
