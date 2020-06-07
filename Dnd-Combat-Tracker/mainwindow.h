#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum  pages {WELCOME, EDIT, ASSIGN, COMBAT, DB_EDIT};

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //************ Welcome Page *********************
    void on_welcomeStart_pushButton_clicked();
        void on_dbOpt_welcome_pushButton_clicked();

    //************ Edit Page *********************
    void on_back_editPage_pushButton_clicked();

    void on_next_editPage_pushButton_clicked();
       void on_dbOpt_editPage_pushButton_clicked();

    //************ Assign Page *********************
    void on_back_assignInit_pushButton_clicked();

    void on_fight_assignInit_pushButton_clicked();

    //************ Combat Page *********************
    void on_endCombat_pushButton_clicked();

    //************ DB Edit Page *********************
    void on_mainMenu_pushButton_clicked();

    void on_combatEditor_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
