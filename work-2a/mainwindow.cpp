#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    list_model = new QStandardItemModel(this);
    ui->myList->setModel(list_model);

    list_model->appendRow(new QStandardItem(QIcon(":/icons/cpp.png"), "C++"));
    list_model->appendRow(new QStandardItem(QIcon(":/icons/C#.png"), "C#"));
    list_model->appendRow(new QStandardItem(QIcon(":/icons/php.png"), "PHP"));
    list_model->appendRow(new QStandardItem(QIcon(":/icons/python.jpg"), "Python"));

    ui->myList->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->myList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->myList->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->myList->setDragDropMode(QAbstractItemView::InternalMove);

    ui->up->setIcon(QIcon(":/icons/up.jpg"));
    ui->down->setIcon(QIcon(":/icons/down.jpg"));
    ui->newRowText->setPlaceholderText("название языка");
    ui->delete_item->setToolTip("удаление выделенной строки");
    ui->addItem->setToolTip("добавление новой строки");
    ui->up->setToolTip("поднять выделенную строку");
    ui->down->setToolTip("опустить выделенную строку");

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_addItem_clicked()
{
    QString newName = ui->newRowText->text();

    if(!newName.isEmpty())
    {
        QStandardItem* newItem = new QStandardItem(QIcon(":/icons/default.jpg"), newName);
        this->list_model->insertRow(0, newItem);
        ui->newRowText->clear();
    }

}


void MainWindow::on_withIcons_stateChanged()
{
    if(ui->withIcons->isChecked())
    {
        ui->myList->setViewMode(QListView::IconMode);
    }
    else
    {
        ui->myList->setViewMode(QListView::ListMode);
    }
}


void MainWindow::on_delete_item_clicked()
{

    ui->myList->model()->removeRow(
                ui->myList->selectionModel()->currentIndex().row());

}


void MainWindow::on_up_clicked()
{
    int index = ui->myList->selectionModel()->currentIndex().row();

    if(index > 0)
    {
        QStandardItem* row = this->list_model->takeItem(index);
        this->list_model->removeRow(index);
        this->list_model->insertRow(--index, row);
        ui->myList->setCurrentIndex(this->list_model->index(index, 0));
    }

}


void MainWindow::on_down_clicked()
{
    int index = ui->myList->selectionModel()->currentIndex().row();

    if(index < (this->list_model->rowCount() - 1) && index >= 0)
    {
        QStandardItem* row = this->list_model->takeItem(index);
        this->list_model->removeRow(index);
        this->list_model->insertRow(++index, row);
        ui->myList->setCurrentIndex(this->list_model->index(index, 0));
    }

}

