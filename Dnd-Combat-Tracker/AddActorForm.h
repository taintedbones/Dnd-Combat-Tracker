#ifndef ADDACTORFORM_H
#define ADDACTORFORM_H

#include <QWidget>
#include <QPushButton>
#include "Database.h"
#include "Actor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AddActorForm; }
QT_END_NAMESPACE

enum Pages { MENU, CUSTOM, PREMADE };

class AddActorForm : public QWidget
{
    Q_OBJECT
public:
     AddActorForm(QWidget *parent = nullptr);
     AddActorForm(QWidget *parent = nullptr, Database *data = nullptr, QTableWidget *table = nullptr);
    ~AddActorForm();

     void submitPremadeActor();
     void Initialize();

private slots:
     void on_addCustomActor_pushButton_clicked();

     void on_addPremadeActor_pushButton_clicked();

     void on_cancelAddActorMenu_pushButton_clicked();

     void on_ok_addActor_pushButton_clicked();

     void on_back_addActor_pushButton_clicked();

     void InitializeAddCustom();

     void InitializeAddPremade();

     void on_back_premade_pushButton_clicked();

     void on_ok_premade_pushButton_clicked();

     void SetPremadeFields(Actor currentActor);

     void on_name_premade_comboBox_currentIndexChanged(const QString &arg1);

     void on_scenario_premade_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::AddActorForm *ui;
    Database *db;
    QVector<Actor> *actorList;
    Actor createdActor;
    QTableWidget *combat;
public:
    QPushButton *okButtonCustom;
    QPushButton *okButtonPremade;
};

#endif // ADDACTORFORM_H
