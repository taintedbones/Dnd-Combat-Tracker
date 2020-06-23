#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QTableWidget>
#include <QSpinBox>
#include <QVariant>

// Forward Declaration of Actor Class
class Actor;

class TableModel
{
public:
     // ********************************** Combat Model **********************************
    const int CombatColCount = 7;

    enum CombatColPositions { C_NAME, C_HP_CUR, C_HP_MAX, C_AC, C_DC, C_INIT, C_NOTES };

    QStringList CombatColNames = { "Name", "HP: Cur", "Max", "AC", "DC", "Init", "Notes" };

    // ********************************** Actor List Model **********************************
    const int ActorListColCount = 6;

    enum ActorListColPositions { A_NAME, A_HP, A_AC, A_DC, A_NOTES, A_TYPE };

    QStringList ActorListColNames = { "Name", "HP", "AC", "DC", "Notes", "Type"};

    // **********************************  Selected Actor Model **********************************
    const int SelectedListColCount = 7;

    enum SelectedListColPositions {S_NAME, S_HP, S_AC, S_DC, S_NOTES, S_TYPE, S_QTY};

    QStringList SelectedListColNames = {"Name", "HP", "AC", "DC", "Notes", "Type", "Quantity"};

     // ********************************** Assign Initiative Model **********************************
    const int AssignInitColCount = 6;

    enum AssignInitColPositions { I_NAME, I_HP, I_AC, I_DC, I_INIT, I_NOTES };

    QStringList AssignInitColNames = { "Name", "HP", "AC", "DC", "Init", "Notes" };

    // ********************************** Database Edit Model ***********************************

    enum DbEditColPositions { D_ID, D_NAME, D_HP, D_AC, D_DC, D_NOTES, D_TYPE};

    // ************************************************************************************************

    const int QtyMax = 10;
    const int QtyMin = 1;

    // Model used for 'combat_page' (Conduct Combat) Page
    void InitializeCombatModel(QTableWidget *combatTable);

    // Model used for 'edit_page' (Add Actors) Page
    void InitializeAddActorTable(QTableWidget *addActors, int cols, QStringList headers);

    void PopulateAddActorTable(QTableWidget *addActors, QVector<Actor>* actorList);

    // Model used for 'assignInit_page' (Assign Initiative) Page
    void InitializeInitiativeModel(QTableWidget *assignInit);

    // Remove actor from one table and back to its origin
    void RemoveActorFromTable(QTableWidget* origin, QTableWidget* destination);

    // Copies entire contents of one tablewidget to another
    void CopyTable(QTableWidget *origin, QTableWidget *destination, bool hasInit);

    // Inserts initiative column to the passed in table
    void InsertSpinBoxCol(QTableWidget *table, int min, int max, int col);

    // Add Actors - Show selected actor type in actor list
    void ShowActorType(QTableWidget* addActors, const QString &type);

    // Deletes entire contents of passed in table
    void DeleteAllTableRows(QTableWidget *table);

    // Adds actor to new table from origin with quantity spin boxes
    void AddActorToTable(QTableWidget *origin, QTableWidget *destination);

    // Constructor
    TableModel();
};

#endif // TABLEMODEL_H
