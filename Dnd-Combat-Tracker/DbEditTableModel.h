#ifndef DBEDITTABLEMODEL_H
#define DBEDITTABLEMODEL_H

#include <QObject>
#include <QSqlTableModel>
#include "Database.h"

class DbEditTableModel : public QSqlTableModel
{
    Q_OBJECT
public:
    DbEditTableModel(QObject *parent, Database *db);
    void Initialize();
    void InitializeScenarios(QString combatName);
};

#endif // DBEDITTABLEMODEL_H
