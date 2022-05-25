#ifndef FILER_H
#define FILER_H

#include <QWidget>
#include <QListWidget>
#include <QFileSystemModel>
#include "ui_filer.h"
#include "finder.h"
#include <memory>


class searchResults;


class filer : public QWidget
{
    Q_OBJECT

public:
    filer(QWidget *parent = nullptr, const QString& filter = "");
    ~filer();

    void retranslateUi();

signals:
    void fileSelected(QString);

private slots:

    void selectionChanged();

    void on_open_clicked();
    void on_copyPath_clicked();
    void on_search_clicked();

    void addResult(const QString& fileName, const QString& filePath);
    void selectFile(QListWidgetItem* item);

private:
    std::unique_ptr<Ui_Form> ui;
    std::unique_ptr<QFileSystemModel> m_model;
    searchResults* list;
    Controller threadControl;

};


class searchResults : public QListWidget
{
    Q_OBJECT

protected:
    void closeEvent(QCloseEvent* event) override;

signals:
    void closed();
};

#endif // FILER_H
