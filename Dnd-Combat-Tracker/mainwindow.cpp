#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = new Database("../itdb.db", "QSQLITE");

    ui->main_stackedWidget->setCurrentIndex(WELCOME);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_welcomeStart_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(EDIT);
}

void MainWindow::on_back_editPage_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(WELCOME);
}

void MainWindow::on_next_editPage_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(ASSIGN);
}

void MainWindow::on_back_assignInit_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(EDIT);
}

void MainWindow::on_fight_assignInit_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(COMBAT);
}

void MainWindow::on_endCombat_pushButton_clicked()
{
    ui->main_stackedWidget->setCurrentIndex(WELCOME);
}
