#ifndef DATABASE_H
#define DATABASE_H

#ifndef DEBUG
#define DEBUG 0
#endif

#include <QtSql>
#include <QString>
#include "Actor.h"
#include "ScenarioListing.h"

class Database : public QSqlDatabase
{
public:
    // Access list of partymembers
    QVector<Actor>* GetPartyList();

    // Access list of actors
    QVector<Actor>* GetActorList();

    QVector<Actor>* GetNonPartyActors();

    // Access list of scenarios
    QStringList GetScenarioList();

    // Access scenario actor is part of based on actorID
    QString GetScenarioByID(int id);

    // Access data of passed in actor name
    Actor GetActor(QString name);

    // Add actor to DB
    void AddActor(Actor* toAdd);

    // Edit actor in DB
    void EditActor(Actor* toEdit);

    // Delete actor from DB
    void DeleteActor(const int &actorID);

    // Get actor list of given scenario table
    QVector<Actor>* GetActorsByScenario(const QString &scenarioName);

    // Get quantities of given scenario table
    QVector<int>* GetScenarioQtys(const QString &scenarioName);

    // Save scenario changes to DB
    void SaveChangesToScenario(QVector<ScenarioListing>* scenarioListings);

    // Delete scenario from db
    void DeleteScenario(const QString &scenarioName);

    bool IsInDatabase(QString name);

    // Constructor
    Database(QString path, QString driver);

    // Destructor
    ~Database() {}

private:
    QSqlQuery query; // Reusable query for all class methods

    enum ActorProfile { ID, NAME , HP, AC, DC, NOTES, TYPE };
};

#endif // DATABASE_H
