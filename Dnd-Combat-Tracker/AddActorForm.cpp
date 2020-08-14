#include "AddActorForm.h"
#include "ui_addactorform.h"
#include "CombatManager.h"
#include <QMessageBox>

AddActorForm::AddActorForm(QWidget *parent, Database *data, QTableWidget *table)
    : QWidget(parent), ui(new Ui::AddActorForm)
{
    ui->setupUi(this);
    setWindowModality(Qt::WindowModality::WindowModal);

    db = data;
    combat = table;
    initCancelButtonIndex = MENU;

    ui->stackedWidget->setCurrentIndex(MENU);

    Initialize();
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

    // Set cancel button on assignInit page to go back to Add Custom Actor page
    initCancelButtonIndex = CUSTOM;

    ResetAddCustomFields();
}

// *************************************************************************************
// Navigates user to Premade Actor Page from Menu
// *************************************************************************************
void AddActorForm::on_addPremadeActor_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(PREMADE);

    // Set cancel button on assignInit page to go back to Add Premade Actor page
    initCancelButtonIndex = PREMADE;

    ResetAddPremadeFields();
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
    ui->name_custom_lineEdit->setText(ui->name_custom_lineEdit->text().trimmed());

    qDebug() << ui->name_custom_lineEdit->text();

    bool nameFieldEmpty = ui->name_custom_lineEdit->text() == "";

    if(nameFieldEmpty)
    {
        QMessageBox::warning(this, "WARNING", "Cannot continue without entering name field", QMessageBox::Ok);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(INIT);
        DeleteInitRows();
        SubmitCustomActor();
    }
}

// *************************************************************************************
// Navigates user to Menu from Add Custom Actor Page
// *************************************************************************************
void AddActorForm::on_back_addActor_pushButton_clicked()
{
    int result = -1;

    bool nameFieldEmpty = ui->name_custom_lineEdit->text() == "";

    // Output warning if name field has been modified
    if(!nameFieldEmpty)
    {
        result = QMessageBox::warning(this, "WARNING", "Leaving the page results in loss of data. Do you want to leave?", QMessageBox::Ok | QMessageBox::Cancel);
    }

    // Go to menu page if name field empty or user clicks 'ok on warning message
   if(nameFieldEmpty || result == QMessageBox::Ok)
   {
        ui->stackedWidget->setCurrentIndex(MENU);
        qDebug() << ui->name_custom_lineEdit->text();
   }
}

// *************************************************************************************
// Initializes all forms in Add Custom Actor Page
// *************************************************************************************
void AddActorForm::ResetAddCustomFields()
{
    ui->name_custom_lineEdit->clear();
    ui->maxHP_custom_spinBox->setValue(1);
    ui->ac_custom_spinBox->setValue(1);
    ui->dc_custom_spinBox->setValue(1);
    ui->notes_custom_textEdit->clear();
    ui->actorType_custom_comboBox->setCurrentIndex(0);
    ui->qty_custom_spinBox->setValue(1);
    ui->qty_custom_spinBox->setEnabled(false);
}

// *************************************************************************************
// Initializes all forms in Add Premade Actor Page
// *************************************************************************************
void AddActorForm::ResetAddPremadeFields()
{
    QString currentScenario;

    // Clear dropdowns
    ui->qty_premade_spinBox->setValue(1);

    UpdatePremadeActors();
}

// *************************************************************************************
// Adds selected actor in specified quantity to setInit table
// *************************************************************************************
void AddActorForm::on_ok_premade_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(INIT);
    DeleteInitRows();
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
void AddActorForm::UpdatePremadeActors()
{
    CombatManager manager = CombatManager(combat);
    bool inCombat = false;
    QString currentActor;

    QVector<Actor>* actorList = db->GetActorList();

    ui->name_premade_comboBox->clear();

    for(int i = 0; i < actorList->size(); i++)
    {
        currentActor = actorList->at(i).GetName();

        inCombat = manager.IsActorInCombat(currentActor);

        qDebug() << currentActor;
        qDebug() << "in combat: " << inCombat;

        if(!inCombat)
        {
                ui->name_premade_comboBox->addItem(actorList->at(i).GetName());
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

//    ResetAddCustomFields();
    ui->maxHP_custom_spinBox->setRange(1, 99);
    ui->ac_custom_spinBox->setRange(1, 99);
    ui->dc_custom_spinBox->setRange(1, 99);
    ui->qty_custom_spinBox->setRange(1, 10);

//    ResetAddPremadeFields();
    ui->qty_premade_spinBox->setRange(1, 10);
}

// *************************************************************************************
// Reinitializes the add pages each time the user goes back to the menu
// *************************************************************************************
void AddActorForm::on_stackedWidget_currentChanged(int arg1)
{
 if(arg1 == INIT)
    {
        DeleteInitRows();
    }
}

// *************************************************************************************
// Adds the actors from setInit to the combat and closes the modal window
// *************************************************************************************
void AddActorForm::on_addToCombat_pushButton_clicked()
{
    AddToCombat();

    close();
}

// *************************************************************************************
// Navigates user from setInit page to whichever Add Actor page they were previously on
// *************************************************************************************
void AddActorForm::on_cancel_setInit_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(initCancelButtonIndex);
}

// *************************************************************************************
// Inserts the passed in actor, with the passed in quantity, to the setInit table widget
// *************************************************************************************
void AddActorForm::InsertActorToSetInit(Actor actor, int qty)
{
    for(int i = 0; i < qty; i++)
    {
        ui->setInit_tableWidget->insertRow(i);
        QTableWidgetItem *name = new QTableWidgetItem("Name");

        if(i == 0)
        {
            name->setData(0, actor.GetName());
        }
        else
        {
            name->setData(0, actor.GetName() +' '+ QString::number(i + 1));
        }

        // Save actor data into widget items for inserting into table widget
        QTableWidgetItem *hp = new QTableWidgetItem(QString::number(actor.GetHitPoints()));
        QTableWidgetItem *ac = new QTableWidgetItem(QString::number(actor.GetArmorClass()));
        QTableWidgetItem *dc = new QTableWidgetItem(QString::number(actor.GetSpellSaveDC()));
        QTableWidgetItem *notes = new QTableWidgetItem(actor.GetNotes());

        ui->setInit_tableWidget->setItem(i, NAME, name);
        ui->setInit_tableWidget->setItem(i, HP, hp);
        ui->setInit_tableWidget->setItem(i, AC, ac);
        ui->setInit_tableWidget->setItem(i, DC, dc);
        ui->setInit_tableWidget->setItem(i, NOTES, notes);

        // Set initiative spinBox into table with valid range
        QSpinBox *initBox = new QSpinBox();
        initBox->setRange(1, 30);
        ui->setInit_tableWidget->setCellWidget(i, INITIATIVE, initBox);
    }
}

// *************************************************************************************
// Initializes formatting for setInit table widget
// *************************************************************************************
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

// *************************************************************************************
//  Deletes all rows in setInit table widget
// *************************************************************************************
void AddActorForm::DeleteInitRows()
{
    int rowCount = ui->setInit_tableWidget->rowCount();

    for(int i = 0; i < rowCount; i++)
    {
        ui->setInit_tableWidget->removeRow(0);
    }
}

// *************************************************************************************
// Enables or disables qty spinbox depending on custom actor type
// *************************************************************************************
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

// *************************************************************************************
// Inserts each actor from the setInit table into the combat table on mainwindow
// *************************************************************************************
void AddActorForm::AddToCombat()
{
    CombatManager manager = CombatManager(combat);
    Actor newActor;
    int init;
    QSpinBox *initBox;
    QTableWidget *initTable = ui->setInit_tableWidget;

    // Insert actor data from each row of setInit into combat
    for(int row = 0; row < initTable->rowCount(); row++)
    {
        newActor.SetName(initTable->item(row, NAME)->text());
        newActor.SetHitPoints(initTable->item(row, HP)->text().toInt());
        newActor.SetArmorClass(initTable->item(row, AC)->text().toInt());
        newActor.SetSpellSaveDC(initTable->item(row, DC)->text().toInt());
        newActor.SetNotes(initTable->item(row, NOTES)->text());

        initBox = qobject_cast<QSpinBox*>(initTable->cellWidget(row, INITIATIVE));
        init = initBox->value();

        manager.InsertActorToCombat(newActor, init);
    }
}

void AddActorForm::on_name_custom_lineEdit_textChanged(const QString &arg1)
{
    bool inDB = db->IsInDatabase(arg1);

    if(inDB)
    {
        QMessageBox::warning(this, "ERROR", "ACTOR ALREADY IN DATABASE\nPlease enter a different name", QMessageBox::Ok);

        if(ui->ok_addActor_pushButton->isEnabled())
        {
            ui->ok_addActor_pushButton->setEnabled(false);
        }
    }
    else
    {
        if(!ui->ok_addActor_pushButton->isEnabled())
        {
            ui->ok_addActor_pushButton->setEnabled(true);
        }
    }
}
