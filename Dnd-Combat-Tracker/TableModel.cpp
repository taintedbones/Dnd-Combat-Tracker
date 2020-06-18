#include "TableModel.h"
#include "Actor.h"
#include <QtDebug>

// Model used for 'combat_page' (Conduct Combat) Page
void TableModel::InitializeCombatModel(QTableWidget *combatTable)
{
    const int ROW_COUNT = combatTable->rowCount();

    combatTable->clearContents();
    combatTable->setColumnCount(CombatColCount);
    combatTable->setHorizontalHeaderLabels(CombatColNames);

    for(int index = 0; index < ROW_COUNT; index++)
    {
        combatTable->removeRow(0);
    }
}

// Model used for 'edit_page' (Add Actors) Page
void TableModel::InitializeAddActorTable(QTableWidget *addActors)
{
    const int ROW_COUNT = addActors->rowCount();

    addActors->clearContents();
    addActors->setColumnCount(ActorListColCount);
    addActors->setHorizontalHeaderLabels(ActorListColNames);

    for(int index = 0; index < ROW_COUNT; index++)
    {
        addActors->removeRow(0);
    }
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

// Move actor from one table to the other
void TableModel::MoveActorToTable(QTableWidget* origin, QTableWidget* destination)
{
    QVector<QTableWidgetItem*> actorListing;
    QTableWidgetItem* actorAttribute = nullptr;
    QTableWidgetItem* tempListing = nullptr;
    bool empty = origin->rowCount() == 0;

    if(!empty)
    {
        // Get selected row index
        int selectedRow = origin->currentItem()->row();

        // Load the data into the attribute, then load it into the listing
        for(int index = 0; index < ActorListColCount; index++)
        {
            // Populate temporary item
            tempListing = origin->item(selectedRow, index);

            // Create new item
            actorAttribute = new QTableWidgetItem;

            // Assign temp listing value to new item
            actorAttribute->setData(0,tempListing->text());

            // Add item to listing
            actorListing.append(actorAttribute);
        }

        // Create new row
        destination->insertRow(destination->rowCount());

        // Load actor into other table
        for(int index = 0; index < ActorListColCount; index++)
        {
            // Insert attributes
            destination->setItem(destination->rowCount() - 1, index, new QTableWidgetItem(actorListing.at(index)->data(0).toString()));
        }

        // Delete old table's listing
        origin->removeRow(selectedRow);
    } // END if(!empty)
}

// Constructor
TableModel::TableModel() {}
