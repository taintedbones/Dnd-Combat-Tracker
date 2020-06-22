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
    addActors->hideColumn(A_TYPE);

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
            // Type
        addActors->setItem(index, A_TYPE, new QTableWidgetItem(actorList->at(index).GetType()));
    }
}

// Model used for 'assignInit_page' (Assign Initiative) Page
void TableModel::InitializeInitiativeModel(QTableWidget *assignInit)
{
    assignInit->clearContents();
    assignInit->setColumnCount(AssignInitColCount);
    assignInit->setHorizontalHeaderLabels(AssignInitColNames);

    assignInit->setEditTriggers(QTableView::NoEditTriggers);/*
    assignInit->setSelectionMode(QAbstractItemView::NoSelection);*/
}

// Move actor from one table to the other
void TableModel::MoveActorToTable(QTableWidget* origin, QTableWidget* destination)
{
    QVector<QTableWidgetItem*> actorListing;
    QTableWidgetItem* actorAttribute = nullptr;
    QTableWidgetItem* tempListing = nullptr;

    // Load the data into the attribute, then load it into the listing
    for(int index = 0; index < ActorListColCount; index++)
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

// Add Actors - Show selected actor type in actor list
void TableModel::ShowActorType(QTableWidget* addActors, const QString &type)
{
    bool match = false;

    // Show all rows (initialize)
    for(int index = 0; index < addActors->rowCount(); index++)
    {
        addActors->showRow(index);
    }

    // Hide actors not found
    for(int index = 0; index < addActors->rowCount(); index++)
    {
        // Set match to false
        match = false;

        // Check to see if listing matches type
        match = addActors->item(index, 5)->data(0).toString() == type;

        // If not, hide it
        if(!match) { addActors->hideRow(index); }
    }
}

// Copies entire contents of one tablewidget to another
void TableModel::CopyTable(QTableWidget *origin, QTableWidget *destination, bool hasInit)
{
    int totalRows = origin->rowCount();
    int totalCols  = origin->columnCount();

    QTableWidgetItem *initCell;
    QSpinBox *initBox;

    // Initiate destination table with same row & col count as origin table
    destination->setRowCount(totalRows);
    destination->setColumnCount(totalCols);

    // Move through each row & copy contents of each col to destination table
    for(int row = 0; row < totalRows; row++)
    {
        for(int col = 0; col < totalCols; col++)
        {
            destination->setItem(row, col, origin->takeItem(row, col));

             // assign init page - Copies value in spinbox to next table
            if(hasInit && col == I_INIT)
            {
                initBox = qobject_cast<QSpinBox*>(origin->cellWidget(row, I_INIT));
                initCell = new QTableWidgetItem(initBox->cleanText());

                destination->setItem(row, I_INIT, initCell);
            } // END -if
        } // END - for (col)
    } // END - for (row)
}

// Inserts initiative column to the passed in table
void TableModel::InsertInitCol(QTableWidget *table)
{
    QSpinBox *initBox;

    for(int row = 0; row < table->rowCount(); row++)
    {
        initBox = new QSpinBox(table);
        initBox->setRange(0, 20);
        table->setCellWidget(row, I_INIT, initBox);
    }
}

// Constructor
TableModel::TableModel() {}
