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

    // Access list of actors
    QVector<Actor>* GetActorList() const;

    Database(QString path, QString driver);
    ~Database() {}

private:
    QVector<Actor>* actorList = nullptr; // list of actor profiles


    QSqlQuery query; // Reusable query for all class methods

    enum ActorProfile { NAME = 1, HP, AC, DC, NOTES };

};

#endif // DATABASE_H
