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

    db->CreateActorList();
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
    tableManager->InitializeActorListModel(ui->actorTable_tableWidget);

    // Get list of all actors from db, store in vector of type actor
    tableManager->PopulateActorListings(ui->actorTable_tableWidget, db->GetActorList());

//    // START --- void PopulateActorTable(QTableWidget *actorTable_tableWidget)

//    // Prep hp list
//    QVector<QTableWidgetItem*> hpItemList;
//    QTableWidgetItem* hpItem;

//    // Prep ac list
//    QVector<QTableWidgetItem*> acItemList;
//    QTableWidgetItem* acItem;

//    // Prep dc list
//    QVector<QTableWidgetItem*> dcItemList;
//    QTableWidgetItem* dcItem;


//    for(int index = 0; index < db->GetActorList()->length(); index++)
//    {
//        // Create new row
//        ui->actorTable_tableWidget->insertRow(index);
//        // Insert attributes
//            // Name
//        ui->actorTable_tableWidget->setItem(index, tableManager->A_NAME, new QTableWidgetItem(db->GetActorList()->at(index).GetName()));
//            // HP
//        hpItem = new QTableWidgetItem;
//        hpItem->setData(0,db->GetActorList()->at(index).GetHitPoints());
//        hpItemList.push_back(hpItem);
//        ui->actorTable_tableWidget->setItem(index, tableManager->A_HP, hpItemList.at(index));
//            // AC
//        acItem = new QTableWidgetItem;
//        acItem->setData(0,db->GetActorList()->at(index).GetArmorClass());
//        acItemList.push_back(acItem);
//        ui->actorTable_tableWidget->setItem(index, tableManager->A_AC, acItemList.at(index));
//            // DC
//        dcItem = new QTableWidgetItem;
//        dcItem->setData(0,db->GetActorList()->at(index).GetSpellSaveDC());
//        dcItemList.push_back(dcItem);
//        ui->actorTable_tableWidget->setItem(index, tableManager->A_DC, dcItemList.at(index));

//            // Notes
//        ui->actorTable_tableWidget->setItem(index, tableManager->A_NOTES, new QTableWidgetItem(db->GetActorList()->at(index).GetNotes()));
//    }
//    // END --- void PopulateActorTable(QTableWidget *actorTable_tableWidget); ---

    // Initialize "combatList" TableWidget
    tableManager->InitializeCombatModel(ui->combatTable_tableWidget);
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
