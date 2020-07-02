#ifndef COMBATMANAGER_H
#define COMBATMANAGER_H

#include <QTableWidget>
#include <QDebug>
#include "TableModel.h"

class Actor;

enum CombatCols { NAME, HP, AC, DC, INIT, NOTES };

class CombatManager
{
public:
    CombatManager();
    CombatManager(QTableWidget *table);
    bool IsActorInCombat(QString name);
    void InsertActorToCombat(Actor actor, int init);
    void InsertRoundDivider();
    int GetRound() const;
    void NextTurn();

private:
    QTableWidget *combat;
    TableModel tableManager;
    int round;
};

#endif // COMBATMANAGER_H
