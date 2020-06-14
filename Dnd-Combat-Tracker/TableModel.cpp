#include "TableModel.h"

// Model used for 'combat_page' (Conduct Combat) Page
void TableModel::InitializeCombatModel(QTableWidget *combatTable)
{
    combatTable->clearContents();
    combatTable->setColumnCount(CombatColCount);
    combatTable->setVerticalHeaderLabels(CombatColNames);
}

// Model used for 'edit_page' (Add Actors) Page
void TableModel::InitializeActorListModel(QTableWidget *addActors)
{
    addActors->clearContents();
    addActors->setColumnCount(ActorListColCount);
    addActors->setVerticalHeaderLabels(ActorListColNames);
}

// Model used for 'assignInit_page' (Assign Initiative) Page
void TableModel::InitializeInitiativeModel(QTableWidget *assignInit)
{
    assignInit->clearContents();
    assignInit->setColumnCount(AssignInitColCount);
    assignInit->setVerticalHeaderLabels(AssignInitColNames);
}

// Constructor
TableModel::TableModel() {}
