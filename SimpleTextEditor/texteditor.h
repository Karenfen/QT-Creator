#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QMainWindow>
#include <QTranslator>
#include <memory>
#include <QTextCharFormat>

#include "filer.h"


QT_BEGIN_NAMESPACE
namespace Ui { class textEditor; }
QT_END_NAMESPACE

class QPlainTextEdit;
class QTextEdit;


class textEditor : public QMainWindow
{
    Q_OBJECT

public:
    textEditor(QWidget *parent = nullptr);
    ~textEditor();

private slots:
// кнопки
    void on_pushButton_save_clicked();
    void on_pushButton_close_clicked();
    void on_pushButton_open_read_only_clicked();
// меню
    void onMenuLangClicked();
    void onMenuKeyCliced();
    void onMenuKeyInfo();
    void on_pushButton_help_clicked();
    void setTheme();
// проводгик
    void filerReturnPath(const QString& path);
// туллбар
    void on_print_clicked();
    void on_pushButton_quickeSave_clicked();
    void on_showFiles_clicked();
    void copy_past_font();
    void setTextAlignment();
    void selectAllText();
    void changeFont();
    void enserDateTime();

private:
    enum Actions{SAVE_FILE, NEW_FILE, PRINT, EXPLOR, COPY_FONT, ALIG_LEFT, ALIG_CENTER, ALIG_RIGHT, SELECT_ALL_TEXT, SET_FONT, DATE, TIME, DATE_TIME,
                LANG_RU, LANG_EN, SET_KEY_SAVE, SET_KEY_OPEN, SET_KEY_QUIT, SET_KEY_CLOSE, SHOW_KEYS, LIGHT_THEME, DARK_THEME, DEFAULT_THEME, HELP };

    std::unique_ptr<Ui::textEditor> ui;
    std::unique_ptr<QPlainTextEdit> help_widget;
    std::unique_ptr<QPlainTextEdit> changeKeyWidjet;
    std::unique_ptr<filer> fileView;


    QTextEdit* plaintext;

    QMenu* menuLeng;
    QMenu* menuKey;
    QMenu* menuTheme;

    QTranslator* translator;


    std::map<QString, int>hotKeys;
    std::map<Actions, QAction*> m_actions;

    QTextCharFormat currentCharFormat;

private:

    QString hoKeyList();
    bool textEditIsValid();


public:

    void setText();
    void personalization();

protected:

    bool eventFilter(QObject* obj, QEvent* event) override;
};
#endif // TEXTEDITOR_H
