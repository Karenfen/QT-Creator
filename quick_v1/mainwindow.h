#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QQuickWidget>
#include <QStandardItemModel>
#include "filerw.h"


class Task
{
public:
    Task(QString nText, QString nDate, QString nStatus):
        text(nText), deadLine(nDate), status(nStatus) {}
    ~Task(){}

    QString Text() { return text; }
    QString Date() { return deadLine; }
    QString Status() { return status; }

private:
    QString text;
    QString deadLine;
    QString status;
};



class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void loadTask();

public slots:
    void newTask(const QString &text, const QString &date, const QString &progress);

signals:
    void loadedTask(Task task);

private:
    FileRW RW;
    QQuickWidget* quickeWidget;
    QList<Task> taskList;
    QStandardItemModel* tasksModel;

};
#endif // MAINWINDOW_H
