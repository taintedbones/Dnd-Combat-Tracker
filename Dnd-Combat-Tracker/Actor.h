#ifndef ACTOR_H
#define ACTOR_H

#include <QString>


class Actor
{
public:

    // Setters
    void SetName(QString &name);
    void SetHealth(int &health);
    void SetArmorClass(int &armorClass);
    void SetSpellSaveDC(int &spellSaveDC);
    void SetInitiative(int &initiative);
    void SetNotes(QString &notes);

    // Getters
    QString GetName() const;
    int GetHealth() const;
    int GetArmorClass() const;
    int GetSpellSaveDC() const;
    int GetInitiative() const;
    QString GetNotes() const;

    // Constructors
        // Default Constructor
    Actor();

        // Parameterized Constructor
    Actor(QString name, int health, int armorClass, int spellSaveDC, int initiative, QString notes);

private:
    QString _name;
    int _health;
    int _armorClass;
    int _spellSaveDC;
    int _initiative;
    QString _notes;
};

#endif // ACTOR_H
