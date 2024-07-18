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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_goto_students_triggered();

    void on_goto_cources_triggered();

    void on_goto_info_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void reactionCourse(int, int);

    void reactionStudent(int, int);

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
