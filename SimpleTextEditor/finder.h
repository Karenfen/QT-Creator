#ifndef FINDER_H
#define FINDER_H

#include <QObject>
#include <QDir>
#include <QThreadPool>
#include <QListWidget>
#include <QRunnable>


class Finder : public QObject
{
    Q_OBJECT
public:
    explicit Finder(const QString& name, const QString& path, QObject* parent = nullptr);
    void doSearch();

private:
    void searchInDir(const QString& dir);

public slots:
    void setCansel();

signals:
    void foundFile(QString, QString);

private:
    QString fileName;
    QString homePath;
    bool cansel;

};


class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject* parent = nullptr);
    ~Controller();
    void start(const QString& file);

public slots:
    void foundFile(const QString& fileName, const QString& filePath);

signals:
    void sendResult(QString, QString);
    void canselThreads();

private:
    QThreadPool* pool;

};




class runwork : public QRunnable
{
public:
    runwork(const QString& fileName, const QString& homePath);
    void run() override;
    ~runwork();
    Finder *worker();

private:
    Finder* obj_work;
};


#endif // FINDER_H
