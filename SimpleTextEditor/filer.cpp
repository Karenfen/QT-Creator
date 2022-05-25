#include "filer.h"
#include <QClipboard>
#include <QAbstractItemModel>
#include <QAction>



filer::filer(QWidget *parent, const QString& filter) :
    QWidget(parent), ui(new Ui_Form), m_model(new QFileSystemModel), list(nullptr)
{
    ui->setupUi(this);

    ui->comboBoxType->addItem(filter);
    ui->comboBoxType->setItemText(0, filter);

    m_model->setRootPath(QDir::homePath());
    ui->treeView->setModel(m_model.get());

    ui->treeView->setColumnWidth(0, 300);

//    resize(860, 480);
    setWindowModality(Qt::WindowModality::ApplicationModal);
    setWindowIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    setWindowTitle(tr("Проводник"));

    connect(ui->treeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &filer::selectionChanged);
}

filer::~filer()
{
    list->close();
    delete list;
}


void filer::retranslateUi()
{
    ui->retranslateUi(this);
}


void filer::selectionChanged()
{
    if(ui->treeView->selectionModel()->hasSelection())
    {
        QModelIndex index = ui->treeView->selectionModel()->currentIndex();
        QString path = m_model->filePath(index);
        ui->currentPath->setText(path);
    }
}


void filer::on_open_clicked()
{
    if(!ui->treeView->selectionModel()->hasSelection())
        return;

    QString path;
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();

    if(!m_model->isDir(index))
    {
        path = ui->currentPath->text();
        emit fileSelected(path);
    }
    else if(m_model->isDir(index) & !ui->fileName->text().isEmpty())
    {
        path = ui->currentPath->text() + "/" + ui->fileName->text() + ui->comboBoxType->currentText();
        emit fileSelected(path);
    }

}


void filer::on_copyPath_clicked()
{
    QClipboard* clipboard = qApp->clipboard();
    clipboard->setText(ui->currentPath->text());
}

void filer::on_search_clicked()
{
    if(ui->fileName->text().isEmpty())
        return;

    if(list)
    {
        delete list;
    }

    list = new searchResults();
    list->setWindowModality(Qt::ApplicationModal);
    list->resize(this->size());
    list->setWindowTitle(tr("поиск..."));
    list->setWindowIcon(windowIcon());
    list->show();

    connect(list, &searchResults::itemDoubleClicked, this, &filer::selectFile);
    connect(&threadControl, &Controller::sendResult, this, &filer::addResult);
    connect(list, &searchResults::closed, &threadControl, &Controller::canselThreads);

    threadControl.start(ui->fileName->text());

}

void filer::addResult(const QString& fileName, const QString& filePath)
{
    if(list != nullptr)
    {
        QListWidgetItem* item = new QListWidgetItem(list);
        item->setText(fileName);
        item->setToolTip(filePath);
        list->addItem(item);
    }
}

void filer::selectFile(QListWidgetItem* item)
{
    emit fileSelected(item->toolTip());
    list->close();
    list->deleteLater();
    list = nullptr;
}


void searchResults::closeEvent(QCloseEvent *event)
{
    emit closed();

    QListWidget::closeEvent(event);
}
