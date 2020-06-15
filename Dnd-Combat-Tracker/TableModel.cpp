#include "TableModel.h"
#include "Actor.h"
#include <QtDebug>

// Model used for 'combat_page' (Conduct Combat) Page
void TableModel::InitializeCombatModel(QTableWidget *combatTable)
{
    combatTable->clearContents();
    combatTable->setColumnCount(CombatColCount);
    combatTable->setHorizontalHeaderLabels(CombatColNames);
}

// Model used for 'edit_page' (Add Actors) Page
void TableModel::InitializeAddActorTable(QTableWidget *addActors)
{
    addActors->clearContents();
    addActors->setColumnCount(ActorListColCount);
    addActors->setHorizontalHeaderLabels(ActorListColNames);
}

void TableModel::PopulateAddActorTable(QTableWidget *addActors, QVector<Actor>* actorList)
{
    // Prep hp list
    QVector<QTableWidgetItem*> hpItemList;
    QTableWidgetItem* hpItem;

    // Prep ac list
    QVector<QTableWidgetItem*> acItemList;
    QTableWidgetItem* acItem;

    // Prep dc list
    QVector<QTableWidgetItem*> dcItemList;
    QTableWidgetItem* dcItem;


    for(int index = 0; index < actorList->length(); index++)
    {
        // Create new row
        addActors->insertRow(index);
        // Insert attributes
            // Name
        addActors->setItem(index, A_NAME, new QTableWidgetItem(actorList->at(index).GetName()));
            // HP
        hpItem = new QTableWidgetItem;
        hpItem->setData(0,actorList->at(index).GetHitPoints());
        hpItemList.push_back(hpItem);
        addActors->setItem(index, A_HP, hpItemList.at(index));
            // AC
        acItem = new QTableWidgetItem;
        acItem->setData(0,actorList->at(index).GetArmorClass());
        acItemList.push_back(acItem);
        addActors->setItem(index, A_AC, acItemList.at(index));
            // DC
        dcItem = new QTableWidgetItem;
        dcItem->setData(0,actorList->at(index).GetSpellSaveDC());
        dcItemList.push_back(dcItem);
        addActors->setItem(index, A_DC, dcItemList.at(index));
            // Notes
        addActors->setItem(index, A_NOTES, new QTableWidgetItem(actorList->at(index).GetNotes()));
    }
}

// Model used for 'assignInit_page' (Assign Initiative) Page
void TableModel::InitializeInitiativeModel(QTableWidget *assignInit)
{
    assignInit->clearContents();
    assignInit->setColumnCount(AssignInitColCount);
    assignInit->setHorizontalHeaderLabels(AssignInitColNames);
}

// Constructor
TableModel::TableModel() {}
