#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Database.h"
#include "DbEditTableModel.h"
#include "TableModel.h"
#include "AddActorForm.h"

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
    //************ Welcome Page *********************
    void on_welcomeStart_pushButton_clicked();
    void on_dbOpt_welcome_pushButton_clicked();

    //************ Edit Page *********************
    void on_back_editPage_pushButton_clicked();
    void on_next_editPage_pushButton_clicked();
    void on_dbOpt_editPage_pushButton_clicked();
    void on_addActor_pushButton_clicked();
    void on_deleteActor_pushButton_clicked();
    void on_showActors_comboBox_activated(int index);

    //************ Assign Page *********************
    void on_back_assignInit_pushButton_clicked();
    void on_fight_assignInit_pushButton_clicked();
    void on_assignInit_tableWidget_itemSelectionChanged();

    //************ Combat Page *********************
    void on_endCombat_pushButton_clicked();

    //************ DB Edit Page *********************
    void on_mainMenu_pushButton_clicked();
    void on_combatEditor_pushButton_clicked();
    void format_dbEdit_tableView();

    void on_addActor_combat_pushButton_clicked();

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
};
#endif // MAINWINDOW_H
