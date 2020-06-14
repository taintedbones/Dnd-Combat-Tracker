#include "Database.h"

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

            // Create object

            // Add Actor to List
            actorList->push_back(actor);

            // DEBUG ONLY - Print Listing from Database
            qDebug() << "Printing Database Listing";
            for(int index = 1; index < 6; index++)
            {
                qDebug() << query.value(index);
            }

            qDebug() << "Printing Memory Listing";
            // DEBUG ONLY - Print Object Attributes
            qDebug() << "Name: " << actor.GetName();
            qDebug() << "HP: " << actor.GetHitPoints();
            qDebug() << "AC: " << actor.GetArmorClass();
            qDebug() << "DC: " << actor.GetSpellSaveDC();
            qDebug() << "Notes: " << actor.GetNotes();
        }
    }
    else // Print error if query is unsuccessful
    {
        qDebug() << query.lastError().text();
    }
}

QVector<Actor>* Database::GetActorList() const
{
    return actorList;
}
