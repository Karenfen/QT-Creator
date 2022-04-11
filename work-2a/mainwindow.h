#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class QStandardItemModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addItem_clicked();

    void on_withIcons_stateChanged();

    void on_delete_item_clicked();

    void on_up_clicked();

    void on_down_clicked();

private:
    Ui::MainWindow *ui;
    QStandardItemModel* list_model;
};
#endif // MAINWINDOW_H
