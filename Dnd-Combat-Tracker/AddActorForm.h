#ifndef ADDACTORFORM_H
#define ADDACTORFORM_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class AddActorForm; }
QT_END_NAMESPACE

class AddActorForm : public QWidget
{
    Q_OBJECT
public:
     AddActorForm(QWidget *parent = nullptr);
    ~AddActorForm();

private:
    Ui::AddActorForm *ui;
};

#endif // ADDACTORFORM_H
