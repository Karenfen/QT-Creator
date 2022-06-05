#include "mainwindow.h"
#include <QVBoxLayout>
#include <QQuickItem>
#include <QTableView>







MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    qmlRegisterType<FileRW>("com.FileRW", 1, 0, "FileRW");

    quickeWidget = new QQuickWidget(QUrl("qrc:/gui.qml"));
    quickeWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    tasksModel = new QStandardItemModel(this);

    tasksModel->setColumnCount(3);
    tasksModel->setHeaderData(0, Qt::Orientation::Horizontal, tr("Дата"));
    tasksModel->setHeaderData(1, Qt::Orientation::Horizontal, tr("Задание"));
    tasksModel->setHeaderData(2, Qt::Orientation::Horizontal, tr("Прогресс"));

    auto table = new QTableView(this);
    table->setModel(tasksModel);


    table->setColumnWidth(0, width()/5);
    table->setColumnWidth(1, width()/2);
    table->setColumnWidth(2, width()/7);

    auto layout = new QVBoxLayout{};

    layout->addWidget(quickeWidget);
    layout->addWidget(table);

    setLayout(layout);

    auto root = quickeWidget->rootObject();
    if(root)
    {
        auto addButton = root->findChild<QObject*>("addButton");
        if(addButton)
        {
            connect(addButton, SIGNAL(createdTask(QString, QString, QString)), this, SLOT(newTask(QString, QString, QString)) );
        }
    }

    loadTask();
}

MainWindow::~MainWindow()
{
    for(auto task : taskList)
    {
        RW.write(QStringList{ task.Date(), task.Status(), task.Text()});
    }

    taskList.clear();
}

void MainWindow::loadTask()
{
    auto tasks = RW.read();

    if(tasks.isEmpty())
        return;

    for(auto task : tasks)
    {
        tasksModel->appendRow(QList<QStandardItem*>{new QStandardItem{task.at(0)}, new QStandardItem{task.at(2)}, new QStandardItem{task.at(1)}});
    }


}

void MainWindow::newTask(const QString &text, const QString &date, const QString &progress)
{
    taskList.append(Task{text, date, progress});
    tasksModel->appendRow(QList<QStandardItem*>{new QStandardItem{date}, new QStandardItem{text}, new QStandardItem{progress}});
}

