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
//  Reformats scenario tableview to display scenario listing or actors for selected
//      scenario
// *************************************************************************************
void MainWindow::on_scenarioView_editScenario_comboBox_currentIndexChanged(const QString &arg1)
{
    // If user has selected "all scenarios"
    if(arg1 == "All Scenarios")
    {
        // Change add button text
        if(ui->add_editScenario_pushButton->text() != "Create New Scenario")
        {
            ui->add_editScenario_pushButton->setText("Create New Scenario");
        }

        // Disable delete button
        if(ui->remove_editScenario_pushButton->isEnabled())
        {
            ui->remove_editScenario_pushButton->setEnabled(false);
        }

        // TODO Top Tableview - Load all actors from db? Is this necessary?


        // Initialize and populate bottom tableWidget
        // TODO: figure out a generic name for this method since it affects more than one type of table, not just 'add actor'
        tableManager->InitializeAddActorTable(ui->scenarios_editScenario_tableWidget, tableManager->AllScenarioColCount, tableManager->AllScenarioColNames);
        tableManager->PopulateScenarioNameTable(ui->scenarios_editScenario_tableWidget, db->GetScenarioList());

    }
    else // Specific scenario is selected
    {
        // Change add button text
        if(ui->add_editScenario_pushButton->text() != "Create New Scenario")
        {
            ui->add_editScenario_pushButton->setText("Create New Scenario");
        }

        // Disable delete button
        if(ui->remove_editScenario_pushButton->isEnabled())
        {
            ui->remove_editScenario_pushButton->setEnabled(false);
        }

        // Change delete button text
        if(ui->remove_editScenario_pushButton->text() != "Remove Actor")
        {
            ui->remove_editScenario_pushButton->setText("Remove Actor");
        }

        // Initialize, populate, and format bottom tablewidget
        tableManager->InitializeScenarioTable(ui->scenarios_editScenario_tableWidget, tableManager->SpecificScenarioColCount, tableManager->SpecificScenarioColNames);
        tableManager->PopulateSelectedScenarioTable(ui->scenarios_editScenario_tableWidget, db->GetActorsByScenario(ui->scenarioView_editScenario_comboBox->currentText()));
        tableManager->InsertSpinBoxCol(ui->scenarios_editScenario_tableWidget, tableManager->qtyMin, tableManager->qtyMax, tableManager->SC_QTY, false, true);

        // Activate signals in spinbox
        for(int index = 0; index < tableManager->spinBoxes->size(); index ++)
        {
            QObject::connect(tableManager->spinBoxes->at(index), SIGNAL(valueChanged(int)), this, SLOT(EnableSaveButton()));

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
    }

    // TODO fix this up
    // commented out because this function will likely be changed
    //FormatScenarioTableView(arg1);
}

void MainWindow::EnableSaveButton()
{
    ui->saveChanges_editScenario_pushButton->setEnabled(true);
}

// *************************************************************************************
//  Formats and sets table model for scenario tableview
// *************************************************************************************
void MainWindow::FormatScenarioTableView(QString scenarioName)
{
    editScenarioModel = new DbEditTableModel(this, db);

    // TODO - Fix deadcode

    if(scenarioName == "All Scenarios")
    {
//        editScenarioModel->InitializeScenarios();

//        ui->scenarios_editScenario_tableView->setModel(editScenarioModel);
//        ui->scenarios_editScenario_tableView->setColumnHidden(0, false);
    }
    else
    {
//        editScenarioModel->InitializeScenarioByName(scenarioName);

//        ui->scenarios_editScenario_tableView->setModel(editScenarioModel);
//        ui->scenarios_editScenario_tableView->setColumnHidden(tableManager->D_ID, true);
//        ui->scenarios_editScenario_tableView->setColumnHidden(7, true);
//        ui->scenarios_editScenario_tableView->setColumnWidth(tableManager->D_NAME, 200);
//        ui->scenarios_editScenario_tableView->setColumnWidth(tableManager->D_NOTES, 400);
    }

//    ui->scenarios_editScenario_tableView->update();
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
        db->CreateScenarioList();
        scenarios = db->GetScenarioList();
        scenarios.prepend("All Scenarios");

        if(ui->scenarioView_editScenario_comboBox->count()==0)
        {
            ui->scenarioView_editScenario_comboBox->addItems(scenarios);
        }

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
//  Help: Displays QMessageBox with instructions on how to use the database options
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
    {
        ui->save_editActors_pushButton->setText("Save Changes");
    }

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

void MainWindow::on_dbEdit_tabWidget_currentChanged(int index)
{
    if(index == EDIT_SCENARIOS)
    {
        QStringList scenarios;

        // Reset buttons
        ui->add_editScenario_pushButton->setText("Create New Scenario");
        ui->remove_editScenario_pushButton->setEnabled(false);
        ui->saveChanges_editScenario_pushButton->setEnabled(false);

        // Reset dropdown
        ui->scenarioView_editScenario_comboBox->setCurrentIndex(0);

        // Initialize and populate bottom tableWidget
        // TODO: figure out a generic name for this method since it affects more than one type of table, not just 'add actor'
        tableManager->InitializeAddActorTable(ui->scenarios_editScenario_tableWidget, tableManager->AllScenarioColCount, tableManager->AllScenarioColNames);

        // TODO if removing vertical headers can be placed somewhere else, that'd be nice
        ui->scenarios_editScenario_tableWidget->verticalHeader()->hide();

        tableManager->PopulateScenarioNameTable(ui->scenarios_editScenario_tableWidget, db->GetScenarioList());

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

        // Add scenario name to combobox list
        ui->scenarioView_editScenario_comboBox->addItem(scenarioName);

        // Select new scenario
        ui->scenarioView_editScenario_comboBox->setCurrentIndex(ui->scenarioView_editScenario_comboBox->count() -1);
    }
    else // Add actor to existing scenario
    {
        // If user has selected a valid row
        if(ui->actors_editScenario_tableView->currentIndex().row() != -1)
        {
            int row = ui->actors_editScenario_tableView->currentIndex().row(); // Row of table
            int IDtoAdd; // Actor's ID to add
            int qty = 1; // Quantity of actor type to add
            QString scenarioName; // Name of scenario in which to add actor
            bool presentOnList = false;
            int index;

            // Get actor's name from selected row
            IDtoAdd = ui->actors_editScenario_tableView->model()->index(row,0).data().toInt();


            // Get scenario name from combobox
            scenarioName = ui->scenarioView_editScenario_comboBox->currentText();

            qDebug() << "Actor ID selected: " << IDtoAdd;
            qDebug() << "Scenario selected: " << scenarioName;
            qDebug() << "Quantity selected: " << qty;

            // Check to see if actor is already on tablewidget
            index = 0;
            presentOnList = ui->actors_editScenario_tableView->model()->index(index,0).data().toInt() == ui->scenarios_editScenario_tableWidget->model()->index(index,0).data().toInt();


            while(!presentOnList && index < ui->scenarios_editScenario_tableWidget->rowCount())
            {
                index++;
                presentOnList = ui->actors_editScenario_tableView->model()->index(index,0).data().toInt() == ui->scenarios_editScenario_tableWidget->model()->index(index,0).data().toInt();
            }// End if present on list and still in row

            if(presentOnList)
            {
                // Pop up window informing user that listing is already present
                QMessageBox warnPrompt;

                warnPrompt.setIcon(QMessageBox::Warning);
                warnPrompt.setText("WARNING");
                warnPrompt.setInformativeText("Actor is already listed in scenario. Please select new actor.");
                warnPrompt.setStandardButtons(QMessageBox::Ok);
            }
            else
            {
                // Collect data from tableview
                Actor toAdd;

                toAdd.SetID(ui->actors_editScenario_tableView->model()->index(row,0).data().toInt());

                // Create new row


                // Add listing to tablewidget


            }
        }
    }
}

// *************************************************************************************
//  DB - Edit Scenario - Top Tableview Clicked
// *************************************************************************************
void MainWindow::on_actors_editScenario_tableView_clicked(const QModelIndex &index)
{
    // If "All Scenarios" selected on combobox
    if(ui->scenarioView_editScenario_comboBox->currentText() == "All Scenarios")
    {
        // Enable "Add" button
        if(!ui->add_editScenario_pushButton->isEnabled())
        { ui->add_editScenario_pushButton->setEnabled(true); }

        // Set "Add" button text
        if(ui->add_editScenario_pushButton->text() != "Create New Scenario")
        { ui->add_editScenario_pushButton->setText("Create New Scenario"); }

        // Disable "Delete" button
        if(ui->remove_editScenario_pushButton->isEnabled())
        { ui->remove_editScenario_pushButton->setEnabled(false); }

        // Set "Delete" button text
        if(ui->remove_editScenario_pushButton->text() != "Delete Scenario")
        { ui->remove_editScenario_pushButton->setText("Delete Scenario"); }
    }
    else // Specific scenario selected
    {
        // Enable "Add" button
        if(!ui->add_editScenario_pushButton->isEnabled())
        { ui->add_editScenario_pushButton->setEnabled(true); }

        // Set "Add" button text
        if(ui->add_editScenario_pushButton->text() != "Add Actor")
        { ui->add_editScenario_pushButton->setText("Add Actor"); }

        // Disable "Delete" button
        if(ui->remove_editScenario_pushButton->isEnabled())
        { ui->remove_editScenario_pushButton->setEnabled(false); }

        // Set "Delete" button text
        if(ui->remove_editScenario_pushButton->text() != "Remove Actor")
        { ui->remove_editScenario_pushButton->setText("Remove Actor"); }
    }
}

// *************************************************************************************
//  DB - Edit Scenario - Bottom TableWidget Clicked
// *************************************************************************************
void MainWindow::on_scenarios_editScenario_tableWidget_itemClicked(QTableWidgetItem *item)
{
    // If "All Scenarios" selected on combobox
    if(ui->scenarioView_editScenario_comboBox->currentText() == "All Scenarios")
    {
        // Enable "Add" button
        if(!ui->add_editScenario_pushButton->isEnabled())
        { ui->add_editScenario_pushButton->setEnabled(true); }

        // Set "Add" button text
        if(ui->add_editScenario_pushButton->text() != "Create New Scenario")
        { ui->add_editScenario_pushButton->setText("Create New Scenario"); }

        // Enable "Delete" button
        if(!ui->remove_editScenario_pushButton->isEnabled())
        { ui->remove_editScenario_pushButton->setEnabled(true); }

        // Set "Delete" button text
        if(ui->remove_editScenario_pushButton->text() != "Delete Scenario")
        { ui->remove_editScenario_pushButton->setText("Delete Scenario"); }
    }
    else // Specific scenario selected
    {
        // Disable "Add" button
        if(ui->add_editScenario_pushButton->isEnabled())
        { ui->add_editScenario_pushButton->setEnabled(false); }

        // Set "Add" button text
        if(ui->add_editScenario_pushButton->text() != "Add Actor")
        { ui->add_editScenario_pushButton->setText("Add Actor"); }

        // Enable "Delete" button
        if(!ui->remove_editScenario_pushButton->isEnabled())
        { ui->remove_editScenario_pushButton->setEnabled(true); }

        // Set "Delete" button text
        if(ui->remove_editScenario_pushButton->text() != "Remove Actor")
        { ui->remove_editScenario_pushButton->setText("Remove Actor"); }
    }
}

void MainWindow::on_saveChanges_editScenario_pushButton_clicked()
{
    // This is where we gotta pull the qtys
    int row = ui->actors_editScenario_tableView->currentIndex().row(); // Row of table
    QSpinBox* temp = qobject_cast<QSpinBox*>(ui->scenarios_editScenario_tableWidget->cellWidget(row, tableManager->S_QTY));

}
