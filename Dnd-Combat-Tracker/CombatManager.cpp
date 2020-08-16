#include "CombatManager.h"
#include "Actor.h"
#include <QMessageBox>
#include <QInputDialog>

CombatManager::CombatManager(QTableWidget *table)
{
    combat = table;
    round = 1;
}

// *************************************************************************************
// Determines if the passed in actor is already in the combat table
// *************************************************************************************
bool CombatManager::IsActorInCombat(QString name)
{
    QString currentName;
    QStringList nameParse;
    int row = 0;
    bool found = false;

    while(!found && row < combat->rowCount())
    {
        currentName = combat->item(row, NAME)->text();

        // Parse the current name in case it has a number after it
        nameParse = currentName.split(" ");

        if(nameParse.size() > 1)
        {
            nameParse.removeLast();

            if(nameParse.size() > 1)
            {
                currentName = nameParse.join(" ");
            }
            else
            {
                currentName = nameParse.first();
            }
        }
        else
        {
            currentName = nameParse.first();
        }

        if(currentName == name)
        {
            found = true;
        }

        row++;
    }

    return found;
}

// *************************************************************************************
// Inserts the passed in actor to the table, sorted by the passed in initiative
// *************************************************************************************
void CombatManager::InsertActorToCombat(Actor actor, int init)
{
    QTableWidgetItem *item;
    QString name;
    int row = 0;

    // Save actor name for renaming of actors to include number
    name = actor.GetName();

    row = FindInsertRow(init);

    qDebug() << row;

    combat->insertRow(row);

    // Handles which data will be placed on the table depending on the column
    for(int col = 0; col < tableManager.CombatColCount; col++)
    {
        switch(col)
        {
        case NAME:
            item = new QTableWidgetItem(name);
            break;
        case HP:
           tableManager.InsertCombatStatsBox(combat, actor.GetHitPoints(), 10, row, col);
            break;
        case AC:
            tableManager.InsertCombatStatsBox(combat, actor.GetArmorClass(), 0, row, col);
            break;
        case DC:
            item = new QTableWidgetItem(QString::number(actor.GetSpellSaveDC()));
            break;
        case INIT:
            item = new QTableWidgetItem(QString::number(init));
            break;
        case NOTES:
            item = new QTableWidgetItem(actor.GetNotes());
            break;
        }

        // Inserts item to table if cell does not contain a spinbox
        if(col != HP && col != AC)
        {
            combat->setItem(row, col, item);
        }
    } // END - for(col)

    if(row == 1)
    {
        CheckForTie();
    }
}

// *************************************************************************************
// Inserts divider row to the end of the combat table
// *************************************************************************************
void CombatManager::InsertRoundDivider()
{
    QTableWidgetItem *item;

    combat->insertRow(combat->rowCount());

    for(int col = 0; col < combat->columnCount(); col++)
    {
        if(col == NOTES)
        {
            item = new QTableWidgetItem("ROUND: " + QString::number(round + 1) + " -------------------------------");
        }
        else
        {
            item = new QTableWidgetItem(DIV);
            item->setTextAlignment(Qt::AlignCenter);
        }

        item->setBackgroundColor(Qt::GlobalColor::lightGray);
        combat->setItem(combat->rowCount() - 1, col, item);
    }
}

// *************************************************************************************
// Returns the current round number
// *************************************************************************************
int CombatManager::GetRound() const
{
    return round;
}

// *************************************************************************************
// Moves the top row to the bottom, displaying a change of turn
// *************************************************************************************
void CombatManager::NextTurn()
{
    int row;
    bool isDivider = false;
    bool lastInRound = false;

    combat->insertRow(combat->rowCount());
    row = combat->rowCount() - 1;

    isDivider = combat->item(0, NAME)->text() == DIV;

    for(int col = 0; col < combat->columnCount(); col++)
    {
        if((col == HP || col == AC) && !isDivider)
        {
            combat->setCellWidget(row, col, combat->cellWidget(0, col));
        }
        else
        {
            combat->setItem(row, col, combat->takeItem(0, col));
        }
    }

    combat->removeRow(0);

    lastInRound = combat->item(0, NAME)->text() == DIV;

    if(lastInRound)
    {
        round++;
        NextTurn();
        combat->item(combat->rowCount() - 1, NOTES)->setText("ROUND: " + QString::number(round + 1) + " -------------------------------");
    }
}

// *************************************************************************************
// Displays confirmation qmessagebox & removes selected actor from combat
// *************************************************************************************
void CombatManager::DeleteActor()
{
    QMessageBox confirmBox;
    QString selectedName;
    bool emptyCombat;
    int selectedRow;

    emptyCombat = combat->rowCount() < 1;
    selectedRow = combat->currentRow();

    if(!emptyCombat && !IsDivider(selectedRow))
    {
        selectedName = combat->item(selectedRow, NAME)->text();

        // Set message box attributes to confirm removal of selected actor
        confirmBox.setText("Remove actor from combat");
        confirmBox.setInformativeText("Are you sure you want to remove " + selectedName + " ?");
        confirmBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        confirmBox.setDefaultButton(QMessageBox::Yes);
        int ret = confirmBox.exec();

        // if user selects 'yes' then remove row
        if(ret == QMessageBox::Yes)
        {
            combat->removeRow(selectedRow);
        }
    }
    else
    {
        QMessageBox::warning(combat, "No Actors in Combat", "Add an actor to the combat before removing");
    }

    if(selectedRow == 1)
    {
        CheckForTie();
    }
}

// *************************************************************************************
// Checks if the passed in row is a divider
// *************************************************************************************
bool CombatManager::IsDivider(int row)
{
    return combat->item(row, NAME)->text() == DIV;
}

// *************************************************************************************
// Checks if the combat table is empty
// *************************************************************************************
bool CombatManager::IsEmpty()
{
    return combat->rowCount() == 1;
}

// *************************************************************************************
//  Searches the table for the row that a new actor will be inserted at, based on initiative
// *************************************************************************************
int CombatManager::FindInsertRow(int initiative)
{
    int divRow = GetDividerLocation();
    int first = -1;
    int last = -1;
    int row = 0;
    int insertAtRow = -1;

    bool found = false;
    bool divIsLast = divRow == combat->rowCount() - 1;
    bool firstGreater = combat->item(0, INIT)->text().toInt() >= initiative;

    // Divider is in last row
    if(divIsLast)
    {
        if(firstGreater) // Actor gets inserted into current round
        {
            first = 0;
            last = divRow - 1;
        }
        else // New actor init greater than current turn init
        {
            first = divRow + 1;
            last = first;
        }
    }
    else // Divider anywhere else in table
    {
        if(firstGreater) // Actor gets inserted to current round
        {
            first = 0;
            last = divRow - 1;
        }
        else // Actor gets inserted to next round
        {
            first = divRow + 1;
            last = combat->rowCount() - 1;
        }
    }

    // Init will be placed above/below divider on side where only one row exists
    if(first == last)
    {
        insertAtRow = last;
    }
    // Divider is anywhere else in combat table
    else
    {
        row = first;

        // Linear search from 'first' to 'last' row
        while(!found && row < (last + 1))
        {
            if(combat->item(row, INIT)->text().toInt() < initiative)
            {
                insertAtRow = row;
                found = true;
            }
            else // Add to end of list
            {
                insertAtRow = row + 1;
            }

            row++;
        }
    }

    return insertAtRow;
}

// *************************************************************************************
// Searches combat for divider row location
// *************************************************************************************
int CombatManager::GetDividerLocation()
{
    bool found = false;
    int loc = -1;
   int row = 0;

   while(!found && row < combat->rowCount())
   {
        if(IsDivider(row))
        {
            found = true;
            loc = row;
        }

       row++;
   }

   return loc;
}

// *************************************************************************************
// Checks the combat table for a tied initiative and prompts the user to select which
//      actor will take the current turn
// *************************************************************************************
void CombatManager::CheckForTie()
{
    QInputDialog tiePrompt;
    QStringList names;
    QString selectedTurn;

    int first = 0;
    int firstInit = combat->item(first, INIT)->text().toInt();
    int last = -1;

    bool active = true;
    bool initMatch = false;
    bool tie= false;

    last = first + 1;
    names.append(combat->item(first, NAME)->text());

    // Linear search to check if the first actor in the list has a tied initiative
    while(active)
    {
        initMatch = combat->item(last, INIT)->text().toInt() == firstInit;

        if(initMatch)
        {
            names.append(combat->item(last, NAME)->text());
            last++;
        }
        else
        {
            active = false;
        }
    }

    tie = names.size() > 1;

    // If a tie is found, prompts the user to select which actor will go next
    if(tie)
    {

        tiePrompt.resize(400, 200);

        // Display input dialog modal window with combobox of actors to select from
        selectedTurn = tiePrompt.getItem(combat, "TIE!", "Please select which actor will go this turn:", names);

        // Moves the selected actor to the first position in the list
        if(selectedTurn != combat->item(0, NAME)->text())
        {
            int row = FindActorByName(selectedTurn);

            combat->insertRow(0);
            row += 1;

            for(int col = 0; col < combat->columnCount(); col++)
            {
                if(col == HP || col == AC)
                {
                    combat->setCellWidget(0, col, combat->cellWidget(row, col));
                }
                else
                {
                    combat->setItem(0, col, combat->takeItem(row, col));
                }
            }

            combat->removeRow(row);
        }
    }
}

// *************************************************************************************
// Searches the combat table for the passed in actor and returns their row number
// *************************************************************************************
int CombatManager::FindActorByName(QString name)
{
    bool found = false;
    int foundRow= -1;
    int row = 0;

    while(!found && row < combat->rowCount())
    {
        if(combat->item(row, NAME)->text() == name)
        {
            foundRow = row;
            found = true;
        }
        row++;
    }

    return foundRow;
}
