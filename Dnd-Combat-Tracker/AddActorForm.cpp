#include "AddActorForm.h"
#include "ui_addactorform.h"
#include "CombatManager.h"

AddActorForm::AddActorForm(QWidget *parent, Database *data, QTableWidget *table)
    : QWidget(parent), ui(new Ui::AddActorForm)
{
    ui->setupUi(this);
    setWindowModality(Qt::WindowModality::WindowModal);

    db = data;
    combat = table;

    ui->stackedWidget->setCurrentIndex(MENU);

    InitializeInitTable();
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
    ui->stackedWidget->setCurrentIndex(INIT);
    SubmitCustomActor();
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
    ui->name_custom_label->clear();
    ui->maxHP_custom_spinBox->setValue(0);
    ui->ac_custom_spinBox->setValue(0);
    ui->dc_custom_spinBox->setValue(0);
    ui->notes_custom_textEdit->clear();
    ui->qty_custom_spinBox->setValue(1);
    ui->qty_custom_spinBox->setEnabled(false);
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
    ui->qty_premade_spinBox->setValue(1);

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
    ui->stackedWidget->setCurrentIndex(INIT);
    SubmitPremadeActor();
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

    if(displayedActor.GetType() == "partymember" || displayedActor.GetType() == "companion")
    {
        ui->qty_premade_spinBox->setEnabled(false);
    }
    else
    {
        ui->qty_premade_spinBox->setEnabled(true);
    }
}

// *************************************************************************************
// Changes displayed names in premade actor combo box to match the current scenario
// *************************************************************************************
void AddActorForm::on_scenario_premade_comboBox_currentIndexChanged(const QString &arg1)
{
    QString scenario;
    CombatManager manager = CombatManager(combat);
    int id;
    bool matchingScenario = false;
    bool inCombat = false;

    ui->name_premade_comboBox->clear();

    for(int i = 0; i < actorList->size(); i++)
    {
        id = actorList->at(i).GetID();
        scenario = db->GetScenarioByID(id);

        matchingScenario = scenario == arg1;
        inCombat = manager.IsActorInCombat(actorList->at(i).GetName());

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
    Actor premade;

    // Store data from form fields into actor object
    premade.SetName(ui->name_premade_comboBox->currentText());
    premade.SetHitPoints(ui->maxHP_premade_lineEdit->text().toInt());
    premade.SetArmorClass(ui->ac_premade_lineEdit->text().toInt());
    premade.SetSpellSaveDC(ui->dc_premade_lineEdit->text().toInt());
    premade.SetNotes(ui->notes_textBrowser->toPlainText());

    InsertActorToSetInit(premade, ui->qty_premade_spinBox->value());
}

// *************************************************************************************
// Stores data from add custom fields in an actor & inserts actor to combat table
// *************************************************************************************
void AddActorForm::SubmitCustomActor()
{
    Actor custom;

    // Store data from form fields into actor object
    custom.SetName(ui->name_custom_lineEdit->text());
    custom.SetHitPoints(ui->maxHP_custom_spinBox->value());
    custom.SetArmorClass(ui->ac_custom_spinBox->value());
    custom.SetSpellSaveDC(ui->dc_custom_spinBox->value());
    custom.SetNotes(ui->notes_custom_textEdit->toPlainText());

    InsertActorToSetInit(custom, ui->qty_custom_spinBox->value());
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
        DeleteInitRows();
    }
}

void AddActorForm::on_addToCombat_pushButton_clicked()
{

}

void AddActorForm::on_cancel_setInit_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(MENU);
}

void AddActorForm::InsertActorToSetInit(Actor actor, int qty)
{
    for(int i = 0; i < qty; i++)
    {
        ui->setInit_tableWidget->insertRow(ui->setInit_tableWidget->rowCount());
        QTableWidgetItem *name = new QTableWidgetItem("Name");

        if(i == 0)
        {
            name->setData(0, actor.GetName());
        }
        else
        {
            name->setData(0, actor.GetName() +' '+ QString::number(i + 1));
        }

        QTableWidgetItem *hp = new QTableWidgetItem(QString::number(actor.GetHitPoints()));
        QTableWidgetItem *ac = new QTableWidgetItem(QString::number(actor.GetArmorClass()));
        QTableWidgetItem *dc = new QTableWidgetItem(QString::number(actor.GetSpellSaveDC()));
        QTableWidgetItem *notes = new QTableWidgetItem(actor.GetNotes());

        ui->setInit_tableWidget->setItem(i, NAME, name);
        ui->setInit_tableWidget->setItem(i, HP, hp);
        ui->setInit_tableWidget->setItem(i, AC, ac);
        ui->setInit_tableWidget->setItem(i, DC, dc);
        ui->setInit_tableWidget->setItem(i, NOTES, notes);

        QSpinBox *initBox = new QSpinBox();
        initBox->setRange(1, 30);

        ui->setInit_tableWidget->setCellWidget(i, INITIATIVE, initBox);
    }
}

void AddActorForm::InitializeInitTable()
{
    QStringList headers = {"Name", "HP", "AC", "DC", "Initiative", "Notes"};

    ui->setInit_tableWidget->setColumnCount(headers.size());
    ui->setInit_tableWidget->setHorizontalHeaderLabels(headers);
    ui->setInit_tableWidget->setColumnWidth(0, 150);
    ui->setInit_tableWidget->setEditTriggers(QTableView::NoEditTriggers);

    ui->setInit_tableWidget->setColumnHidden(HP, true);
    ui->setInit_tableWidget->setColumnHidden(AC, true);
    ui->setInit_tableWidget->setColumnHidden(DC, true);
    ui->setInit_tableWidget->setColumnHidden(NOTES, true);

    DeleteInitRows();
}

void AddActorForm::DeleteInitRows()
{
    for(int i = 0; i < ui->setInit_tableWidget->rowCount(); i++)
    {
        ui->setInit_tableWidget->removeRow(0);
    }
}

void AddActorForm::on_actorType_custom_comboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "partymember" || arg1 == "companion")
    {
        ui->qty_custom_spinBox->setEnabled(false);
    }
    else
    {
        ui->qty_custom_spinBox->setEnabled(true);
    }
}
