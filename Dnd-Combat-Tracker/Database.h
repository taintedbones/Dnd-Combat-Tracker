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
    Database(QString path, QString driver);
    ~Database() {}

    void AddActor();


};

#endif // DATABASE_H
