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

    // START COMBOBOX INITIALIZATION

    // Populate Combobox
    ui->showActors_comboBox->addItems(addActorsComboBoxLabels);

    // END COMBOBOX INITIALIZATION

}

MainWindow::~MainWindow()
{
    delete ui;
}

// Navigates user to combat edit page from welcome page
void MainWindow::on_welcomeStart_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(EDIT);

    // Ensure combobox displays proper index
    ui->showActors_comboBox->setCurrentIndex(0);

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

     tableManager->InitializeInitiativeModel(ui->assignInit_tableWidget);

     // Inserts initiative column to ensure table copies to assign init page correctly
     ui->combatTable_tableWidget->insertColumn(tableManager->I_INIT);

    // Copy combat table to assignInit table
    tableManager->CopyTable(ui->combatTable_tableWidget, ui->assignInit_tableWidget, false);
    tableManager->InsertInitCol(ui->assignInit_tableWidget);

    // Removes actor type column
    ui->assignInit_tableWidget->removeColumn(tableManager->AssignInitColCount);
}

// Navigates user to combat edit page from assign initiative page
void MainWindow::on_back_assignInit_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(EDIT);
    tableManager->CopyTable(ui->assignInit_tableWidget, ui->combatTable_tableWidget, false);

    // Removes initiative column to ensure table copies to combat editor page correctly
    ui->combatTable_tableWidget->removeColumn(tableManager->I_INIT);
    
    // Ensure combobox displays proper index
    ui->showActors_comboBox->setCurrentIndex(0);
}

// Navigates user to combat page from assign initiative page
void MainWindow::on_fight_assignInit_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(COMBAT);

    tableManager->CopyTable(ui->assignInit_tableWidget, ui->activeCombatTable_tableWidget, true);
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
    ui->dbEdit_tableView->setColumnHidden(tableManager->D_ID, true);
    ui->dbEdit_tableView->setColumnWidth(tableManager->D_NOTES, 400);
    ui->dbEdit_tableView->setColumnWidth(tableManager->D_NAME, 200);
}

// 'Adds' actor to combat list by moving actor listing from actor list to combat list
void MainWindow::on_addActor_pushButton_clicked()
{
    // Move selected actor from "Add Actor" table to "Combat" Table
    tableManager->MoveActorToTable(ui->actorTable_tableWidget, ui->combatTable_tableWidget);
}

// 'Removes' actor from combat list by moving actor listing from combat list to actor list
void MainWindow::on_deleteActor_pushButton_clicked()
{
    // Move selected actor from "Add Actor" table to "Combat" Table
    tableManager->MoveActorToTable(ui->combatTable_tableWidget, ui->actorTable_tableWidget);
}

// Filters displayed actors by type
void MainWindow::on_showActors_comboBox_activated(int index)
{
    switch(index)
    {
        case ALL: for(int index = 0; index < ui->actorTable_tableWidget->rowCount(); index++) { ui->actorTable_tableWidget->showRow(index); }
            break;
        case PARTY: tableManager->ShowActorType(ui->actorTable_tableWidget, "partymember");
            break;
        case CREATURES: tableManager->ShowActorType(ui->actorTable_tableWidget, "creature");
            break;
        case COMPANIONS: tableManager->ShowActorType(ui->actorTable_tableWidget, "companion");
            break;
        case EFFECTS: tableManager->ShowActorType(ui->actorTable_tableWidget, "effect");
            break;
        default: break;
    }
}

//  Focuses on initiative spinbox whenever user changes selection on table
void MainWindow::on_assignInit_tableWidget_itemSelectionChanged()
{
    int currentRow;
    QSpinBox* initBox;

    currentRow = ui->assignInit_tableWidget->currentRow();
    initBox = qobject_cast<QSpinBox*>(ui->assignInit_tableWidget->cellWidget(currentRow, tableManager->I_INIT));

    initBox->setFocus();
    initBox->selectAll();
}
