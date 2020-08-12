#include "Actor.h"

    // Setters //
// Set actor ID
void Actor::SetID(int id) { _id = id; }

// Set actor name
void Actor::SetName(QString name) { _name = name; }

// Set actor health
void Actor::SetHitPoints(int hitpoints) { _hitpoints = hitpoints; }

// Set actor armor class
void Actor::SetArmorClass(int armorClass) { _armorClass = armorClass; }

// Set actor spell save DC
void Actor::SetSpellSaveDC(int spellSaveDC) { _spellSaveDC = spellSaveDC; }

// Set actor initiative score
void Actor::SetInitiative(int initiative) { _initiative = initiative; }

// Set notes on actor
void Actor::SetNotes(QString notes) { _notes = notes; }

// Set type of actor
void Actor::SetType(QString type) { _type = type; }

    // Getters //

// Get actor ID
int Actor::GetID() const { return _id; }

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

// Get type of actor
QString Actor::GetType() const { return _type; }


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
    _type = "";
}

// Parameterized Constructor
Actor::Actor(int id, QString name, int hitpoints, int armorClass, int spellSaveDC, int initiative, QString notes, QString type)
{
    _id = id;
    _name = name;
    _hitpoints = hitpoints;
    _armorClass = armorClass;
    _spellSaveDC = spellSaveDC;
    _initiative = initiative;
    _notes = notes;
    _type = type;
}
