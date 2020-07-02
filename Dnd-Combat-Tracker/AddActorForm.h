#ifndef ADDACTORFORM_H
#define ADDACTORFORM_H

#include <QWidget>
#include <QPushButton>
#include "Database.h"
#include "Actor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AddActorForm; }
QT_END_NAMESPACE

enum Pages { MENU, CUSTOM, PREMADE };

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

     // Adds custom actor to combat
     void on_ok_addActor_pushButton_clicked();

     // Navigates user to Menu from Add Custom Actor Page
     void on_back_addActor_pushButton_clicked();

     // Initializes all forms in Add Custom Actor Page
     void InitializeAddCustom();

     // Initializes all forms in Add Premade Actor Page
     void InitializeAddPremade();

     // Navigates user to Menu from Premade Actor Page
     void on_back_premade_pushButton_clicked();

     // Adds selected actor to combat table
     void on_ok_premade_pushButton_clicked();

     // Sets all the fields on premade page to data of passed in actor
     void SetPremadeFields(Actor currentActor);

     // Auto fills the premade actor data fields for the current actor
     void on_name_premade_comboBox_currentIndexChanged(const QString &arg1);

     // Changes displayed names in premade actor combo box to match the current scenario
     void on_scenario_premade_comboBox_currentIndexChanged(const QString &arg1);

     // Reinitializes the add pages each time the user goes back to the menu
     void on_stackedWidget_currentChanged(int arg1);

private:
    Ui::AddActorForm *ui;
    Database *db;
    QVector<Actor> *actorList;
    Actor createdActor;
    QTableWidget *combat;
};

#endif // ADDACTORFORM_H
