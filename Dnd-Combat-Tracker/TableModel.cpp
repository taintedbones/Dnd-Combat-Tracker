#include "TableModel.h"

// Model used for 'combat_page' (Conduct Combat) Page
void TableModel::InitializeCombatModel(int &CombatColCount, QStringList &CombatColNames)
{
    this->setColumnCount(CombatColCount);
    this->setVerticalHeaderLabels(CombatColNames);
}

// Model used for 'edit_page' (Add Actors) Page
void TableModel::InitializeActorListModel(int &ActorListColCount, QStringList &ActorListColNames)
{
    this->setColumnCount(ActorListColCount);
    this->setVerticalHeaderLabels(ActorListColNames);
}

// Model used for 'assignInit_page' (Assign Initiative) Page
void TableModel::InitializeInitiativeModel(int &AssignInitColCount, QStringList &AssignInitColNames)
{
    this->setColumnCount(AssignInitColCount);
    this->setVerticalHeaderLabels(AssignInitColNames);
}

// Constructor
TableModel::TableModel()
{
    QTableWidget();
}
