#include "CombatManager.h"
#include "Actor.h"

CombatManager::CombatManager(QTableWidget *table)
{
    combat = table;
    round = 1;
}

// Determines if the passed in actor is already in the combat table
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

void CombatManager::InsertActorToCombat(Actor actor, int init)
{
    QTableWidgetItem *item;
    QString name;
    int row = 0;

    // Save actor name for renaming of actors to include number
    name = actor.GetName();

    //    qty = ui->qty_premade_spinBox->value();

    combat->insertRow(combat->rowCount());
    row = combat->rowCount() - 1;

    // Handles which data will be placed on the table depending on the column
    for(int col = 0; col < tableManager.CombatColCount; col++)
    {
        switch(col)
        {
        case NAME:
//            item = new QTableWidgetItem(name + " " + QString::number(i + 1));
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
}


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
            item = new QTableWidgetItem("------------");
            item->setTextAlignment(Qt::AlignCenter);
        }

        item->setBackgroundColor(Qt::GlobalColor::lightGray);
        combat->setItem(combat->rowCount() - 1, col, item);
    }
}

int CombatManager::GetRound() const
{
    return round;
}

// Moves the top row to the bottom, displaying a change of turn
void CombatManager::NextTurn()
{
    int row;
    bool isDivider = false;
    bool lastInRound = false;

    combat->insertRow(combat->rowCount());
    row = combat->rowCount() - 1;

    isDivider = combat->item(0, NAME)->text() == "------------";

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

    lastInRound = combat->item(0, NAME)->text() == "------------";

    if(lastInRound)
    {
        round++;
        NextTurn();
        combat->item(combat->rowCount() - 1, NOTES)->setText("ROUND: " + QString::number(round + 1) + " -------------------------------");
    }
}

