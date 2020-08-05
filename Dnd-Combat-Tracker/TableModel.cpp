#include "TableModel.h"
#include "Actor.h"
#include "CombatManager.h"
#include <QtDebug>

// Model used for 'combat_page' (Conduct Combat) Page
void TableModel::InitializeCombatModel(QTableWidget *combatTable)
{
    combatTable->setColumnCount(CombatColCount);
    combatTable->setHorizontalHeaderLabels(CombatColNames);
    combatTable->setEditTriggers(QTableView::NoEditTriggers);
    combatTable->setColumnWidth(C_HP, 80);
    combatTable->setColumnWidth(C_AC, 80);
    combatTable->setColumnWidth(C_DC, 80);
    combatTable->setColumnWidth(C_INIT, 80);

    DeleteAllTableRows(combatTable);
}

// Model used for 'edit_page' (Add Actors) Page
void TableModel::InitializeAddActorTable(QTableWidget *addActors, int cols, QStringList headers)
{
    addActors->clearContents();
    addActors->setColumnCount(cols);
    addActors->setHorizontalHeaderLabels(headers);
    addActors->hideColumn(A_TYPE);
    addActors->setColumnWidth(A_NAME, 150);
    addActors->setColumnWidth(A_NOTES, 530);
    addActors->setEditTriggers(QTableView::NoEditTriggers);

    DeleteAllTableRows(addActors);
}

// Model used for 'edit_page' (Add Actors) Page
void TableModel::InitializeScenarioTable(QTableWidget *scenarioActors, int cols, QStringList headers)
{
    scenarioActors->clearContents();
    scenarioActors->setColumnCount(cols);
    scenarioActors->setHorizontalHeaderLabels(headers);
    scenarioActors->hideColumn(SC_ID);
    scenarioActors->setColumnWidth(SC_NAME, 150);
    scenarioActors->setColumnWidth(SC_NOTES, 530);
    scenarioActors->setEditTriggers(QTableView::NoEditTriggers);

    DeleteAllTableRows(scenarioActors);
}


// Fills Add Actor Table with actors in database
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
    } // END for
}

// Fills Add Actor Table with actors in database
void TableModel::PopulateSelectedScenarioTable(QTableWidget *scenarioActorsTable, QVector<Actor>* scenarioActorList)
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

    for(int index = 0; index < scenarioActorList->length(); index++)
    {
        // Create new row
        scenarioActorsTable->insertRow(index);
        // Insert attributes
        scenarioActorsTable->setItem(index, SC_ID, new QTableWidgetItem(scenarioActorList->at(index).GetID()));
        scenarioActorsTable->setItem(index, SC_NAME, new QTableWidgetItem(scenarioActorList->at(index).GetName()));
        // HP
        hpItem = new QTableWidgetItem;
        hpItem->setData(0,scenarioActorList->at(index).GetHitPoints());
        hpItemList.push_back(hpItem);
        scenarioActorsTable->setItem(index, SC_HP, hpItemList.at(index));
        // AC
        acItem = new QTableWidgetItem;
        acItem->setData(0,scenarioActorList->at(index).GetArmorClass());
        acItemList.push_back(acItem);
        scenarioActorsTable->setItem(index, SC_AC, acItemList.at(index));
        // DC
        dcItem = new QTableWidgetItem;
        dcItem->setData(0,scenarioActorList->at(index).GetSpellSaveDC());
        dcItemList.push_back(dcItem);
        scenarioActorsTable->setItem(index, SC_DC, dcItemList.at(index));
        // Notes
        scenarioActorsTable->setItem(index, SC_NOTES, new QTableWidgetItem(scenarioActorList->at(index).GetNotes()));
        // Type
        scenarioActorsTable->setItem(index, SC_TYPE, new QTableWidgetItem(scenarioActorList->at(index).GetType()));
    } // END for
}


// Model used for 'assignInit_page' (Assign Initiative) Page
void TableModel::InitializeInitiativeModel(QTableWidget *assignInit)
{
    assignInit->clearContents();
    assignInit->setColumnCount(AssignInitColCount);
    assignInit->setHorizontalHeaderLabels(AssignInitColNames);
    assignInit->setColumnWidth(I_NAME, 150);
    assignInit->setEditTriggers(QTableView::NoEditTriggers);
    DeleteAllTableRows(assignInit);
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
void TableModel::CopyTableToInitPage(QTableWidget *origin, QTableWidget *destination)
{ 
    QTableWidgetItem *newCell;
    QSpinBox *qtyBox;
    QString name;
    QString num;

    int destRow = 0;
    int qty = 0;
    int firstOcc = 0;
    bool isFirstOccurence = false;

    // Move through each row & copy contents of each col to destination table
    for(int originRow = 0; originRow < origin->rowCount(); originRow++)
    {
        // Get quantity of current actor in origin table
        qtyBox = qobject_cast<QSpinBox*>(origin->cellWidget(originRow, S_QTY));
        qty = qtyBox->value();

        // Determines how many times actor will be copied to next table
        for(int i = 0; i < qty; i++)
        {
            destRow = destination->rowCount();

            // insert new row to destination table
            destination->insertRow(destination->rowCount());

            // Copy all columns before notes from origin to destination
            for(int col = 0; col < S_NOTES; col++)
            {
                isFirstOccurence = i == 0;

                if(isFirstOccurence)
                {
                    newCell = new QTableWidgetItem(origin->item(originRow, col)->text());
                    destination->setItem(destRow, col, newCell);

                    // Only save first occurence if multiples of actor
                    if(qty > 1)
                    {
                        firstOcc = destRow;
                    }
                }
                else
                {
                    // Copies text of current column for first occurence & stores in new item
                    newCell = new QTableWidgetItem(destination->item(firstOcc, col)->text());

                    // sets item in destination table
                    destination->setItem(destRow, col, newCell);

                    if (col == S_NAME)
                    {
                        // get name & qrt number
                        name = destination->item(destRow,col)->text();
                        num = QString::number(i + 1);

                        // reset actor name to include qty number
                        destination->item(destRow, col)->setText(name  + " " + num);

                        isFirstOccurence = i == (qty - 1);

                        // reset actor name to include qty number
                        if(isFirstOccurence)
                        {
                            name = destination->item(firstOcc, col)->text();
                            num = QString::number(1);
                            destination->item(firstOcc, col)->setText(name  + " " + num);
                        }
                    }
                } // END - if (isFirstOccurence)
            } // END - for (col)

            newCell = new QTableWidgetItem(origin->item(originRow, S_NOTES)->text());
            destination->setItem(destRow, I_NOTES, newCell);
        } // END - for (i)
    } // END - for (originRow)
}

// Copies contents of  assign init table widget's contents to combat page
void TableModel::CopyTableToCombatPage(QTableWidget *origin, QTableWidget *destination)
{
    QSpinBox *initBox;
    QTableWidgetItem *initCell;

    // Move through each row & copy contents of each col to destination table
    for(int row = 0; row < origin->rowCount(); row++)
    {
        destination->insertRow(destination->rowCount());

        for(int col = 0; col < origin->columnCount(); col++)
        {
            if(col == I_INIT)
            {
                initBox = qobject_cast<QSpinBox*>(origin->cellWidget(row, I_INIT));
                initCell = new QTableWidgetItem();
                initCell->setData(0, initBox->value());

                destination->setItem(row, I_INIT, initCell);
            }
            else
            {
                destination->setItem(row, col, origin->takeItem(row, col));
            }
        } // END - for (col)
    } // END - for (row)
}

// Inserts spinbox column to the passed in table
void TableModel::InsertSpinBoxCol(QTableWidget *table, int min, int max, int col, bool disable, bool addToVector)
{
    QSpinBox *sBox;

    if(addToVector) { spinBoxes->clear(); }

    for(int row = 0; row < table->rowCount(); row++)
    {    
        sBox = new QSpinBox(table);
        sBox->setRange(min, max);
        table->setCellWidget(row, col, sBox);

        if(addToVector)
        { spinBoxes->append(sBox); }

        if(disable)
        { sBox->setDisabled(true); }
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
    bool isCompanion = false;

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
        isCompanion = destination->item(placedRow, S_TYPE)->text() == "companion";

        // Checks if actor is a party member; if true, sets quantity to only 1
        if(inParty || isCompanion)
        {
            qtyBox->setRange(1, 1);
            qtyBox->setDisabled(true);
        }
        else
        {
             qtyBox->setRange(qtyMin, qtyMax);
        }

        // Stores quantity spin box into table
        destination->setItem(placedRow, S_QTY, qtyItem);
        destination->setCellWidget(placedRow, S_QTY, qtyBox);

        // Delete old table's listing
        origin->removeRow(selectedRow);
    } // END if(!empty)
}

// Converts all items in a column into a stats spinbox
void TableModel::SetupCombatStatsCol(QTableWidget *table, int overflow, int col)
{
    QSpinBox *sBox;
    int max;

    for(int row = 0; row < table->rowCount(); row++)
    {
        max = table->item(row, col)->text().toInt();

        sBox = new QSpinBox(table);
        sBox->setRange(0, max + overflow);
        sBox->setValue(max);
        sBox->setSuffix(" / " + QString::number(max));

        table->setCellWidget(row, col, sBox);
    }
}

void TableModel::InsertCombatStatsBox(QTableWidget *table, int value, int overflow, int row, int col)
{
    QSpinBox *sBox;
    QTableWidgetItem *item;
    int max;

    item = new QTableWidgetItem();
    table->setItem(row, col, item);
    max = value;

    sBox = new QSpinBox(table);
    sBox->setRange(0, max + overflow);
    sBox->setValue(max);
    sBox->setSuffix(" / " + QString::number(max));

    table->setCellWidget(row, col, sBox);
}

// Populate Scenario Table with Scenario Names
void TableModel::PopulateScenarioNameTable(QTableWidget *scenarioTable, QStringList scenarioNames)
{
    QTableWidgetItem* scenarioItem;

    for(int index = 0; index < scenarioNames.length(); index++)
    {
        // Create Item
        scenarioItem = new QTableWidgetItem;

        // Assign name to item
        scenarioItem->setText(scenarioNames.at(index));

        // Create new row
        scenarioTable->insertRow(index);

        // Insert item
        scenarioTable->setItem(index, 0, scenarioItem);
    } // END for
}

// Constructor
TableModel::TableModel()
{
    spinBoxes = new QVector<QSpinBox*>;
}
