#include "Actor.h"

    // Setters //

// Set actor name
void Actor::SetName(QString &name) { _name = name; }

// Set actor health
void Actor::SetHitPoints(int &hitpoints) { _hitpoints = hitpoints; }

// Set actor armor class
void Actor::SetArmorClass(int &armorClass) { _armorClass = armorClass; }

// Set actor spell save DC
void Actor::SetSpellSaveDC(int &spellSaveDC) { _spellSaveDC = spellSaveDC; }

// Set actor initiative score
void Actor::SetInitiative(int &initiative) { _initiative = initiative; }

// Set notes on actor
void Actor::SetNotes(QString &notes) { _notes = notes; }

    // Getters //

// Get actor name
QString Actor::GetName() const { return _name; }

// Get actor health value
int Actor::GetHitPoints() const { return _hitpoints; }

// Get actor armor class
int Actor::GetArmorClass() const { return _armorClass; }

// Get actor spell save DC
int Actor::GetSpellSaveDC() const { return _spellSaveDC; }

// Get actor initiative score
int Actor::GetInitiative() const { return _initiative; }

// Get notes on actor
QString Actor::GetNotes() const { return _notes; }

    // Constructors

// Default Constructor
Actor::Actor()
{
    _name = "";
    _hitpoints = 0;
    _armorClass = 0;
    _spellSaveDC = 0;
    _initiative = 0;
    _notes = "";
}

// Parameterized Constructor
Actor::Actor(QString name, int hitpoints, int armorClass, int spellSaveDC, int initiative, QString notes)
{
    _name = name;
    _hitpoints = hitpoints;
    _armorClass = armorClass;
    _spellSaveDC = spellSaveDC;
    _initiative = initiative;
    _notes = notes;
}
