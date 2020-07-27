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

    //  Initialies model to display actor table & sets headers
    void Initialize();

    //  Initializes model to display scenario_list view & sets header
    void InitializeScenarios();

    //  Initializes model to display scenario_actorList view & filters table by passed in
    //      scenario name
    void InitializeScenarioByName(QString scenarioName);
private:
    Database *database;
};

#endif // DBEDITTABLEMODEL_H
