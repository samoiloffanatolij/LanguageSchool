#include "course_management.cpp"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->tableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(reactionCourse(int,int)));
    connect(ui->tableWidget_2, SIGNAL(cellClicked(int,int)), this, SLOT(reactionStudent(int,int)));
    QIcon winIcon("C:/Qt/projects/languages_school/s.jpg");
    this->setWindowIcon(winIcon);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_goto_students_triggered()
{
    ui->tableWidget_2->setRowCount(0);
    vector<string> students = cms.getStudents();
    for (int i = 0; i < (int)students.size(); ++i) {
        ui->tableWidget_2->setRowCount(ui->tableWidget_2->rowCount() + 1);
        ui->tableWidget_2->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(students[i])));
    }
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_goto_cources_triggered()
{
    ui->tableWidget->setRowCount(0);
    for (int i = 0; i < cms.getNumGroups() && cms.getNumStudentsOfGroup(i) >= 5; ++i) {
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem("Group " + QString::number(i + 1)));
    }
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_goto_info_triggered()
{
    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_pushButton_clicked()
{
    modeling = Modeling(
                        ui->lineEdit->text().toInt(),
                        1 - ui->textEdit_2->toPlainText().toDouble(),
                        1 - ui->textEdit->toPlainText().toDouble()
                       );
    ui->textBrowser_43->setText("0");
    ui->textBrowser_45->setText("0");
    ui->textBrowser_47->setText("0");
    ui->textBrowser_49->setText("0");
    ui->textBrowser_51->setText("0");
    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_pushButton_2_clicked()
{
    if (modeling.modelingNextStep())
    {
        ui->textBrowser_4->setText(QString::number(modeling.getDate().first));
        ui->textBrowser_6->setText(QString::number(modeling.getDate().second));
        ui->textBrowser_43->setText(QString::number(cms.getNumPeople()));
        ui->textBrowser_45->setText(QString::number(cms.getNumGroups()));
        ui->textBrowser_47->setText(QString::number(cms.averageNumGroup()));
        ui->textBrowser_49->setText(QString::number(cms.averageCostStudent()));
        ui->textBrowser_51->setText(QString::number(cms.averageCostGroup()));
        ui->stackedWidget->setCurrentIndex(4);
    }
    else
    {
        vector<double> res = modeling.result();
        ui->textBrowser_10->setText(QString::number((int)res[0]));
        ui->textBrowser_12->setText(QString::number((int)res[1]));
        ui->textBrowser_14->setText(QString::number(res[2]));
        ui->textBrowser_38->setText(QString::number(res[3]));
        ui->textBrowser_40->setText(QString::number(res[4]));
        ui->stackedWidget->setCurrentIndex(3);
    }

}


void MainWindow::reactionCourse(int row, int col) {
    auto group_info = cms.getGroupInfo(row), students = cms.getStudentsOfGroup(row);
    ui->textBrowser_17->setText(QString::fromStdString(group_info[0]));
    ui->textBrowser_19->setText(QString::fromStdString(group_info[1]));
    ui->textBrowser_21->setText(QString::fromStdString(group_info[2]));
    ui->textBrowser_23->setText(QString::fromStdString(group_info[3]));
    ui->textBrowser_25->setText(QString::fromStdString(group_info[4]));
    ui->textBrowser_34->setText("");
    for (string &student : students)
        ui->textBrowser_34->setText(ui->textBrowser_34->toPlainText() + QString::fromStdString(student) + "\n");
    ui->stackedWidget->setCurrentIndex(5);
}


void MainWindow::reactionStudent(int row, int col) {
    auto student_info = cms.getStudentInfo(row);
    auto cources = cms.getStudentCources(row);
    ui->textBrowser_28->setText(QString::fromStdString(student_info.first));
    ui->textBrowser_36->setText(QString::fromStdString(student_info.second));
    ui->textBrowser_30->setText("");
    ui->textBrowser_32->setText("");
    for (int group_ind : cources.second)
        ui->textBrowser_32->setText(ui->textBrowser_32->toPlainText() + QString::number(group_ind) + " ");
    for (auto &course : cources.first) {
        for (string &s : course)
            ui->textBrowser_30->setText(ui->textBrowser_30->toPlainText() + QString::fromStdString(s) + " ");
        ui->textBrowser_30->setText(ui->textBrowser_30->toPlainText() + "\n");
    }
    ui->stackedWidget->setCurrentIndex(6);
}


void MainWindow::on_pushButton_3_clicked()
{
    ui->lineEdit->setText("");
    ui->textEdit->setText("");
    ui->textEdit_2->setText("");
    cms = CourseManagementSystem();
    ui->stackedWidget->setCurrentIndex(0);
}

