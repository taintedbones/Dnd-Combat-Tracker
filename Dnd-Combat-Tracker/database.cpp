#include "Database.h"
#include <algorithm>

Database::Database(QString path, QString driver) : QSqlDatabase(addDatabase(driver))
{
    setHostName("localhost");
    setDatabaseName(path);

    if(open())
    {
        qDebug() << "Database opened successfully";
    }
    else {
        qDebug() << this->lastError().text();
    }

    // Create Actor List
    actorList = new QVector<Actor>;

    // Create Party List
    combatList = new QVector<Actor>;

    // Create Scenario List
    actorsInScenario = new QVector<Actor>;

    //TODO what does this do?
    scenarioList.clear();

    scenarioQtysList = new QVector<int>;
}

// *************************************************************************************
// Load database of actors into program
// *************************************************************************************
void Database::CreateActorList()
{
    Actor actor; // temp object for loading actorList

    query.prepare("SELECT * FROM actors");

    if(query.exec())
    {
        while(query.next())
        {
            // Populate attributes
            actor.SetID((query.value(ID).toInt()));
            actor.SetName(query.value(NAME).toString());
            actor.SetHitPoints(query.value(HP).toInt());
            actor.SetArmorClass(query.value(AC).toInt());
            actor.SetSpellSaveDC(query.value(DC).toInt());
            actor.SetNotes(query.value(NOTES).toString());
            actor.SetType(query.value(TYPE).toString());
            // Add Actor to List
            actorList->push_back(actor);
        }
    }
    else // Print error if query is unsuccessful
    {
        qDebug() << query.lastError().text();
    }
}

// *************************************************************************************
// Pull party from actor list
// *************************************************************************************
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

// *************************************************************************************
// Load scenarios from database into program memory
// *************************************************************************************
void Database::CreateScenarioList()
{
    scenarioList.clear();
    query.prepare("SELECT DISTINCT scenarioName FROM scenarios");

    if(query.exec())
    {
        while(query.next())
        {
           scenarioList.append(query.value(0).toString());
        }
    }
    else // Print error if query is unsuccessful
    {
        qDebug() << query.lastError().text();
    }
}

// *************************************************************************************
// Access scenario actor is part of based on actorID
// *************************************************************************************
QString Database::GetScenarioByID(int id)
{
    QString scenario;

    query.prepare("SELECT scenarioName FROM scenarios WHERE actorID = :id");
    query.bindValue(":id", id);

    if(query.exec())
    {
        if(query.next())
        {
            scenario = query.value(0).toString();
        }
    }
    else
    {
        qDebug() << query.lastError().text();
    }

    return scenario;
}

// *************************************************************************************
// Accessor to return list of all actors currently on list
// *************************************************************************************
QVector<Actor>* Database::GetActorList() const
{   
    return actorList;
}

// *************************************************************************************
// Accessor to return list of partymembers
// *************************************************************************************
QVector<Actor>* Database::GetPartyList() const
{
    return combatList;
}

// *************************************************************************************
// Access list of scenarios
// *************************************************************************************
QStringList Database::GetScenarioList() const
{
    return scenarioList;
}

// *************************************************************************************
// Generate and return list of quantities of actors in given scenario name
// *************************************************************************************
QVector<int>* Database::GetScenarioQtys(const QString &scenarioName)
{
    scenarioQtysList->clear();

    query.prepare("SELECT quantity FROM scenarios WHERE scenarioName =:scenarioName");
    query.bindValue(":scenarioName", scenarioName);

    // If successful, assign values to vector and return
    if(query.exec())
    {
        while(query.next())
        {
            scenarioQtysList->append(query.value(0).toInt());
        }
    }
    else // Print error
    {
        qDebug() << query.lastError().text();
    }

    return scenarioQtysList;
}

// *************************************************************************************
// Access data of passed in actor name
// *************************************************************************************
Actor Database::GetActor(QString name)
{
    Actor foundActor;

    query.prepare("SELECT * FROM actors WHERE name = :name");
    query.bindValue(":name", name);

    if(query.exec())
    {
        if(query.next())
        {
            foundActor.SetID(query.value(ID).toInt());
            foundActor.SetName(query.value(NAME).toString());
            foundActor.SetHitPoints(query.value(HP).toInt());
            foundActor.SetArmorClass(query.value(AC).toInt());
            foundActor.SetSpellSaveDC(query.value(DC).toInt());
            foundActor.SetNotes(query.value(NOTES).toString());
            foundActor.SetScenario(GetScenarioByID(foundActor.GetID()));
        }
    }
    else
    {
        qDebug() << query.lastError().text();
    }

    return foundActor;
}

// *************************************************************************************
// Add actor to database
// *************************************************************************************
void Database::AddActor(Actor* toAdd)
{
    // Prepare query
    query.prepare("INSERT INTO actors (name,health,armorClass,spellSaveDC,notes,type)"
                  "VALUES (:name,:health,:armorClass,:spellSaveDC,:notes,:type)");

    // Bind values
    query.bindValue(":name", toAdd->GetName());
    query.bindValue(":health", toAdd->GetHitPoints());
    query.bindValue(":armorClass", toAdd->GetArmorClass());
    query.bindValue(":spellSaveDC", toAdd->GetSpellSaveDC());
    query.bindValue(":notes", toAdd->GetNotes());
    query.bindValue(":type", toAdd->GetType());

    // Execute query. Print error if unsuccessful
    if(!query.exec()) { qDebug() << query.lastError().text(); }
}

// Edit actor in DB
void Database::EditActor(Actor *toEdit)
{
        query.prepare("UPDATE actors "
                      "SET name = :name, "
                      "health = :health, "
                      "armorClass = :armorClass, "
                      "spellSaveDC = :spellSaveDC, "
                      "notes = :notes, "
                      "type = :type "
                      "WHERE actorID = :actorID;");

        // Bind safe values
        query.bindValue(":actorID", toEdit->GetID());
        query.bindValue(":name", toEdit->GetName());
        query.bindValue(":health", toEdit->GetHitPoints());
        query.bindValue(":armorClass", toEdit->GetArmorClass());
        query.bindValue(":spellSaveDC", toEdit->GetSpellSaveDC());
        query.bindValue(":notes", toEdit->GetNotes());
        query.bindValue(":type", toEdit->GetType());

        // Print error if unsuccessful
        if(!query.exec()) { qDebug() << query.lastError().text(); }
}

// Delete actor from DB
void Database::DeleteActor(const int &actorID)
{
    query.prepare("DELETE FROM actors WHERE actorID = :actorID");

    query.bindValue(":actorID", actorID);

    // Print error if unsuccessful
    if(!query.exec()) { qDebug() << query.lastError().text(); }
}

// Get actor list by scenario name
QVector<Actor>* Database::GetActorsByScenario(const QString &scenarioName)
{
    Actor actor; // actor to add

    actorsInScenario->clear();

    query.prepare("SELECT actors.actorID, actors.name, actors.health, "
                  "actors.armorClass, actors.spellSaveDC, actors.notes, actors.type "
                  "FROM actors, scenarios "
                  "WHERE scenarios.scenarioName = :scenarioName "
                  "AND scenarios.actorID = actors.actorID");

    query.bindValue(":scenarioName", scenarioName);

    // Print error if unsuccessful
    if(!query.exec()) { qDebug() << query.lastError().text(); }

    // Add names to list
    while(query.next())
    {
        // Load data into object
        actor.SetID(query.value(0).toInt());
        actor.SetName(query.value(1).toString());
        actor.SetHitPoints(query.value(2).toInt());
        actor.SetArmorClass(query.value(3).toInt());
        actor.SetSpellSaveDC(query.value(4).toInt());
        actor.SetNotes(query.value(5).toString());
        actor.SetType(query.value(6).toString());

        // Insert object
        actorsInScenario->push_back(actor);
    }

    return actorsInScenario;
}

// TODO Add scenario to DB

// TODO Edit scenario in DB

// TODO Delete scenario from DB
