#ifndef FILERW_H
#define FILERW_H

#include <QObject>
#include <QFile>
#include <QDate>


class FileRW : public QObject
{
    Q_OBJECT
public:
    explicit FileRW(QObject *parent = nullptr);
    ~FileRW();
    Q_INVOKABLE QList<QStringList> read();
    Q_INVOKABLE void write(const QStringList& data);

private:
    QFile* file;

};



#endif // FILERW_H
