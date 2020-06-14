#ifndef ACTOR_H
#define ACTOR_H

#include <QString>


class Actor
{
public:

    // Setters
    void SetName(QString &name);
    void SetHitPoints(int &hitpoints);
    void SetArmorClass(int &armorClass);
    void SetSpellSaveDC(int &spellSaveDC);
    void SetInitiative(int &initiative);
    void SetNotes(QString &notes);

    // Getters
    QString GetName() const;
    int GetHitPoints() const;
    int GetArmorClass() const;
    int GetSpellSaveDC() const;
    int GetInitiative() const;
    QString GetNotes() const;

    // Constructors
        // Default Constructor
    Actor();

        // Parameterized Constructor
    Actor(QString name, int hitpoints, int armorClass, int spellSaveDC, int initiative, QString notes);

private:
    QString _name;
    int _hitpoints;
    int _armorClass;
    int _spellSaveDC;
    int _initiative;
    QString _notes;
};

#endif // ACTOR_H
