#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QtGlobal>
#include <QObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->main_stackedWidget->setCurrentIndex(WELCOME);

    db = new Database("../itdb.db", "QSQLITE");

    FormatEditActorsTableView();

    tableManager = new TableModel;

    // Create tablewidgets
    combatTable = new QTableWidget;
    assignInit = new QTableWidget;
    combatManager = new CombatManager(ui->activeCombatTable_tableWidget);

    // Populate Combobox
    ui->showActors_comboBox->addItems(addActorsComboBoxLabels);

    // Set save status
    _saved = true;

    addActorForm = new AddActorForm(nullptr, db, ui->activeCombatTable_tableWidget);

    // DEBUG
    // Create scenario listings
    listings = new QVector<ScenarioListing>;
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
    tableManager->PopulateAddActorTable(ui->actorTable_tableWidget, db->GetNonPartyActors());

    // Populate "Combat" TableWidget
        // Initialize & populate "combatList" TableWidget
    tableManager->InitializeAddActorTable(ui->combatTable_tableWidget, tableManager->SelectedListColCount, tableManager->SelectedListColNames);
    tableManager->PopulateAddActorTable(ui->combatTable_tableWidget, db->GetPartyList());
    tableManager->InsertSpinBoxCol(ui->combatTable_tableWidget, 1, 1, tableManager->S_QTY, true, false);
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
        tableManager->InsertSpinBoxCol(ui->assignInit_tableWidget, 1, 30, tableManager->I_INIT, false, false);
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

    // TODO
    // This needs to call CreateActorList from db class because the
    // existing one has been altered.

    // And CreatePartyList probably since those have been mixed up
}

// *************************************************************************************
// Navigates user to database editor page from welcome page
// *************************************************************************************
void MainWindow::on_dbOpt_welcome_pushButton_clicked()
{
    // Navigate to Database Options Page
    ui->main_stackedWidget->setCurrentIndex(DB_EDIT);

    // Disable use of save button until appropriate option clicked
    ui->save_editActors_pushButton->setEnabled(false);
    ui->save_editActors_pushButton->setText("Save");
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
//  Enables save button
// *************************************************************************************
void MainWindow::EnableSaveButton()
{
    ui->saveChanges_editScenario_pushButton->setEnabled(true);
}

// *************************************************************************************
//  Formats and sets table model for scenario actors table view
// *************************************************************************************
void MainWindow::FormatEditScenarioActorsTableView()
{
    editScenarioActorsModel = new DbEditTableModel(this, db);

    ui->actors_editScenario_tableView->setModel(editScenarioActorsModel);
    ui->actors_editScenario_tableView->setColumnHidden(tableManager->D_ID, true);
    ui->actors_editScenario_tableView->setColumnWidth(tableManager->D_NOTES, 400);
    ui->actors_editScenario_tableView->setColumnWidth(tableManager->D_NAME, 200);
    ui->actors_editScenario_tableView->verticalHeader()->hide();
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
        scenarios = db->GetScenarioList();
        scenarios.prepend("All Scenarios");

        if(ui->scenarioView_editScenario_comboBox->count()==0)
        {
            ui->scenarioView_editScenario_comboBox->addItems(scenarios);
        }

        FormatEditScenarioActorsTableView();
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
    ClearDBFields();

    // Enable 'save' button usage
    ui->save_editActors_pushButton->setEnabled(true);
}

// *************************************************************************************
//  Deletes actor from database
// *************************************************************************************
void MainWindow::on_deleteActor_dbEdit_pushButton_clicked()
{
    // Get selected row
    int row = ui->dbEdit_tableView->currentIndex().row();
    bool rowSelected =  row != -1;

    // Pull ID from row
    int actorID = ui->dbEdit_tableView->model()->index(row,0).data().toInt();

    // Pull name from row for popup window
    QString name = ui->dbEdit_tableView->model()->index(row,1).data().toString();

    // Popup window asking if they want to delete that person
    QMessageBox warnPrompt;
    QString warnMsg = "Are you sure to want to delete " + name.toUpper() + " from the database?";

    if(rowSelected)
    {
        warnPrompt.setIcon(QMessageBox::Warning);
        warnPrompt.setText("WARNING");
        warnPrompt.setInformativeText(warnMsg);
        warnPrompt.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

        if(warnPrompt.exec() == QMessageBox::Ok)
        {
            // Delete actor
            db->DeleteActor(actorID);

            // Clear fields
            ClearDBFields();

            // Refresh table
            editActorsModel->select();

            // Disable button until further action taken
            ui->save_editActors_pushButton->setEnabled(false);
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
        ClearDBFields();

        // Disable button until further action taken
        ui->save_editActors_pushButton->setEnabled(false);

        ui->save_editActors_pushButton->setText("Save");

        // Destroy object
        delete toAdd;
    }
    else if (ui->save_editActors_pushButton->text() == "Save Changes")
    {
        // Get selected row
        int row = ui->dbEdit_tableView->currentIndex().row();
        qDebug() << "Row Selected: " << row;

        // Determine if row is selected
        bool rowSelected = row != -1;

        if(rowSelected)
        {
            // Create object
            Actor* toEdit;
            toEdit = new Actor;

            // Load field information into object
            toEdit->SetID(ui->dbEdit_tableView->model()->index(row,0).data().toInt());
            toEdit->SetName(ui->name_editActors_lineEdit->text());
            toEdit->SetHitPoints(ui->hp_editActors_spinBox->text().toInt());
            toEdit->SetArmorClass(ui->ac_editActors_spinBox->text().toInt());
            toEdit->SetSpellSaveDC(ui->dc_editActors_spinBox->text().toInt());
            toEdit->SetNotes(ui->notes_editActors_textEdit->toPlainText());
            toEdit->SetType(ui->type_editActors_comboBox->currentText());

            // Debug: See if ID is selected
            qDebug() << "Actor ID Selected: " << row;

            db->EditActor(toEdit);

            // Refresh table
            editActorsModel->select();

            // Clear Fields
            ClearDBFields();

            // Disable button until further action taken
            ui->save_editActors_pushButton->setEnabled(false);

            // Reset button text
            ui->save_editActors_pushButton->setText("Save");

            // Destroy object
            delete toEdit;
        }
    }
    else { qDebug() << "Add/Edit Switch Failure"; }
}

// *************************************************************************************
//  Displays QMessageBox with instructions on how to use the database options
// *************************************************************************************
void MainWindow::on_help_dbEdit__pushButton_clicked()
{
    //TODO Create popup warning window with instructions for database options
}

// *************************************************************************************
//  Pulls data from tableview into text fields upon clicking any table entry
// *************************************************************************************
void MainWindow::on_dbEdit_tableView_clicked()
{
    // If not in edit mode, enter edit mode
    if(ui->save_editActors_pushButton->text() != "Save Changes")
    { ui->save_editActors_pushButton->setText("Save Changes"); }

    // Enable use of save button
    ui->save_editActors_pushButton->setEnabled(true);

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

// *************************************************************************************
// Resets all objects on the edit scenarios tab
// *************************************************************************************
void MainWindow::on_dbEdit_tabWidget_currentChanged(int index)
{
    if(index == EDIT_SCENARIOS)
    {
        // Reset buttons
        ui->add_editScenario_pushButton->setText("Create New Scenario");
        ui->remove_editScenario_pushButton->setEnabled(false);
        ui->saveChanges_editScenario_pushButton->setEnabled(false);

        // Reset dropdown
        tableManager->InitializeAddActorTable(ui->scenarios_editScenario_tableWidget, tableManager->AllScenarioColCount, tableManager->AllScenarioColNames);
        tableManager->PopulateScenarioNameTable(ui->scenarios_editScenario_tableWidget, db->GetScenarioList());
        ui->scenarioView_editScenario_comboBox->setCurrentIndex(0);

        // Show table data
        ui->scenarios_editScenario_tableWidget->setColumnHidden(0, false);
    }
}


// *************************************************************************************
//  PushButton for user to clear fields on DB edit page
// *************************************************************************************
void MainWindow::on_clear_editActors_pushButton_clicked()
{
    ClearDBFields();
}

// Helper method clears Database entry lineedits
void MainWindow::ClearDBFields()
{
    ui->name_editActors_lineEdit->clear();
    ui->hp_editActors_spinBox->clear();
    ui->ac_editActors_spinBox->clear();
    ui->dc_editActors_spinBox->clear();
    ui->notes_editActors_textEdit->clear();
    ui->type_editActors_comboBox->setCurrentIndex(DB_CREATURE);

}

// *************************************************************************************
//  Edit-Scenario - PushButton "Add" for user to create scenarios or add actors to existing scenario
// *************************************************************************************
void MainWindow::on_add_editScenario_pushButton_clicked()
{
    // New Scenario Creation
    if(ui->scenarioView_editScenario_comboBox->currentIndex() == 0)
    {
        bool ok = false;
        QInputDialog scenarioDialogue;

        // Get scenario name from user
        QString scenarioName = scenarioDialogue.getText(this, "Create New Scenario", "Scenario Name: ", QLineEdit::Normal, "", &ok);

        // While scenario name is empty, reprompt
        while (ok && scenarioName.isEmpty())
        {
            scenarioName = scenarioDialogue.getText(this, "Create New Scenario", "Invalid Input. Please Enter Scenario name: ", QLineEdit::Normal, "", &ok);
        }

        if(scenarioName != "")
        {
            // Add scenario name to combobox list
            ui->scenarioView_editScenario_comboBox->addItem(scenarioName);

            // Select new scenario
            ui->scenarioView_editScenario_comboBox->setCurrentIndex(ui->scenarioView_editScenario_comboBox->count() -1);

            // Initialize tablewidget
            tableManager->InitializeScenarioTable(ui->scenarios_editScenario_tableWidget, tableManager->SpecificScenarioColCount, tableManager->SpecificScenarioColNames);

            // Deactivate "add button"
            if(ui->add_editScenario_pushButton->isEnabled())
            { ui->add_editScenario_pushButton->setEnabled(false); }

            // Set save status
            if(_saved == true)
            { _saved = false; }
        }
    }
    else // Add actor to existing scenario
    {
        // Enable save button
        if(!ui->saveChanges_editScenario_pushButton->isEnabled())
        { ui->saveChanges_editScenario_pushButton->setEnabled(true); }

        // Set save status
        if(_saved == true)
        { _saved = false; }

        // If user has selected a valid row
        if(ui->actors_editScenario_tableView->currentIndex().row() != -1)
        {
            int row = ui->actors_editScenario_tableView->currentIndex().row(); // Row of table
            int IDtoAdd; // Actor's ID to add
            bool presentOnList = false;
            int index;

            // Get actor's name from selected row
            IDtoAdd = ui->actors_editScenario_tableView->model()->index(row,0).data().toInt();

            // Check to see if actor is already on tablewidget
            index = 0;
            presentOnList = IDtoAdd == ui->scenarios_editScenario_tableWidget->model()->index(index,tableManager->SC_ID).data().toInt();

            while(!presentOnList && index < ui->scenarios_editScenario_tableWidget->rowCount())
            {
                index++;
                presentOnList = IDtoAdd == ui->scenarios_editScenario_tableWidget->model()->index(index,tableManager->SC_ID).data().toInt();
            }// End if present on list and still in row

            if(presentOnList)
            {
                // Pop up window informing user that listing is already present
                QMessageBox warnPrompt;

                QString warningMsg = ui->scenarios_editScenario_tableWidget->model()->index(index,tableManager->SC_NAME).data().toString() + " is already present in this scenario. Please select a new actor.";

                warnPrompt.setIcon(QMessageBox::Warning);
                warnPrompt.setText("WARNING");
                warnPrompt.setInformativeText(warningMsg);
                warnPrompt.setStandardButtons(QMessageBox::Ok);
                warnPrompt.exec();
            }
            else
            {
                // Collect data from tableview
                Actor* toAdd = new Actor;
                toAdd->SetID(ui->actors_editScenario_tableView->model()->index(row,tableManager->SC_ID).data().toInt());
                toAdd->SetName(ui->actors_editScenario_tableView->model()->index(row,tableManager->SC_NAME).data().toString());
                toAdd->SetHitPoints(ui->actors_editScenario_tableView->model()->index(row,tableManager->SC_HP).data().toInt());
                toAdd->SetArmorClass(ui->actors_editScenario_tableView->model()->index(row,tableManager->SC_AC).data().toInt());
                toAdd->SetSpellSaveDC(ui->actors_editScenario_tableView->model()->index(row,tableManager->SC_DC).data().toInt());
                toAdd->SetNotes(ui->actors_editScenario_tableView->model()->index(row,tableManager->SC_NOTES).data().toString());
                toAdd->SetType(ui->actors_editScenario_tableView->model()->index(row,tableManager->SC_TYPE).data().toString());

                // Add listing to tablewidget
                tableManager->AddActorToScenarioTable(ui->scenarios_editScenario_tableWidget, toAdd);

                // Connect combobox to signal
                QObject::connect(tableManager->spinBoxes->at(tableManager->spinBoxes->size()-1), SIGNAL(valueChanged(int)), this, SLOT(EnableSaveButton()));
            }
        }
    }
}

// *************************************************************************************
//  Edit Scenario - Configures scenario ui buttons for when top actors tableview clicked
// *************************************************************************************
void MainWindow::on_actors_editScenario_tableView_clicked(const QModelIndex &index)
{
    // If "All Scenarios" selected on combobox
    if(ui->scenarioView_editScenario_comboBox->currentText() == "All Scenarios")
    {
        // Configure UI Buttons
        ConfigureScenarioUIButtons (true, "Create New Scenario", false, "Delete Scenario");
    }
    else // Specific scenario selected
    {
        // Configure UI Buttons
        ConfigureScenarioUIButtons (true, "Add Actor", false, "Remove Actor");
    }
}

// *************************************************************************************
//  DB - Edit Scenario - Configures scenario ui buttons for when bottom scenarios tablewidget clicked
// *************************************************************************************
void MainWindow::on_scenarios_editScenario_tableWidget_itemClicked(QTableWidgetItem *item)
{
    // If "All Scenarios" selected on combobox
    if(ui->scenarioView_editScenario_comboBox->currentText() == "All Scenarios")
    {
        // Configure UI Buttons
        ConfigureScenarioUIButtons (true, "Create New Scenario", false, "Delete Scenario");
    }
    else // Specific scenario selected
    {
        // // Configure UI Buttons
        ConfigureScenarioUIButtons (false, "Add Actor", true, "Remove Actor");
    }
}

// *************************************************************************************
//  Saves changes made to edit scenarios table widget when 'save changes' button is clicked
// *************************************************************************************
void MainWindow::on_saveChanges_editScenario_pushButton_clicked()
{
    QVector<ScenarioListing> *listings;
    listings = new QVector<ScenarioListing>;
    ScenarioListing singleListing;

    for(int index = 0; index < ui->scenarios_editScenario_tableWidget->rowCount(); index++)
    {
        // Collect data from table
        singleListing._id = ui->scenarios_editScenario_tableWidget->model()->index(index, tableManager->SC_ID).data().toInt();
        singleListing._scenarioName = ui->scenarioView_editScenario_comboBox->currentText();
        QSpinBox* sBox = qobject_cast<QSpinBox*>(ui->scenarios_editScenario_tableWidget->cellWidget(index, tableManager->SC_QTY));
        singleListing._qty = sBox->value();

        // Load listing into vector
        listings->append(singleListing);
    }

    // Overwrite old scenario listing
    db->SaveChangesToScenario(listings);

    // Disable button
    ui->saveChanges_editScenario_pushButton->setDisabled(true);

    // Set save status
    if(_saved == false)
    { _saved = true; }
}

// *************************************************************************************
//  Removes scenario or actor in scenario depending on table widget view
// *************************************************************************************
void MainWindow::on_remove_editScenario_pushButton_clicked()
{
    // Get Row
    int row = ui->scenarios_editScenario_tableWidget->currentRow();
    bool rowSelected = row != -1;

    // User selected "All Scenarios" their intention is to remove an entire scenario
    if(ui->scenarioView_editScenario_comboBox->currentText() == "All Scenarios")
    {
        // Pop warning to user
        QMessageBox warnPrompt;
        QString scenarioToDelete = ui->scenarios_editScenario_tableWidget->model()->index(row, 0).data().toString();
        QString warnMsg = "This will delete scenario: " + scenarioToDelete + ". Do you wish to continue?";
        warnPrompt.setIcon(QMessageBox::Warning);
        warnPrompt.setText("WARNING");
        warnPrompt.setInformativeText(warnMsg);
        warnPrompt.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

        if(warnPrompt.exec() == QMessageBox::Ok)
        {
            // Delete scenario from db
            db->DeleteScenario(scenarioToDelete);

            tableManager->InitializeAddActorTable(ui->scenarios_editScenario_tableWidget, tableManager->AllScenarioColCount, tableManager->AllScenarioColNames);
            tableManager->PopulateScenarioNameTable(ui->scenarios_editScenario_tableWidget, db->GetScenarioList());

            // Remove scenario from combobox
            ui->scenarioView_editScenario_comboBox->removeItem(ui->scenarioView_editScenario_comboBox->findText(scenarioToDelete));

            // Set save status
            if(_saved == true)
            { _saved = false; }
        }
    }
    else // Else, they've chosen a specific scenario, and wish to remove an actor
    {
        // Get actor's name to remove
        QString name = ui->scenarios_editScenario_tableWidget->model()->index(row,tableManager->SC_NAME).data().toString();

        // If user has selected valid row, pop warning
        if(rowSelected)
        {
                // remove actor from bottom table widget
                ui->scenarios_editScenario_tableWidget->removeRow(row);

                // Set save status
                if(_saved == true)
                { _saved = false; }
        }
        else // If user has not selected valid row, inform them
        {
            QMessageBox warnPrompt;
            warnPrompt.setIcon(QMessageBox::Warning);
            warnPrompt.setText("WARNING");
            warnPrompt.setInformativeText("Please select an Actor to remove first!");
            warnPrompt.setStandardButtons(QMessageBox::Ok);
            warnPrompt.exec();
        }
    }

    // Configure UI Buttons
    ui->remove_editScenario_pushButton->setEnabled(false);
    ui->saveChanges_editScenario_pushButton->setEnabled(true);
}

// *************************************************************************************
//  Reformats scenario tableview to display scenario listing or actors for selected
//  scenario
// *************************************************************************************
void MainWindow::on_scenarioView_editScenario_comboBox_activated(int index)
{
    // Inform user they have created an empty scenario and it will not be saved unless populated
    if(_saved == false)
    { 
        QMessageBox emptyScenario;
        emptyScenario.setIcon(QMessageBox::Warning);
        emptyScenario.setText("WARNING");
        emptyScenario.setInformativeText("You haven't added any actors to your new scenario. Would you like to leave without saving?");
        emptyScenario.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

        // "I want to leave without saving changes"
        if(emptyScenario.exec() == QMessageBox::Ok)
        {
            // Remove newly-created listing from combobox
            ui->scenarioView_editScenario_comboBox->removeItem(ui->scenarioView_editScenario_comboBox->count() - 1);

            // Set saved since user is fine with leaving
            _saved = true;
        }
        else // "I want to stay and populate the scenario"
        {
            ui->scenarioView_editScenario_comboBox->setCurrentIndex(ui->scenarioView_editScenario_comboBox->count() - 1);
        }
    }

    // TODO if user attempts to leave the current scenario and it has unsaved changes, inform them
    if(ui->saveChanges_editScenario_pushButton->isEnabled())
    {
        // Inform user they have not saved changes
        QMessageBox unsavedScenarioChanges;
        QAbstractButton* saveButton = unsavedScenarioChanges.addButton(tr("Save Changes"), QMessageBox::YesRole);
        unsavedScenarioChanges.setIcon(QMessageBox::Warning);
        unsavedScenarioChanges.setText("WARNING");
        unsavedScenarioChanges.setInformativeText("Unsaved changes will be lost. Would you like to leave without saving?");
        unsavedScenarioChanges.addButton(tr("Leave without saving"), QMessageBox::NoRole);
        unsavedScenarioChanges.exec();

        // If user selects "Save, please" then do so.
        // TODO If there's a way to force the button to click instead of copy/pasting the code from button below, that'd be nice
        if(unsavedScenarioChanges.clickedButton()==saveButton)
        {
            // Initialize list
            listings->clear();

            // Pull data from table
            for(int index = 0; index < ui->scenarios_editScenario_tableWidget->rowCount(); index++)
            {
                // Collect data from table
                singleListing._id = ui->scenarios_editScenario_tableWidget->model()->index(index, tableManager->SC_ID).data().toInt();
                singleListing._scenarioName = previousText;
                QSpinBox* sBox = qobject_cast<QSpinBox*>(ui->scenarios_editScenario_tableWidget->cellWidget(index, tableManager->SC_QTY));
                singleListing._qty = sBox->value();

                // Load listing into vector
                listings->append(singleListing);
            }

            // Write to db
            db->SaveChangesToScenario(listings);

            // Change listing
            // If user has selected "all scenarios"
            if(ui->scenarioView_editScenario_comboBox->currentText() == "All Scenarios")
            {
                // Configure UI buttons
                ConfigureScenarioUIButtons (true, "Create New Scenario", false, "Delete Scenario");

                // Initialize and populate bottom tableWidget
                tableManager->InitializeAddActorTable(ui->scenarios_editScenario_tableWidget, tableManager->AllScenarioColCount, tableManager->AllScenarioColNames);
                tableManager->PopulateScenarioNameTable(ui->scenarios_editScenario_tableWidget, db->GetScenarioList());
                ui->scenarios_editScenario_tableWidget->setColumnHidden(0, false);
            }
            else // Specific scenario is selected
            {
                // Configure UI buttons
                ConfigureScenarioUIButtons (true, "Create New Scenario", false, "Remove Actor");

                // Disable save button
                ui->saveChanges_editScenario_pushButton->setEnabled(false);

                // Initialize, populate, and format bottom tablewidget
                tableManager->InitializeScenarioTable(ui->scenarios_editScenario_tableWidget, tableManager->SpecificScenarioColCount, tableManager->SpecificScenarioColNames);
                tableManager->PopulateSelectedScenarioTable(ui->scenarios_editScenario_tableWidget, db->GetActorsByScenario(ui->scenarioView_editScenario_comboBox->currentText()));
                tableManager->InsertSpinBoxCol(ui->scenarios_editScenario_tableWidget, tableManager->qtyMin, tableManager->qtyMax, tableManager->SC_QTY, false, true);

                // Disable spinboxes for partymembers
                for(int index = 0; index < ui->scenarios_editScenario_tableWidget->rowCount(); index++)
                {
                    if(ui->scenarios_editScenario_tableWidget->model()->index(index,tableManager->SC_TYPE).data().toString() == "partymember")
                    {
                        if(tableManager->spinBoxes->at(index)->isEnabled())
                        { tableManager->spinBoxes->at(index)->setDisabled(true); }
                    }
                }

                //Populate Spinboxes with quantities from database
                QVector<int>* scenarioQtys = new QVector<int>;
                scenarioQtys = db->GetScenarioQtys(ui->scenarioView_editScenario_comboBox->currentText());

                // Assign quantities to spinboxes
                for(int index = 0; index < ui->scenarios_editScenario_tableWidget->rowCount(); index++)
                {
                    // Insert quantity into spinbox
                    tableManager->spinBoxes->at(index)->setValue(scenarioQtys->at(index));
                }

                // Activate signals in spinbox
                for(int index = 0; index < tableManager->spinBoxes->size(); index ++)
                {
                    QObject::connect(tableManager->spinBoxes->at(index), SIGNAL(valueChanged(int)), this, SLOT(EnableSaveButton()));
                }
            }
        } // End save changes
        else // Leave without saving
        {
            // If user has selected "all scenarios"
            if(ui->scenarioView_editScenario_comboBox->currentText() == "All Scenarios")
            {
                // Configure UI buttons
                ConfigureScenarioUIButtons (true, "Create New Scenario", false, "Delete Scenario");

                // Initialize and populate bottom tableWidget
                tableManager->InitializeAddActorTable(ui->scenarios_editScenario_tableWidget, tableManager->AllScenarioColCount, tableManager->AllScenarioColNames);
                tableManager->PopulateScenarioNameTable(ui->scenarios_editScenario_tableWidget, db->GetScenarioList());
                ui->scenarios_editScenario_tableWidget->setColumnHidden(0, false);
            }
            else // Specific scenario is selected
            {
                // TODOCONFIG
                ConfigureScenarioUIButtons (true, "Create New Scenario", false, "Remove Actor");

                // Initialize, populate, and format bottom tablewidget
                tableManager->InitializeScenarioTable(ui->scenarios_editScenario_tableWidget, tableManager->SpecificScenarioColCount, tableManager->SpecificScenarioColNames);
                tableManager->PopulateSelectedScenarioTable(ui->scenarios_editScenario_tableWidget, db->GetActorsByScenario(ui->scenarioView_editScenario_comboBox->currentText()));
                tableManager->InsertSpinBoxCol(ui->scenarios_editScenario_tableWidget, tableManager->qtyMin, tableManager->qtyMax, tableManager->SC_QTY, false, true);

                // Disable spinboxes for partymembers
                for(int index = 0; index < ui->scenarios_editScenario_tableWidget->rowCount(); index++)
                {
                    if(ui->scenarios_editScenario_tableWidget->model()->index(index,tableManager->SC_TYPE).data().toString() == "partymember")
                    {
                        if(tableManager->spinBoxes->at(index)->isEnabled())
                        { tableManager->spinBoxes->at(index)->setDisabled(true); }
                    }
                }

                //Populate Spinboxes with quantities from database
                QVector<int>* scenarioQtys = new QVector<int>;
                scenarioQtys = db->GetScenarioQtys(ui->scenarioView_editScenario_comboBox->currentText());

                // Assign quantities to spinboxes
                for(int index = 0; index < ui->scenarios_editScenario_tableWidget->rowCount(); index++)
                {
                    // Insert quantity into spinbox
                    tableManager->spinBoxes->at(index)->setValue(scenarioQtys->at(index));
                }

                // Activate signals in spinbox
                for(int index = 0; index < tableManager->spinBoxes->size(); index ++)
                {
                    QObject::connect(tableManager->spinBoxes->at(index), SIGNAL(valueChanged(int)), this, SLOT(EnableSaveButton()));

                } // End activate signals in spinbox
            } // End populate specific scenario
        } // End leave without saving

        // Set save status
        if(_saved == false)
        { _saved = true; }

        ui->saveChanges_editScenario_pushButton->setEnabled(false);

    } // End unsaved changes found
    else // If no unsaved changes, navigate away
    {
        // If user has selected "all scenarios"
        if(ui->scenarioView_editScenario_comboBox->currentText() == "All Scenarios")
        {
            // Configure UI Buttons
            ConfigureScenarioUIButtons (true, "Create New Scenario", false, "Delete Scenario");

            // Initialize and populate bottom tableWidget
            tableManager->InitializeAddActorTable(ui->scenarios_editScenario_tableWidget, tableManager->AllScenarioColCount, tableManager->AllScenarioColNames);
            tableManager->PopulateScenarioNameTable(ui->scenarios_editScenario_tableWidget, db->GetScenarioList());
            ui->scenarios_editScenario_tableWidget->setColumnHidden(0, false);
        }
        else // Specific scenario is selected
        {
            // Configure UI Buttons
            ConfigureScenarioUIButtons (true, "Create New Scenario", false, "Remove Actor");

            // Initialize, populate, and format bottom tablewidget
            tableManager->InitializeScenarioTable(ui->scenarios_editScenario_tableWidget, tableManager->SpecificScenarioColCount, tableManager->SpecificScenarioColNames);
            tableManager->PopulateSelectedScenarioTable(ui->scenarios_editScenario_tableWidget, db->GetActorsByScenario(ui->scenarioView_editScenario_comboBox->currentText()));
            tableManager->InsertSpinBoxCol(ui->scenarios_editScenario_tableWidget, tableManager->qtyMin, tableManager->qtyMax, tableManager->SC_QTY, false, true);

            // Disable spinboxes for partymembers
            for(int index = 0; index < ui->scenarios_editScenario_tableWidget->rowCount(); index++)
            {
                if(ui->scenarios_editScenario_tableWidget->model()->index(index,tableManager->SC_TYPE).data().toString() == "partymember")
                {
                    if(tableManager->spinBoxes->at(index)->isEnabled())
                    { tableManager->spinBoxes->at(index)->setDisabled(true); }
                }
            }

            //Populate Spinboxes with quantities from database
            QVector<int>* scenarioQtys = new QVector<int>;
            scenarioQtys = db->GetScenarioQtys(ui->scenarioView_editScenario_comboBox->currentText());

            // Assign quantities to spinboxes
            for(int index = 0; index < ui->scenarios_editScenario_tableWidget->rowCount(); index++)
            {
                // Insert quantity into spinbox
                tableManager->spinBoxes->at(index)->setValue(scenarioQtys->at(index));
            }

            // Activate signals in spinbox
            for(int index = 0; index < tableManager->spinBoxes->size(); index ++)
            {
                QObject::connect(tableManager->spinBoxes->at(index), SIGNAL(valueChanged(int)), this, SLOT(EnableSaveButton()));

            }
        }
    }

    // Save old index and text
    previousIndex = ui->scenarioView_editScenario_comboBox->currentIndex();
    previousText = ui->scenarioView_editScenario_comboBox->currentText();
}

// *************************************************************************************
// Configures the add/remove buttons text and enable status with the passed in values
// *************************************************************************************
void MainWindow::ConfigureScenarioUIButtons (const bool &addButtonStatus, const QString &addButtonText, const bool &deleteButtonStatus, const QString &deleteButtonText)
{
    // Enable/disable add button
    ui->add_editScenario_pushButton->setEnabled(addButtonStatus);

    // Set add button text
    ui->add_editScenario_pushButton->setText(addButtonText);

    // Enable/disable delete button
    ui->remove_editScenario_pushButton->setEnabled(deleteButtonStatus);

    // Set delete button text
    ui->remove_editScenario_pushButton->setText(deleteButtonText);
}

// Adding actor to scenario through doubleclick
void MainWindow::on_actors_editScenario_tableView_doubleClicked(const QModelIndex &index)
{
    // If user has selected specific scenario
    if(ui->scenarioView_editScenario_comboBox->currentText() != "All Scenarios")
    {
        // "Add Actor" code from add_editScenario_pushbutton_clicked()
        // Enable save button
        if(!ui->saveChanges_editScenario_pushButton->isEnabled())
        { ui->saveChanges_editScenario_pushButton->setEnabled(true); }

        // Set save status
        if(_saved == true)
        { _saved = false; }

        // If user has selected a valid row
        if(ui->actors_editScenario_tableView->currentIndex().row() != -1)
        {
            int row = ui->actors_editScenario_tableView->currentIndex().row(); // Row of table
            int IDtoAdd; // Actor's ID to add
            bool presentOnList = false;
            int index;

            // Get actor's name from selected row
            IDtoAdd = ui->actors_editScenario_tableView->model()->index(row,0).data().toInt();

            // Check to see if actor is already on tablewidget
            index = 0;
            presentOnList = IDtoAdd == ui->scenarios_editScenario_tableWidget->model()->index(index,tableManager->SC_ID).data().toInt();

            while(!presentOnList && index < ui->scenarios_editScenario_tableWidget->rowCount())
            {
                index++;
                presentOnList = IDtoAdd == ui->scenarios_editScenario_tableWidget->model()->index(index,tableManager->SC_ID).data().toInt();
            }// End if present on list and still in row

            if(presentOnList)
            {
                // Pop up window informing user that listing is already present
                QMessageBox warnPrompt;

                QString warningMsg = ui->scenarios_editScenario_tableWidget->model()->index(index,tableManager->SC_NAME).data().toString() + " is already present in this scenario. Please select a new actor.";

                warnPrompt.setIcon(QMessageBox::Warning);
                warnPrompt.setText("WARNING");
                warnPrompt.setInformativeText(warningMsg);
                warnPrompt.setStandardButtons(QMessageBox::Ok);
                warnPrompt.exec();
            }
            else
            {
                // Collect data from tableview
                Actor* toAdd = new Actor;
                toAdd->SetID(ui->actors_editScenario_tableView->model()->index(row,tableManager->SC_ID).data().toInt());
                toAdd->SetName(ui->actors_editScenario_tableView->model()->index(row,tableManager->SC_NAME).data().toString());
                toAdd->SetHitPoints(ui->actors_editScenario_tableView->model()->index(row,tableManager->SC_HP).data().toInt());
                toAdd->SetArmorClass(ui->actors_editScenario_tableView->model()->index(row,tableManager->SC_AC).data().toInt());
                toAdd->SetSpellSaveDC(ui->actors_editScenario_tableView->model()->index(row,tableManager->SC_DC).data().toInt());
                toAdd->SetNotes(ui->actors_editScenario_tableView->model()->index(row,tableManager->SC_NOTES).data().toString());
                toAdd->SetType(ui->actors_editScenario_tableView->model()->index(row,tableManager->SC_TYPE).data().toString());

                // Add listing to tablewidget
                tableManager->AddActorToScenarioTable(ui->scenarios_editScenario_tableWidget, toAdd);

                // Connect combobox to signal
                QObject::connect(tableManager->spinBoxes->at(tableManager->spinBoxes->size()-1), SIGNAL(valueChanged(int)), this, SLOT(EnableSaveButton()));
            }
        }
    }
}

// User can double-click actor from scenario to remove them or double-click scenario name to navigate to scenario
void MainWindow::on_scenarios_editScenario_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    // Get row
    int row = item->row();
    // If user has selected scenario name, navigate to scenario listing
    if(ui->scenarioView_editScenario_comboBox->currentText() == "All Scenarios")
    {
        // Get name of scenario to find
        QString scenarioName = ui->scenarios_editScenario_tableWidget->model()->index(item->row(),tableManager->SC_ID).data().toString();

        // Find index of scenario in combobox
        int indexFound = ui->scenarioView_editScenario_comboBox->findText(scenarioName);

        // Navigate to index of listing
        ui->scenarioView_editScenario_comboBox->setCurrentIndex(indexFound);

        // Initialize, populate, and format bottom tablewidget
        tableManager->InitializeScenarioTable(ui->scenarios_editScenario_tableWidget, tableManager->SpecificScenarioColCount, tableManager->SpecificScenarioColNames);
        tableManager->PopulateSelectedScenarioTable(ui->scenarios_editScenario_tableWidget, db->GetActorsByScenario(ui->scenarioView_editScenario_comboBox->currentText()));
        tableManager->InsertSpinBoxCol(ui->scenarios_editScenario_tableWidget, tableManager->qtyMin, tableManager->qtyMax, tableManager->SC_QTY, false, true);


    }
    else // If user has selected an actor, remove them from scenario
    {
        // remove actor from bottom table widget
        ui->scenarios_editScenario_tableWidget->removeRow(row);

        // Set save status
        if(_saved == true)
        { _saved = false; }
    }
}
