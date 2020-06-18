#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QTableWidget>

// Forward Declaration of Actor Class
class Actor;

class TableModel
{
public:
        // Combat Model
    // Column Count
    const int CombatColCount = 7;

    // Column Positions
    enum CombatColPositions { C_NAME, C_HP_CUR, C_HP_MAX, C_AC, C_DC, C_INIT, C_NOTES };

    // Column Names
    QStringList CombatColNames = { "Name", "HP: Cur", "Max", "AC", "DC", "Init", "Notes" };
        // Actor List Model
    // Column Count
    const int ActorListColCount = 5;

    // Column Positions
    enum ActorListColPositions { A_NAME, A_HP, A_AC, A_DC, A_NOTES };

    // Column Names
    QStringList ActorListColNames = { "Name", "HP", "AC", "DC", "Notes" };

        // Assign Initiative Model //
    // Column Count
    const int AssignInitColCount = 6;

    // Column Positions
    enum AssignInitColPositions { I_NAME, I_HP, I_AC, I_DC, I_INIT, I_NOTES };

    // Column Names
    QStringList AssignInitColNames = { "Name", "HP", "AC", "DC", "Init", "Notes" };

    // Model used for 'combat_page' (Conduct Combat) Page
    void InitializeCombatModel(QTableWidget *combatTable);

    // Model used for 'edit_page' (Add Actors) Page
    void InitializeAddActorTable(QTableWidget *addActors);

    void PopulateAddActorTable(QTableWidget *addActors, QVector<Actor>* actorList);

    // Model used for 'assignInit_page' (Assign Initiative) Page
    void InitializeInitiativeModel(QTableWidget *assignInit);

    // Move actor from one table to the other
    void MoveActorToTable(QTableWidget* origin, QTableWidget* destination);

    // Constructor
    TableModel();
};

#endif // TABLEMODEL_H
