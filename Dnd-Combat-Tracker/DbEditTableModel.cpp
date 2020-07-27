#include "DbEditTableModel.h"
#include <QSqlQuery>

DbEditTableModel::DbEditTableModel(QObject *parent, Database *db) : QSqlTableModel(parent, *db)
{
    Initialize();
    database = db;
}

// *************************************************************************************
//  Initialies model to display actor table & sets headers
// *************************************************************************************
void DbEditTableModel::Initialize()
{
    this->setTable("actors");

    this->setHeaderData(1, Qt::Horizontal, tr("Actor"));
    this->setHeaderData(2, Qt::Horizontal, tr("Max HP"));
    this->setHeaderData(3, Qt::Horizontal, tr("Armor Class"));
    this->setHeaderData(4, Qt::Horizontal, tr("Spell Save DC"));
    this->setHeaderData(5, Qt::Horizontal, tr("Notes"));
    this->setHeaderData(6, Qt::Horizontal, tr("Type"));

    this->select();
}

// *************************************************************************************
//  Initializes model to display scenario_list view & sets header
// *************************************************************************************
void DbEditTableModel::InitializeScenarios()
{
    this->setTable("scenario_list");

    this->setHeaderData(0, Qt::Horizontal, tr("Scenario"));

    this->select();
}

// *************************************************************************************
//  Initializes model to display scenario_actorList view & filters table by passed in
//      scenario name
// *************************************************************************************
void DbEditTableModel::InitializeScenarioByName(QString scenarioName)
{
    this->setTable("scenario_actorList");

    this->setFilter("scenarioName =\"" + scenarioName+ "\"");

    this->setHeaderData(1, Qt::Horizontal, tr("Actor"));
    this->setHeaderData(2, Qt::Horizontal, tr("Max HP"));
    this->setHeaderData(3, Qt::Horizontal, tr("Armor Class"));
    this->setHeaderData(4, Qt::Horizontal, tr("Spell Save DC"));
    this->setHeaderData(5, Qt::Horizontal, tr("Notes"));
    this->setHeaderData(6, Qt::Horizontal, tr("Type"));

    this->select();
}
