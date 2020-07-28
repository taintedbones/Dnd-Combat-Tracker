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

    FormatEditActorsTableView();

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
        // Initialize/Clear & populate TableWidget
    tableManager->InitializeAddActorTable(ui->actorTable_tableWidget, tableManager->ActorListColCount, tableManager->ActorListColNames);
    tableManager->PopulateAddActorTable(ui->actorTable_tableWidget, db->GetActorList());

    // Populate "Combat" TableWidget
        // Initialize & populate "combatList" TableWidget
    tableManager->InitializeAddActorTable(ui->combatTable_tableWidget, tableManager->SelectedListColCount, tableManager->SelectedListColNames);
    tableManager->PopulateAddActorTable(ui->combatTable_tableWidget, db->GetPartyList());
    tableManager->InsertSpinBoxCol(ui->combatTable_tableWidget, 1, 1, tableManager->S_QTY);
}

// *************************************************************************************
// Navigates user to welcome page from combat edit page
// *************************************************************************************
void MainWindow::on_back_editPage_pushButton_clicked()
{
    QMessageBox warnPrompt;

    warnPrompt.setIcon(QMessageBox::Warning);
    warnPrompt.setText("WARNING");
    warnPrompt.setInformativeText("Any changes will be lost. Do you want to continue?");
    warnPrompt.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    if(warnPrompt.exec() == QMessageBox::Ok)
    {
        ui->main_stackedWidget->setCurrentIndex(WELCOME);
    }
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

        // Copy combat table to assignInit table
        tableManager->CopyTableToInitPage(ui->combatTable_tableWidget, ui->assignInit_tableWidget);
        tableManager->InsertSpinBoxCol(ui->assignInit_tableWidget, 1, 30, tableManager->I_INIT);
    }
}

// *************************************************************************************
// Navigates user to combat edit page from assign initiative page
// *************************************************************************************
void MainWindow::on_back_assignInit_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(EDIT);

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

    // Set round count and current player name labels
    ui->roundVal_label->setText(QString::number(combatManager->GetRound()));
    ui->playerName_label->setText(ui->activeCombatTable_tableWidget->item(0, 0)->text());

    combatManager->CheckForTie();
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
void MainWindow::FormatEditActorsTableView()
{
    editActorsModel = new DbEditTableModel(this, db);

    // Format editActors tableview
    ui->dbEdit_tableView->setModel(editActorsModel);
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
// Updates notes text browser to display selected actor's notes
// *************************************************************************************
void MainWindow::on_activeCombatTable_tableWidget_itemSelectionChanged()
{
    int currentRow;
    QString actorNotes;
    QString name;

    currentRow = ui->activeCombatTable_tableWidget->currentRow();

    ui->deleteActor_combat_pushButton->setDisabled(combatManager->IsDivider(currentRow));

    // Only displays selected actor's notes if valid actor
    if(combatManager->IsDivider(currentRow))
    {
        ui->notes_textEdit->clear();
        ui->notesName_label->clear();
    }
    else
    {
        actorNotes = ui->activeCombatTable_tableWidget->item(currentRow, tableManager->C_NOTES)->text();
        name = ui->activeCombatTable_tableWidget->item(currentRow, tableManager->C_NAME)->text();

        ui->notes_textEdit->setText(actorNotes);
        ui->notesName_label->setText(name);
    }
}

// *************************************************************************************
// Ends current player's turn by moving their row to bottom of table, updates round &
//  player name, and checks for tie
// *************************************************************************************
void MainWindow::on_endTurn_pushButton_clicked()
{
    combatManager->NextTurn();
    ui->roundVal_label->setText(QString::number(combatManager->GetRound()));
    ui->playerName_label->setText(ui->activeCombatTable_tableWidget->item(0, 0)->text());
    combatManager->CheckForTie();
}

// *************************************************************************************
//  Deletes the selected actor from the active combat table
// *************************************************************************************
void MainWindow::on_deleteActor_combat_pushButton_clicked()
{
    combatManager->DeleteActor();
    combatManager->CheckForTie();
    ui->endTurn_pushButton->setDisabled(combatManager->IsEmpty());
}

// *************************************************************************************
//   Moves selected actor from "Add Actor" table to "Combat" Table
// *************************************************************************************
void MainWindow::on_actorTable_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    tableManager->AddActorToTable(ui->actorTable_tableWidget, ui->combatTable_tableWidget);
}

// *************************************************************************************
//  Reformats scenario tableview to display scenario listing or actors for selected
//      scenario
// *************************************************************************************
void MainWindow::on_scenarioView_editScenario_comboBox_currentIndexChanged(const QString &arg1)
{
    FormatScenarioTableView(arg1);
}

// *************************************************************************************
//  Formats and sets table model for scenario tableview
// *************************************************************************************
void MainWindow::FormatScenarioTableView(QString scenarioName)
{
    DbEditTableModel *editScenarioModel = new DbEditTableModel(this, db);

    if(scenarioName == "All Scenarios")
    {
        editScenarioModel->InitializeScenarios();

        ui->scenarios_editScenario_tableView->setModel(editScenarioModel);
        ui->scenarios_editScenario_tableView->setColumnHidden(0, false);
    }
    else
    {
        editScenarioModel->InitializeScenarioByName(scenarioName);

        ui->scenarios_editScenario_tableView->setModel(editScenarioModel);
        ui->scenarios_editScenario_tableView->setColumnHidden(tableManager->D_ID, true);
        ui->scenarios_editScenario_tableView->setColumnHidden(7, true);
        ui->scenarios_editScenario_tableView->setColumnWidth(tableManager->D_NAME, 200);
        ui->scenarios_editScenario_tableView->setColumnWidth(tableManager->D_NOTES, 400);
    }

    ui->scenarios_editScenario_tableView->update();
}

// *************************************************************************************
//  Formats and sets table model for scenario actors table view
// *************************************************************************************
void MainWindow::FormatEditScenarioActorsTableView()
{
    DbEditTableModel *editScenarioActorsModel = new DbEditTableModel(this, db);

    ui->actors_editScenario_tableView->setModel(editScenarioActorsModel);
    ui->actors_editScenario_tableView->setColumnHidden(tableManager->D_ID, true);
    ui->actors_editScenario_tableView->setColumnWidth(tableManager->D_NOTES, 400);
    ui->actors_editScenario_tableView->setColumnWidth(tableManager->D_NAME, 200);
}

// *************************************************************************************
//  Initializes DB edit page when user navigates to that page
// *************************************************************************************
void MainWindow::on_main_stackedWidget_currentChanged(int arg1)
{
    QStringList scenarios;

    // Initialize DB edit page
    if(arg1 == DB_EDIT)
    {
        ui->dbEdit_tabWidget->setCurrentIndex(0);

        FormatEditActorsTableView();

        // Load scenario list into view dropdown
        db->CreateScenarioList();
        scenarios = db->GetScenarioList();
        scenarios.prepend("All Scenarios");

        ui->scenarioView_editScenario_comboBox->addItems(scenarios);

        FormatEditScenarioActorsTableView();
        FormatScenarioTableView(ui->scenarioView_editScenario_comboBox->currentText());
    }
}

// *************************************************************************************
//  Shows actor statistics fields, allowing user to add actor to database
// *************************************************************************************
void MainWindow::on_addActor_dbEdit_pushButton_clicked()
{
    // Label save button as 'add'
    ui->save_editActors_pushButton->setText("Add Actor");

    // Clear fields
    ui->name_editActors_lineEdit->clear();
    ui->hp_editActors_spinBox->clear();
    ui->ac_editActors_spinBox->clear();
    ui->dc_editActors_spinBox->clear();
    ui->notes_editActors_textEdit->clear();
    ui->type_editActors_comboBox->setCurrentIndex(DB_CREATURE);
}

// *************************************************************************************
//  Deletes actor from database
// *************************************************************************************
void MainWindow::on_deleteActor_dbEdit_pushButton_clicked()
{
    // Get selected row
    int row = ui->dbEdit_tableView->currentIndex().row();

    bool rowSelected =  row != -1;

    qDebug() << "Row Selected: " << row;

    // Pull ID from row
    int actorID = ui->dbEdit_tableView->model()->index(row,0).data().toInt();
    qDebug() << "Actor ID Selected: " << row;

    // Pull name from row for popup window
    QString name = ui->dbEdit_tableView->model()->index(row,1).data().toString();
    qDebug() << "Actor Name Selected: " << name;

    // Popup window asking if they want to delete that person
    QMessageBox warnPrompt;
    QString warnMsg = "Are you sure to want to delete " + name.toUpper() + " from the database?";

    qDebug() << "DEFAULT ROW SELECTED: " << ui->dbEdit_tableView->currentIndex().row();

    if(rowSelected)
    {
        warnPrompt.setIcon(QMessageBox::Warning);
        warnPrompt.setText("WARNING");
        warnPrompt.setInformativeText(warnMsg);
        warnPrompt.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

        if(warnPrompt.exec() == QMessageBox::Ok)
        {
            // START void DeleteActor(int actorID)
            QSqlQuery query;

            query.prepare("DELETE FROM actors WHERE actorID = :actorID");

            query.bindValue(":actorID", actorID);

            // Print error if unsuccessful
            if(!query.exec()) { qDebug() << query.lastError().text(); }

            // END void DeleteActor(int actorID)

            // Clear fields
            ui->name_editActors_lineEdit->clear();
            ui->hp_editActors_spinBox->clear();
            ui->ac_editActors_spinBox->clear();
            ui->dc_editActors_spinBox->clear();
            ui->notes_editActors_textEdit->clear();
            ui->type_editActors_comboBox->setCurrentIndex(DB_CREATURE);

            // Refresh table
            editActorsModel->select();
        }
    }
}

// *************************************************************************************
//  Saves changes (add/edit) to database
// *************************************************************************************
void MainWindow::on_save_editActors_pushButton_clicked()
{
    if(ui->save_editActors_pushButton->text() == "Add Actor")
    {
        // Create object
        Actor* toAdd;
        toAdd = new Actor;

        // String Testing
        QString notesText = ui->notes_editActors_textEdit->toPlainText();

        // Load field information into object
        toAdd->SetName(ui->name_editActors_lineEdit->text());
        toAdd->SetHitPoints(ui->hp_editActors_spinBox->text().toInt());
        toAdd->SetArmorClass(ui->ac_editActors_spinBox->text().toInt());
        toAdd->SetSpellSaveDC(ui->dc_editActors_spinBox->text().toInt());
        toAdd->SetNotes(ui->notes_editActors_textEdit->toPlainText());
        toAdd->SetType(ui->type_editActors_comboBox->currentText());

        // Add object to database
        db->AddActor(toAdd);

        // Refresh table
        editActorsModel->select();

        // Clear Fields
        ui->name_editActors_lineEdit->clear();
        ui->hp_editActors_spinBox->clear();
        ui->ac_editActors_spinBox->clear();
        ui->dc_editActors_spinBox->clear();
        ui->notes_editActors_textEdit->clear();

        // Destroy object
        delete toAdd;
    }
    else if (ui->save_editActors_pushButton->text() == "Save Changes")
    {
        // do the edit method here
    }
    else { qDebug() << "Add/Edit Switch Failure"; }
}

// *************************************************************************************
//  Help: Displays QMessageBox with instructions on how to use the database options
// *************************************************************************************
void MainWindow::on_help_dbEdit__pushButton_clicked()
{

}

// *************************************************************************************
//  Pulls data from tableview into text fields upon clicking any table entry
// *************************************************************************************
void MainWindow::on_dbEdit_tableView_clicked()
{
    // If not in edit mode, enter edit mode
    if(ui->save_editActors_pushButton->text() != "Save Changes")
    {
        ui->save_editActors_pushButton->setText("Save Changes");
    }

    // Get selected row
    int row = ui->dbEdit_tableView->currentIndex().row();
    qDebug() << "Row Selected: " << row;

    // Pull name from row for popup window
    QString name = ui->dbEdit_tableView->model()->index(row,1).data().toString();
    qDebug() << "Actor Name Selected: " << name;

    QString actorType = ui->dbEdit_tableView->model()->index(row, tableManager->D_TYPE).data().toString();

    // Pull info from table to fields
    ui->name_editActors_lineEdit->setText(ui->dbEdit_tableView->model()->index(row, tableManager->D_NAME).data().toString());
    ui->hp_editActors_spinBox->setValue(ui->dbEdit_tableView->model()->index(row, tableManager->D_HP).data().toInt());
    ui->ac_editActors_spinBox->setValue(ui->dbEdit_tableView->model()->index(row, tableManager->D_AC).data().toInt());
    ui->dc_editActors_spinBox->setValue(ui->dbEdit_tableView->model()->index(row, tableManager->D_DC).data().toInt());
    ui->notes_editActors_textEdit->setText(ui->dbEdit_tableView->model()->index(row, tableManager->D_NOTES).data().toString());

    // Set index on 'type' combobox
    if(actorType == "partymember") { ui->type_editActors_comboBox->setCurrentIndex(DB_PARTY); }
    else if(actorType == "creature") { ui->type_editActors_comboBox->setCurrentIndex(DB_CREATURE); }
    else if(actorType == "companion") { ui->type_editActors_comboBox->setCurrentIndex(DB_COMPANION); }
    else if(actorType == "effect") { ui->type_editActors_comboBox->setCurrentIndex(DB_EFFECT); }
    else { qDebug() << "Makeshift switch failed"; }
}
