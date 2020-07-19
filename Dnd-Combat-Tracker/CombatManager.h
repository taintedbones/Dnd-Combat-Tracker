#ifndef COMBATMANAGER_H
#define COMBATMANAGER_H

#include <QTableWidget>
#include <QDebug>
#include "TableModel.h"

class Actor;

class CombatManager
{
public: 
    enum CombatCols { NAME, HP, AC, DC, INIT, NOTES };

    CombatManager();
    CombatManager(QTableWidget *table);

    // Determines if the passed in actor is already in the combat table
    bool IsActorInCombat(QString name);

    // Inserts the passed in actor to the table, sorted by the passed in initiative
    void InsertActorToCombat(Actor actor, int init);

    // Inserts divider row to the end of the combat table
    void InsertRoundDivider();

    // Returns the current round number
    int GetRound() const;

    // Moves the top row to the bottom, displaying a change of turn
    void NextTurn();

    // Displays confirmation qmessagebox & removes selected actor from combat
    void DeleteActor();

    // Checks if the passed in row is a divider
    bool IsDivider(int row);

    // Checks if the combat table is empty
    bool IsEmpty();

    //  Searches the table for the row that a new actor will be inserted at, based on initiativeS
    int FindInsertRow(int initiative);

    // Searches combat for divider row location
    int GetDividerLocation();
private:
    QTableWidget *combat;
    TableModel tableManager;
    int round;
    const QString DIV = "------------";
};

#endif // COMBATMANAGER_H
