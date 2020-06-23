#include "TableModel.h"
#include "Actor.h"
#include <QtDebug>

// Model used for 'combat_page' (Conduct Combat) Page
void TableModel::InitializeCombatModel(QTableWidget *combatTable)
{
    combatTable->clearContents();
    combatTable->setColumnCount(CombatColCount);
    combatTable->setHorizontalHeaderLabels(CombatColNames);
    combatTable->setEditTriggers(QTableView::NoEditTriggers);

    DeleteAllTableRows(combatTable);
}

// Model used for 'edit_page' (Add Actors) Page
void TableModel::InitializeAddActorTable(QTableWidget *addActors, int cols, QStringList headers)
{
    addActors->clearContents();
    addActors->setColumnCount(cols);
    addActors->setHorizontalHeaderLabels(headers);
//    addActors->hideColumn(A_TYPE);
    addActors->setColumnWidth(A_NAME, 150);
    addActors->setEditTriggers(QTableView::NoEditTriggers);

    DeleteAllTableRows(addActors);
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

//    QTableWidgetItem *qtyItem;
//    QSpinBox *qtyBox;


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
    } // END for
}

// Model used for 'assignInit_page' (Assign Initiative) Page
void TableModel::InitializeInitiativeModel(QTableWidget *assignInit)
{
    assignInit->clearContents();
    assignInit->setColumnCount(AssignInitColCount);
    assignInit->setHorizontalHeaderLabels(AssignInitColNames);

    assignInit->setEditTriggers(QTableView::NoEditTriggers);
}

// Remove actor from one table and back to its origin
void TableModel::RemoveActorFromTable(QTableWidget* origin, QTableWidget* destination)
{
    bool empty = origin->rowCount() == 0;

    if(!empty)
    {
        // Get selected row index
        int selectedRow = origin->currentItem()->row();
        int placedRow = destination->rowCount();

        // Create new row
        destination->insertRow(destination->rowCount());

        // Load actor into other table
        for(int col = 0; col < SelectedListColCount; col++)
        {
            // Insert attributes
            destination->setItem(placedRow, col, origin->takeItem(selectedRow, col));
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
void TableModel::InsertSpinBoxCol(QTableWidget *table, int min, int max, int col)
{
    QSpinBox *sBox;

    for(int row = 0; row < table->rowCount(); row++)
    {
        sBox = new QSpinBox(table);
        sBox->setRange(min, max);
        table->setCellWidget(row, col, sBox);
    }
}

// Deletes entire contents of passed in table
void TableModel::DeleteAllTableRows(QTableWidget *table)
{
    const int ROW_COUNT = table->rowCount();

    for(int index = 0; index < ROW_COUNT; index++)
    {
        table->removeRow(0);
    }
}

// Adds actor to new table from origin with quantity spin boxes
void TableModel::AddActorToTable(QTableWidget *origin, QTableWidget *destination)
{
    QSpinBox *qtyBox = new QSpinBox(destination);
    QTableWidgetItem *qtyItem = new QTableWidgetItem();
    bool empty  = origin->rowCount() == 0;
    bool inParty = false;

    if(!empty)
    {
        // Get selected row index
        int selectedRow = origin->currentItem()->row();
        int placedRow = destination->rowCount();

        // Create new row
        destination->insertRow(destination->rowCount());

        // Load actor into other table
        for(int col = 0; col < ActorListColCount; col++)
        {
            // Insert attributes
            destination->setItem(placedRow, col, origin->takeItem(selectedRow, col));
        }

        inParty = destination->item(placedRow, S_TYPE)->text() == "partymember";

        // Checks if actor is a party member; if true, sets quantity to only 1
        if(inParty)
        {
            qtyBox->setRange(1, 1);
        }
        else
        {
             qtyBox->setRange(QtyMin, QtyMax);
        }

        // Stores quantity spin box into table
        destination->setItem(placedRow, S_QTY, qtyItem);
        destination->setCellWidget(placedRow, S_QTY, qtyBox);

        // Delete old table's listing
        origin->removeRow(selectedRow);
    } // END if(!empty)
}

// Constructor
TableModel::TableModel() {}
