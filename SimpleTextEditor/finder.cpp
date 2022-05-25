#include "finder.h"

Finder::Finder(const QString& name, const QString& path, QObject* parent) : QObject(parent)
{
    fileName = name;
    homePath = path;
    cansel = false;
}


void Finder::searchInDir(const QString& dir)
{
    if(cansel)
        return;

    QDir currentDir = QDir(dir);

    QStringList files = currentDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Readable | QDir::Writable, QDir::DirsLast);

    if(files.isEmpty())
        return;

    for(const QString& file : files)
    {
        QString path = currentDir.filePath(file);

        if(file.indexOf(fileName) != -1)
            emit foundFile(file, path);

        if(path != QDir::homePath())
            searchInDir(path);
    }
}

void Finder::setCansel()
{
    cansel = true;
}

void Finder::doSearch()
{
    searchInDir(QDir::homePath());
    searchInDir(homePath);
}


Controller::Controller(QObject* parent) : QObject(parent)
{
    pool = QThreadPool::globalInstance();
}


Controller::~Controller()
{
    emit canselThreads();
    pool->clear();
    pool->waitForDone(300);
}


void Controller::start(const QString &file)
{
    if(QSysInfo::productType() == "windows")
    {
        QFileInfoList drives = QDir::drives();

        for(auto drive : drives)
        {
            runwork* workerRuner = new runwork(file, drive.filePath());
            pool->start(workerRuner, QThread::HighPriority);

            connect(workerRuner->worker(), &Finder::foundFile, this, &Controller::foundFile);
            connect(this, &Controller::canselThreads, workerRuner->worker(), &Finder::setCansel);
        }
    }
    else
    {
        runwork* workerRuner = new runwork(file, "/");
        pool->start(workerRuner, QThread::HighPriority);

        connect(workerRuner->worker(), &Finder::foundFile, this, &Controller::foundFile);
        connect(this, &Controller::canselThreads, workerRuner->worker(), &Finder::setCansel);
    }
}


void Controller::foundFile(const QString& fileName, const QString& filePath)
{
    emit sendResult(fileName, filePath);
}


runwork::runwork(const QString &fileName, const QString &filePath)
{
    obj_work = new Finder(fileName, filePath);
}

void runwork::run()
{
    obj_work->doSearch();
}

runwork::~runwork()
{
    delete obj_work;
}

Finder *runwork::worker()
{
    return obj_work;
}
