#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmath.h"
#include "string"





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ==================================================

void MainWindow::on_task1_pushButton_solve_clicked()
{
    double answer;

    if (ui->task1_ensert_a->value() == 0)
    {
        if (ui->task1_ensert_b->value() == 0)
        {
            ui->task1_answer_x1->setText("-");
            ui->task1_answer_x2->setText("-");
        }
        else
        {
            answer = 0 - ui->task1_ensert_c->value() / ui->task1_ensert_b->value();

            if(qFuzzyIsNull(answer))  // приравниваю слишком маленькие значения к нулю, чтобы не было в выводе -0.000000
                answer = 0;

            ui->task1_answer_x1->setText(std::to_string(answer).c_str());
            ui->task1_answer_x2->setText("-");
        }
    }
    else
    {
        int D { static_cast<int>(qPow(ui->task1_ensert_b->value(), 2)) - 4 * ui->task1_ensert_a->value() * ui->task1_ensert_c->value() };

        if (D < 0)
        {
            ui->task1_answer_x1->setText("-");
            ui->task1_answer_x2->setText("-");
        }
        else if (D == 0)
        {
            answer = (0 - ui->task1_ensert_b->value()) / (2 * ui->task1_ensert_a->value()); // не знаю как эфективнее менять знак у числа ( 0-value или value*(-1) )

            if(qFuzzyIsNull(answer))
                answer = 0;

            ui->task1_answer_x1->setText(std::to_string(answer).c_str());
            ui->task1_answer_x2->setText("-");
        }
        else if ( D > 0 )
        {
            answer = (qSqrt(D) - ui->task1_ensert_b->value() ) / (2 * ui->task1_ensert_a->value()); // каст в int для того, чтобы не было отрицательного нуля в ответе

            if(qFuzzyIsNull(answer))
                answer = 0;

            ui->task1_answer_x1->setText(std::to_string(answer).c_str());

            answer = (0 - ui->task1_ensert_b->value() - qSqrt(D)) / (2 * ui->task1_ensert_a->value());

            if(qFuzzyIsNull(answer))
                answer = 0;

            ui->task1_answer_x2->setText(std::to_string(answer).c_str());
        }
    }
}


void MainWindow::on_task1_pushButton_clear_clicked()
{
    ui->task1_ensert_a->setValue(0);
    ui->task1_ensert_b->setValue(0);
    ui->task1_ensert_c->setValue(0);
    ui->task1_answer_x1->clear();
    ui->task1_answer_x2->clear();
}

// ==================================================

void MainWindow::on_task2_pushButton_clear_clicked()
{
    ui->task2_doubleSpinBox_A->setValue(0);
    ui->task2_doubleSpinBox_B->setValue(0);
    ui->task2_doubleSpinBox_y->setValue(0);
    ui->task2_lineEdit_answer->clear();
}


void MainWindow::on_task2_pushButton_solve_clicked()
{
    double y;
    double a = ui->task2_doubleSpinBox_A->value();
    double b = ui->task2_doubleSpinBox_B->value();
    double c;

    if(ui->task2_radioButton_deg->isChecked())
    {
        y = qDegreesToRadians(ui->task2_doubleSpinBox_y->value());
    }
    else
    {
        y = ui->task2_doubleSpinBox_y->value();
    }

    c = qSqrt(qPow(a, 2) + qPow(b, 2) - 2 * a * b * qCos(y));

    ui->task2_lineEdit_answer->setText(std::to_string(c).c_str());
}

// ==================================================

void MainWindow::on_task3_pushButton_ensertHTML_clicked()
{
    ui->task3_plainTextEdit2->appendHtml("<font color='red'>Hello</font>");
}


void MainWindow::on_task3_pushButton_clearT2_clicked()
{
    ui->task3_plainTextEdit2->clear();
}


void MainWindow::on_task3_pushButton_replace_clicked()
{
    ui->task3_plainTextEdit2->clear();
    ui->task3_plainTextEdit2->appendPlainText(ui->task3_plainTextEdit1->toPlainText());
}


void MainWindow::on_task3_pushButton_add_clicked()
{
    ui->task3_plainTextEdit2->appendPlainText(ui->task3_plainTextEdit1->toPlainText());
}


void MainWindow::on_task3_pushButton_clearT1_clicked()
{
     ui->task3_plainTextEdit1->clear();
}

