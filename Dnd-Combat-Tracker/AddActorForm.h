#ifndef ADDACTORFORM_H
#define ADDACTORFORM_H

#include <QWidget>
#include <QPushButton>
#include "Database.h"
#include "Actor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AddActorForm; }
QT_END_NAMESPACE

class AddActorForm : public QWidget
{
    Q_OBJECT
public:
     AddActorForm(QWidget *parent = nullptr);
     AddActorForm(QWidget *parent = nullptr, Database *data = nullptr, QTableWidget *table = nullptr);
    ~AddActorForm();

     // Inserts actor to combat table
     void SubmitPremadeActor();

     // Stores data from add custom fields in an actor & inserts actor to combat table
     void SubmitCustomActor();

     // Initializes modal window pages & sets current index to the menu
     void Initialize();

private slots:
     // Navigates user to Add Custom Actor page from Menu
     void on_addCustomActor_pushButton_clicked();

     // Navigates user to Premade Actor Page from Menu
     void on_addPremadeActor_pushButton_clicked();

     // Closes modal window
     void on_cancelAddActorMenu_pushButton_clicked();

     // Adds custom actor in specified quantity to setInit table
     void on_ok_addActor_pushButton_clicked();

     // Navigates user to Menu from Add Custom Actor Page
     void on_back_addActor_pushButton_clicked();

     // Initializes all forms in Add Custom Actor Page
     void ResetAddCustomFields();

     // Initializes all forms in Add Premade Actor Page
     void ResetAddPremadeFields();

     // Navigates user to Menu from Premade Actor Page
     void on_back_premade_pushButton_clicked();

     // Adds selected actor in specified quantity to setInit table
     void on_ok_premade_pushButton_clicked();

     // Sets all the fields on premade page to data of passed in actor
     void SetPremadeFields(Actor currentActor);

     // Auto fills the premade actor data fields for the current actor
     void on_name_premade_comboBox_currentIndexChanged(const QString &arg1);

     // Reinitializes the add pages each time the user goes back to the menu
     void on_stackedWidget_currentChanged(int arg1);

     // Adds the actors from setInit to the combat and closes the modal window
     void on_addToCombat_pushButton_clicked();

     // Navigates user from setInit page to whichever Add Actor page they were previously on
     void on_cancel_setInit_pushButton_clicked();

     // Inserts the passed in actor, with the passed in quantity, to the setInit table widget
     void InsertActorToSetInit(Actor actor, int qty);

     // Initializes formatting for setInit table widget
     void InitializeInitTable();

     //  Deletes all rows in setInit table widget
     void DeleteInitRows();

     // Enables or disables qty spinbox depending on custom actor type
     void on_actorType_custom_comboBox_currentIndexChanged(const QString &arg1);

     // Inserts each actor from the setInit table into the combat table on mainwindow
     void AddToCombat();

     void on_name_custom_lineEdit_textChanged(const QString &arg1);

     void UpdatePremadeActors();

private:
    Ui::AddActorForm *ui;
    Database *db;
//    QVector<Actor>* actorList;
    Actor createdActor;
    QTableWidget *combat;
    int initCancelButtonIndex;

    enum Pages { MENU, CUSTOM, PREMADE, INIT };
    enum Cols { NAME, HP, AC, DC, INITIATIVE, NOTES };
};
#endif // ADDACTORFORM_H
