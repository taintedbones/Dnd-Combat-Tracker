#include "Database.h"
#include <algorithm>

Database::Database(QString path, QString driver) : QSqlDatabase(addDatabase(driver))
{
    setHostName("localhost");
    setDatabaseName(path);

    if(open()) {
        qDebug() << "Database opened sucessfully";
    }
    else {
        qDebug() << this->lastError().text();
    }

    // Create Actor List
    actorList = new QVector<Actor>;

    // Create party list
    combatList = new QVector<Actor>;
}

// Load database of actors into program
void Database::CreateActorList()
{
    // These temp variables are here because the compiler isnt letting me assign the
    // values of object Actor directly using the values of object query. Also,
    // I could just use two variables, but I created more for readability.
    QString name;
    int hitpoints;
    int armorClass;
    int spellDC;
    QString notes;
    QString type;

    Actor actor; // temp object for loading actorList

    query.prepare("SELECT * FROM actors");

    if(query.exec())
    {
        while(query.next())
        {
            // Populate attributes
                // Name
            name = query.value(NAME).toString();
            actor.SetName(name);
                // HP
            hitpoints = query.value(HP).toInt();
            actor.SetHitPoints(hitpoints);
                // AC
            armorClass = query.value(AC).toInt();
            actor.SetArmorClass(armorClass);
                // DC
            spellDC = query.value(DC).toInt();
            actor.SetSpellSaveDC(spellDC);
                // Notes
            notes = query.value(NOTES).toString();
            actor.SetNotes(notes);

                // Type
            type = query.value(TYPE).toString();
            actor.SetType(type);

            // Add Actor to List
            actorList->push_back(actor);
        }
    }
    else // Print error if query is unsuccessful
    {
        qDebug() << query.lastError().text();
    }
}

// Pull party from actor list
void Database::CreatePartyList()
{
    // Cycle actorlist, pulling partymembers onto partylist
    for(int index = 0; index < actorList->length(); index++)
    {
        // If type 'partymember' found, place on partylist
        if(actorList->at(index).GetType() == "partymember")
        {
            // Add actor to partyList
            combatList->push_back(actorList->at(index));
            // Remove actor from actorList
            actorList->remove(index);
            // Go back a space since there's a gap now
            index--;
        }
    }
}

// Accessor to return list of all actors currently on list
QVector<Actor>* Database::GetActorList() const
{
    return actorList;
}

// Accessor to return list of partymembers
QVector<Actor>* Database::GetPartyList() const
{
    return combatList;
}



