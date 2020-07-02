#include "AddActorForm.h"
#include "ui_addactorform.h"
#include "TableModel.h"

AddActorForm::AddActorForm(QWidget *parent, Database *data, QTableWidget *table)
    : QWidget(parent), ui(new Ui::AddActorForm)
{
    ui->setupUi(this);
    setWindowModality(Qt::WindowModality::WindowModal);

    db = data;
    combat = table;

    ui->stackedWidget->setCurrentIndex(MENU);
}

AddActorForm::~AddActorForm()
{
    delete ui;
}

// *************************************************************************************
// Navigates user to Add Custom Actor page from Menu
// *************************************************************************************
void AddActorForm::on_addCustomActor_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(CUSTOM);
}

// *************************************************************************************
// Navigates user to Premade Actor Page from Menu
// *************************************************************************************
void AddActorForm::on_addPremadeActor_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(PREMADE);

    InitializeAddPremade();
}

// *************************************************************************************
// Closes modal window
// *************************************************************************************
void AddActorForm::on_cancelAddActorMenu_pushButton_clicked()
{
    close();
}

// *************************************************************************************
// Adds custom actor to combat
// *************************************************************************************
void AddActorForm::on_ok_addActor_pushButton_clicked()
{
    SubmitCustomActor();
    close();
}

// *************************************************************************************
// Navigates user to Menu from Add Custom Actor Page
// *************************************************************************************
void AddActorForm::on_back_addActor_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(MENU);
}

// *************************************************************************************
// Initializes all forms in Add Custom Actor Page
// *************************************************************************************
void AddActorForm::InitializeAddCustom()
{   
    QStringList scenarioList = db->GetScenarioList();

    // Clear dropdowns
    ui->selectScenario_comboBox->clear();

    ui->selectScenario_comboBox->addItems(scenarioList);
}

// *************************************************************************************
// Initializes all forms in Add Premade Actor Page
// *************************************************************************************
void AddActorForm::InitializeAddPremade()
{
    QString currentScenario;

    // Clear dropdowns
    ui->name_premade_comboBox->clear();
    ui->scenario_premade_comboBox->clear();

    // Create actor list
    actorList = db->GetActorList();

    // initialize comboboxes with scenarios & actors
    db->CreateScenarioList();
    ui->scenario_premade_comboBox->addItem("All Scenarios");
    ui->scenario_premade_comboBox->addItems(db->GetScenarioList());
}

// *************************************************************************************
// Adds selected actor to combat table
// *************************************************************************************
void AddActorForm::on_ok_premade_pushButton_clicked()
{
    SubmitPremadeActor();
    close();
}

// *************************************************************************************
// Navigates user to Menu from Premade Actor Page
// *************************************************************************************
void AddActorForm::on_back_premade_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(MENU);
}

// *************************************************************************************
// Sets all the fields on premade page to data of passed in actor
// *************************************************************************************
void AddActorForm::SetPremadeFields(Actor currentActor)
{
    ui->ac_premade_lineEdit->setText(QString::number(currentActor.GetArmorClass()));
    ui->dc_premade_lineEdit->setText(QString::number(currentActor.GetSpellSaveDC()));
    ui->maxHP_premade_lineEdit->setText(QString::number(currentActor.GetHitPoints()));
    ui->notes_textBrowser->setText(currentActor.GetNotes());
    ui->qty_premade_spinBox->setRange(1, 20);
}

// *************************************************************************************
// Auto fills the premade actor data fields for the current actor
// *************************************************************************************
void AddActorForm::on_name_premade_comboBox_currentIndexChanged(const QString &arg1)
{
    Actor displayedActor = db->GetActor(arg1);

    SetPremadeFields(displayedActor);
}

// *************************************************************************************
// Changes displayed names in premade actor combo box to match the current scenario
// *************************************************************************************
void AddActorForm::on_scenario_premade_comboBox_currentIndexChanged(const QString &arg1)
{
    QString scenario;
    TableModel manager;
    int id;
    bool matchingScenario = false;
    bool inCombat = false;

    ui->name_premade_comboBox->clear();

    for(int i = 0; i < actorList->size(); i++)
    {
        id = actorList->at(i).GetID();
        scenario = db->GetScenarioByID(id);

        matchingScenario = scenario == arg1;
        inCombat = manager.IsActorInCombat(actorList->at(i).GetName(), combat);

        if(!inCombat)
        {
            // Adds actor names into combobox depending on scenario type
            if(matchingScenario)
            {
                ui->name_premade_comboBox->addItem(actorList->at(i).GetName());
            }
            else if(arg1 == "All Scenarios")
            {
                ui->name_premade_comboBox->addItem(actorList->at(i).GetName());
            }
        }
    }
}

// *************************************************************************************
// Inserts actor to combat table
// *************************************************************************************
void AddActorForm::SubmitPremadeActor()
{
    TableModel manager;
    Actor premade;
    int init;

    // Store data from form fields into actor object
    premade.SetName(ui->name_premade_comboBox->currentText());
    premade.SetHitPoints(ui->maxHP_premade_lineEdit->text().toInt());
    premade.SetArmorClass(ui->ac_premade_lineEdit->text().toInt());
    premade.SetSpellSaveDC(ui->dc_premade_lineEdit->text().toInt());
    premade.SetNotes(ui->notes_textBrowser->toPlainText());

    init = ui->init_premade_spinBox->value();

    manager.InsertActorToCombat(combat, premade, init);
}

// *************************************************************************************
// Stores data from add custom fields in an actor & inserts actor to combat table
// *************************************************************************************
void AddActorForm::SubmitCustomActor()
{
    TableModel manager;
    Actor custom;
    int init;

    // Store data from form fields into actor object
    custom.SetName(ui->name_lineEdit->text());
    custom.SetHitPoints(ui->maxHP_spinBox->value());
    custom.SetArmorClass(ui->ac_spinBox->value());
    custom.SetSpellSaveDC(ui->dc_spinBox->value());
    custom.SetNotes(ui->notes_textEdit->toPlainText());

    init = ui->init_spinBox->value();

    manager.InsertActorToCombat(combat, custom, init);
}

// *************************************************************************************
// Initializes modal window pages & sets current index to the menu
// *************************************************************************************
void AddActorForm::Initialize()
{
    ui->stackedWidget->setCurrentIndex(MENU);
    InitializeAddCustom();
    InitializeAddPremade();
}

// *************************************************************************************
// Reinitializes the add pages each time the user goes back to the menu
// *************************************************************************************
void AddActorForm::on_stackedWidget_currentChanged(int arg1)
{
    if(arg1 == MENU)
    {
        InitializeAddCustom();
        InitializeAddPremade();
    }
}
