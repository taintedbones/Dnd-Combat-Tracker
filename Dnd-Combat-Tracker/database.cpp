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
}
