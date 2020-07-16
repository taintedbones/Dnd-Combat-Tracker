#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->main_stackedWidget->setCurrentIndex(MENU);

    db = new Database("../itdb.db", "QSQLITE");

    format_dbEdit_tableView();

    tableManager = new TableModel;

    // Create tablewidgets
    combatTable = new QTableWidget;
    assignInit = new QTableWidget;
    combatManager = new CombatManager(ui->activeCombatTable_tableWidget);

    // Pull actor list from database
    db->CreateActorList();

    // Populate list of partymembers
    db->CreatePartyList();

    // START COMBOBOX INITIALIZATION

    // Populate Combobox
    ui->showActors_comboBox->addItems(addActorsComboBoxLabels);

    // END COMBOBOX INITIALIZATION
    addActorForm = new AddActorForm(nullptr, db, ui->activeCombatTable_tableWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// *************************************************************************************
// Navigates user to combat edit page from welcome page
// *************************************************************************************
void MainWindow::on_welcomeStart_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(EDIT);

    // Ensure combobox displays proper index
    ui->showActors_comboBox->setCurrentIndex(0);

    // Populate "Actors" TableWidget
        // Initialize/Clear TableWidget
    tableManager->InitializeAddActorTable(ui->actorTable_tableWidget, tableManager->ActorListColCount, tableManager->ActorListColNames);

        // Get list of all actors from db, store in vector of type actor
    tableManager->PopulateAddActorTable(ui->actorTable_tableWidget, db->GetActorList());

    // Populate "Combat" TableWidget
        // Initialize "combatList" TableWidget
    tableManager->InitializeAddActorTable(ui->combatTable_tableWidget, tableManager->SelectedListColCount, tableManager->SelectedListColNames);

        // Populate prepCombatTable tableWidget
    tableManager->PopulateAddActorTable(ui->combatTable_tableWidget, db->GetPartyList());

    tableManager->InsertSpinBoxCol(ui->combatTable_tableWidget, 1, 1, tableManager->S_QTY);
}

// *************************************************************************************
// Navigates user to welcome page from combat edit page
// *************************************************************************************
void MainWindow::on_back_editPage_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(WELCOME);
}

// *************************************************************************************
// Navigates user to assign initiative page from combat edit page. Also copies selected
// actors to set initiative table
// *************************************************************************************
void MainWindow::on_next_editPage_pushButton_clicked()
{
    bool emptyCombat = ui->combatTable_tableWidget->rowCount() < 1;

    if(emptyCombat)
    {
        QMessageBox::critical(this, "Empty Combat", "Please add an actor to combat before proceeding");
    }
    else
    {
        ui->main_stackedWidget->setCurrentIndex(ASSIGN);

         tableManager->InitializeInitiativeModel(ui->assignInit_tableWidget);

         // Inserts initiative column to ensure table copies to assign init page correctly
//         ui->combatTable_tableWidget->insertColumn(tableManager->I_INIT);

        // Copy combat table to assignInit table
        tableManager->CopyTableToInitPage(ui->combatTable_tableWidget, ui->assignInit_tableWidget);
        tableManager->InsertSpinBoxCol(ui->assignInit_tableWidget, 1, 30, tableManager->I_INIT);

        // Removes actor type column
//        ui->assignInit_tableWidget->removeColumn(tableManager->AssignInitColCount);
    }
}

// *************************************************************************************
// Navigates user to combat edit page from assign initiative page
// *************************************************************************************
void MainWindow::on_back_assignInit_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(EDIT);

    // Removes initiative column to ensure table copies to combat editor page correctly
//    ui->combatTable_tableWidget->removeColumn(tableManager->I_INIT);

    // Ensure combobox displays proper index
    ui->showActors_comboBox->setCurrentIndex(0);
}

// *************************************************************************************
// Navigates user to combat page from assign initiative page
// *************************************************************************************
void MainWindow::on_fight_assignInit_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(COMBAT);

    tableManager->InitializeCombatModel(ui->activeCombatTable_tableWidget);
    tableManager->CopyTableToCombatPage(ui->assignInit_tableWidget, ui->activeCombatTable_tableWidget);
    tableManager->SetupCombatStatsCol(ui->activeCombatTable_tableWidget, 20, tableManager->C_HP);
    tableManager->SetupCombatStatsCol(ui->activeCombatTable_tableWidget, 0, tableManager->C_AC);

    ui->activeCombatTable_tableWidget->removeColumn(6);
    ui->activeCombatTable_tableWidget->sortItems(tableManager->C_INIT, Qt::DescendingOrder);

    combatManager->InsertRoundDivider();
    ui->activeCombatTable_tableWidget->selectRow(0);

    ui->roundVal_label->setText(QString::number(combatManager->GetRound()));
    ui->playerName_label->setText(ui->activeCombatTable_tableWidget->item(0, 0)->text());
}

// *************************************************************************************
// Navigates user to welcome page from combat page
// *************************************************************************************
void MainWindow::on_endCombat_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(WELCOME);

    ui->activeCombatTable_tableWidget->clear();

    // This needs to call CreateActorList from db class because the
    // existing one has been altered.

    // And CreatePartyList probably since those have been mixed up
}

// *************************************************************************************
// Navigates user to database editor page from welcome page
// *************************************************************************************
void MainWindow::on_dbOpt_welcome_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(DB_EDIT);
}

// *************************************************************************************
// Navigates user to database editor page from combat edit page
// *************************************************************************************
void MainWindow::on_dbOpt_editPage_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(DB_EDIT);
}

// *************************************************************************************
// Navigates user to welcome page from database editor page
// *************************************************************************************
void MainWindow::on_mainMenu_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(WELCOME);
}

// *************************************************************************************
// Navigates user to combat edit page from database editor page
// *************************************************************************************
void MainWindow::on_combatEditor_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(EDIT);
}

// *************************************************************************************
// Formats db edit table view
// *************************************************************************************
void MainWindow::format_dbEdit_tableView()
{
    model = new DbEditTableModel(this, db);

    ui->dbEdit_tableView->setModel(model);
    ui->dbEdit_tableView->setColumnHidden(tableManager->D_ID, true);
    ui->dbEdit_tableView->setColumnWidth(tableManager->D_NOTES, 400);
    ui->dbEdit_tableView->setColumnWidth(tableManager->D_NAME, 200);
}

// *************************************************************************************
// 'Adds' actor to combat list by moving actor listing from actor list to combat list
// *************************************************************************************
void MainWindow::on_addActor_pushButton_clicked()
{
    // Move selected actor from "Add Actor" table to "Combat" Table
    tableManager->AddActorToTable(ui->actorTable_tableWidget, ui->combatTable_tableWidget);
}

// *************************************************************************************
// 'Removes' actor from combat list by moving actor listing from combat list to actor list
// *************************************************************************************
void MainWindow::on_deleteActor_pushButton_clicked()
{
    // Move selected actor from "Add Actor" table to "Combat" Table
    tableManager->RemoveActorFromTable(ui->combatTable_tableWidget, ui->actorTable_tableWidget);
}

// *************************************************************************************
// Filters displayed actors by type
// *************************************************************************************
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

// *************************************************************************************
//  Focuses on initiative spinbox whenever user changes selection on table
// *************************************************************************************
void MainWindow::on_assignInit_tableWidget_itemSelectionChanged()
{
    int currentRow;
    QSpinBox* initBox;

    currentRow = ui->assignInit_tableWidget->currentRow();
    initBox = qobject_cast<QSpinBox*>(ui->assignInit_tableWidget->cellWidget(currentRow, tableManager->I_INIT));

    initBox->setFocus();
    initBox->selectAll();
}

// *************************************************************************************
// Opens & initializes add actor form whenever add actor button is clicked in combat page
// *************************************************************************************
void MainWindow::on_addActor_combat_pushButton_clicked()
{
    addActorForm->show();
    addActorForm->Initialize();
}

// *************************************************************************************
//
// *************************************************************************************
void MainWindow::on_activeCombatTable_tableWidget_itemSelectionChanged()
{
    int currentRow;
    QString actorNotes;
    QString name;

    currentRow = ui->activeCombatTable_tableWidget->currentRow();

    ui->deleteActor_combat_pushButton->setDisabled(combatManager->IsDivider());

    if(!combatManager->IsDivider())
    {
        actorNotes = ui->activeCombatTable_tableWidget->item(currentRow, tableManager->C_NOTES)->text();
        name = ui->activeCombatTable_tableWidget->item(currentRow, tableManager->C_NAME)->text();

        ui->notes_textEdit->setText(actorNotes);
        ui->notesName_label->setText(name);
    }
}

// *************************************************************************************
//
// *************************************************************************************
void MainWindow::on_endTurn_pushButton_clicked()
{
    combatManager->NextTurn();
    ui->roundVal_label->setText(QString::number(combatManager->GetRound()));
    ui->playerName_label->setText(ui->activeCombatTable_tableWidget->item(0, 0)->text());
}

// *************************************************************************************
//  Deletes the selected actor from the active combat table
// *************************************************************************************
void MainWindow::on_deleteActor_combat_pushButton_clicked()
{
    combatManager->DeleteActor();
}
