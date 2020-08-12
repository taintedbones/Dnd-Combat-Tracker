#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QInputDialog>
#include "Database.h"
#include "DbEditTableModel.h"
#include "TableModel.h"
#include "AddActorForm.h"
#include "CombatManager.h"
#include "ScenarioListing.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Program Main Pages
    enum  pages { WELCOME, EDIT, ASSIGN, COMBAT, DB_EDIT };

    // Database Options Tabs
    enum dbOptions { EDIT_ACTORS, EDIT_SCENARIOS };

    // Add Actors Page ComboBox
    enum  addActorsComboBox { ALL, PARTY, CREATURES, COMPANIONS, EFFECTS };

    // Database Options - Actors - Combobox
    enum DbEditComboBoxPositions { DB_PARTY, DB_CREATURE, DB_COMPANION, DB_EFFECT };

    QStringList addActorsComboBoxLabels = { "All Actors", "Partymembers", "Creatures", "Companions", "Effects" };

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // *************************************************************************************
    //   WELCOME PAGE
    // *************************************************************************************
    // Navigates user to combat edit page from welcome page
    void on_welcomeStart_pushButton_clicked();

    // Navigates user to database editor page from welcome page
    void on_dbOpt_welcome_pushButton_clicked();

    //  Initializes DB edit page when user navigates to that page
    void on_main_stackedWidget_currentChanged(int arg1);

    // *************************************************************************************
    //  EDIT PAGE
    // *************************************************************************************
    // Navigates user to welcome page from combat edit page
    void on_back_editPage_pushButton_clicked();

    // Navigates user to assign initiative page from combat edit page
    void on_next_editPage_pushButton_clicked();

    // Navigates user to database editor page from combat edit page
    void on_dbOpt_editPage_pushButton_clicked();

    // 'Adds' actor to combat list by moving actor listing from actor list to combat list
    void on_addActor_pushButton_clicked();

    // 'Removes' actor from combat list by moving actor listing from combat list to actor list
    void on_deleteActor_pushButton_clicked();

    // Filters displayed actors by type
    void on_showActors_comboBox_activated(int index);

    // *************************************************************************************
    //  ASSIGN PAGE
    // *************************************************************************************
    // Navigates user to combat edit page from assign initiative page
    void on_back_assignInit_pushButton_clicked();

    // Navigates user to combat page from assign initiative page
    void on_fight_assignInit_pushButton_clicked();

    //  Focuses on initiative spinbox whenever user changes selection on table
    void on_assignInit_tableWidget_itemSelectionChanged();

    // *************************************************************************************
    //  COMBAT PAGE
    // *************************************************************************************
    // Navigates user to welcome page from combat page
    void on_endCombat_pushButton_clicked();

    // Opens & initializes add actor form whenever add actor button is clicked in combat page
    void on_addActor_combat_pushButton_clicked();

    // Updates notes text browser to display selected actor's notes
    void on_activeCombatTable_tableWidget_itemSelectionChanged();

    // Ends current player's turn by moving their row to bottom of table, updates round &
    //  player name, and checks for tie
    void on_endTurn_pushButton_clicked();

    //  Deletes the selected actor from the active combat table
    void on_deleteActor_combat_pushButton_clicked();

    //   Moves selected actor from "Add Actor" table to "Combat" Table
    void on_actorTable_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    // *************************************************************************************
    //  DB EDIT PAGE
    // *************************************************************************************
    // Navigates user to welcome page from database editor page
    void on_mainMenu_pushButton_clicked();

    // Navigates user to combat edit page from database editor page
    void on_combatEditor_pushButton_clicked();

    // Formats db edit table view
    void FormatEditActorsTableView();

    //  Formats and sets table model for scenario actors table view
    void FormatEditScenarioActorsTableView();

    //  Shows actor statistics fields, allowing user to add actor to database
    void on_addActor_dbEdit_pushButton_clicked();

    //  Saves changes (add/edit) to database
    void on_save_editActors_pushButton_clicked();

    //  Deletes actor from database
    void on_deleteActor_dbEdit_pushButton_clicked();

    //  Displays QMessageBox with instructions on how to use the database options
    void on_help_dbEdit__pushButton_clicked();

    //  Pulls data from tableview into text fields upon clicking any table entry
    void on_dbEdit_tableView_clicked();

    //  PushButton for user to clear fields on DB edit page
    void on_clear_editActors_pushButton_clicked();

    // Resets all objects on the edit scenarios tab
    void on_dbEdit_tabWidget_currentChanged(int index);

    //  PushButton "Add" for user to create scenarios or add actors to existing scenario
    void on_add_editScenario_pushButton_clicked();

    // Configures scenario ui buttons for when top actors tableview clicked
    void on_actors_editScenario_tableView_clicked(const QModelIndex &index);

    // Configures scenario ui buttons for when bottom scenarios tablewidget clicked
    void on_scenarios_editScenario_tableWidget_itemClicked(QTableWidgetItem *item);

    //  Saves changes made to edit scenarios table widget when 'save changes' button is clicked
    void on_saveChanges_editScenario_pushButton_clicked();

    //  Removes scenario or actor in scenario depending on table widget view
    void on_remove_editScenario_pushButton_clicked();

    //  Reformats scenario tableview to display scenario listing or actors for selected
    //  scenario
    void on_scenarioView_editScenario_comboBox_activated(int index);

    // Helper Functions
    void ClearDBFields();

    //  Enables save button
    void EnableSaveButton();

    // Configures the add/remove buttons text and enable status with the passed in values
    void ConfigureScenarioUIButtons (const bool &addButtonStatus, const QString &addButtonText, const bool &deleteButtonStatus, const QString &deleteButtonText);

    void on_actors_editScenario_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    Database *db;
    TableModel *tableManager;

    // TableWidgets
    QTableWidget *combatTable;
    QTableWidget *addActors;
    QTableWidget *assignInit;

    // Lists
    QVector<Actor> *combatList;
    AddActorForm *addActorForm;
    CombatManager *combatManager;

    // Models
    DbEditTableModel *editActorsModel = nullptr;
    DbEditTableModel *editScenarioModel = nullptr;
    DbEditTableModel *editScenarioActorsModel = nullptr;

    // DEBUG: Previous index checking
    QVector<ScenarioListing> *listings = nullptr;
    ScenarioListing singleListing;
    int previousIndex = 0;
    QString previousText = "";

    // Flag to determine if data has been saved or not
    bool _saved;
};
#endif // MAINWINDOW_H
