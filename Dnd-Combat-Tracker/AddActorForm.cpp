#include "AddActorForm.h"
#include "ui_addactorform.h"

AddActorForm::AddActorForm(QWidget *parent)
    : QWidget(parent), ui(new Ui::AddActorForm)
{
    ui->setupUi(this);
    setWindowModality(Qt::WindowModality::WindowModal);


}

AddActorForm::~AddActorForm()
{
    delete ui;
}
