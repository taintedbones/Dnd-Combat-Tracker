#ifndef DATABASE_H
#define DATABASE_H

#ifndef DEBUG
#define DEBUG 0
#endif

#include <QtSql>
#include <QString>
#include "Actor.h"

class Database : public QSqlDatabase
{
public:
    // Load actors from database into program memory
    void CreateActorList();

    // Load partymembers from database into program memory
    void CreatePartyList();

    void CreateScenarioList();

    // Access list of partymembers
    QVector<Actor>* GetPartyList() const;

    // Access list of actors
    QVector<Actor>* GetActorList() const;

    QStringList GetScenarioList() const;

    QString GetScenarioByID(int id);

    Actor GetActor(QString name);

    Database(QString path, QString driver);
    ~Database() {}

private:
    QVector<Actor>* actorList = nullptr; // list of actor profiles
    QVector<Actor>* combatList = nullptr; // list of actor profiles added to combat
    QStringList scenarioList;


    QSqlQuery query; // Reusable query for all class methods

    enum ActorProfile { ID, NAME , HP, AC, DC, NOTES, TYPE };

};

#endif // DATABASE_H
