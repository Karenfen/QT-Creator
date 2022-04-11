#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myModel = new QStandardItemModel(this);

    myModel->appendRow({new QStandardItem("home PC"),
                        new QStandardItem("192.168.1.52"),
                        new QStandardItem("00:26:57:00:1f:02")});
    myModel->appendRow({new QStandardItem("work PC"),
                        new QStandardItem("172.11.0.132"),
                        new QStandardItem("c4:85::08:e1:67:ed")});
    myModel->appendRow({new QStandardItem("kid PC"),
                        new QStandardItem("192.168.1.53"),
                        new QStandardItem("08:00:27:3f:fe:83")});
    myModel->appendRow({new QStandardItem("PC-152"),
                        new QStandardItem("205.5.64.137"),
                        new QStandardItem("00:1f:16:95:11:04")});

    myModel->setHeaderData(0, Qt::Orientation::Horizontal, "PC name");
    myModel->setHeaderData(1, Qt::Orientation::Horizontal, "IP address");
    myModel->setHeaderData(2, Qt::Orientation::Horizontal, "MAC address");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::start()
{
    this->setMinimumSize(500, 300);

    ui->myTable->setModel(myModel);
    ui->myTable->resize(500,250);
    ui->myTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->myTable->setColumnWidth(0, 150);
    ui->myTable->setColumnWidth(1, 150);
    ui->myTable->setColumnWidth(2, 150);

    ui->painting->setText("покрасить");
    ui->painting->setToolTip("покрасить строки в жёлтый");
    ui->painting->move(0, 250);

    ui->defaultColor->setText("сбросить");
    ui->defaultColor->setToolTip("отменить покраску строк");
    ui->defaultColor->move(75, 250);
}


void MainWindow::on_painting_clicked()
{
    if(ui->myTable->selectionModel()->hasSelection())
    {
        QModelIndexList selectedIndexes = ui->myTable->selectionModel()->selectedIndexes();
        for (auto pIndex : selectedIndexes)
        {
            myModel->itemFromIndex(pIndex)->setData(QColor(Qt::yellow), Qt::BackgroundColorRole);
        }
    }
}


void MainWindow::on_defaultColor_clicked()
{
    if(ui->myTable->selectionModel()->hasSelection())
    {
        QModelIndexList selectedIndexes = ui->myTable->selectionModel()->selectedIndexes();
        for (auto pIndex : selectedIndexes)
        {
            myModel->itemFromIndex(pIndex)->setData(QColor(Qt::white), Qt::BackgroundColorRole);
        }
    }
}

