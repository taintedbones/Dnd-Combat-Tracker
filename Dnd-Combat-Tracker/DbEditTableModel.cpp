#include "DbEditTableModel.h"

DbEditTableModel::DbEditTableModel(QObject *parent, Database *db) : QSqlTableModel(parent, *db)
{
    Initialize();
}

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

void DbEditTableModel::InitializeScenarios(QString combatName)
{
    this->setTable("scenarios");

    this->select();
}
