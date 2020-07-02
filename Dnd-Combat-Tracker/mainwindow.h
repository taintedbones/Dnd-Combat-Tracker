#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include "Database.h"
#include "DbEditTableModel.h"
#include "TableModel.h"
#include "AddActorForm.h"
#include "CombatManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum  pages {WELCOME, EDIT, ASSIGN, COMBAT, DB_EDIT};

    // Add Actors Page ComboBox
    enum  addActorsComboBox { ALL, PARTY, CREATURES, COMPANIONS, EFFECTS};

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

    // *************************************************************************************
    //  DB EDIT PAGE
    // *************************************************************************************
    // Navigates user to welcome page from database editor page
    void on_mainMenu_pushButton_clicked();

    // Navigates user to combat edit page from database editor page
    void on_combatEditor_pushButton_clicked();

    // Formats db edit table view
    void format_dbEdit_tableView();
    void on_activeCombatTable_tableWidget_itemSelectionChanged();

    void on_endTurn_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Database *db;
    DbEditTableModel *model;
    TableModel *tableManager;

    // TableWidgets
    QTableWidget *combatTable;
    QTableWidget *addActors;
    QTableWidget *assignInit;

    // Lists
    QVector<Actor> *combatList;
    AddActorForm *addActorForm;
    CombatManager *combatManager;
};
#endif // MAINWINDOW_H
