#include "texteditor.h"
#include "ui_texteditor.h"

#include <QTextStream>
#include <QFileDialog>
#include <QApplication>
#include <QMessageBox>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QMdiSubWindow>
#include <QToolBar>
#include <QDesktopWidget>
#include <QFontDialog>
#include <QDateTime>



textEditor::textEditor(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::textEditor),
      help_widget(new QPlainTextEdit),
      changeKeyWidjet(new QPlainTextEdit),
      fileView(new filer(nullptr, ".txt"))

{
    ui->setupUi(this);

    translator = new QTranslator;
    plaintext = new QTextEdit(this);

// создаём меню
    /* выбор языка */
    menuLeng = new QMenu(ui->menubar);

    m_actions[LANG_RU] = new QAction(this);
    m_actions[LANG_EN] = new QAction(this);

    menuLeng->addActions({m_actions.at(LANG_RU), m_actions.at(LANG_EN)});

    /* горячие клавиши */
    menuKey = new QMenu(ui->menubar);

    m_actions[SET_KEY_SAVE] = new QAction(menuKey);
    m_actions[SET_KEY_OPEN] = new QAction(menuKey);
    m_actions[SET_KEY_QUIT] = new QAction(menuKey);
    m_actions[SET_KEY_CLOSE] = new QAction(menuKey);
    m_actions[SHOW_KEYS] = new QAction(menuKey);

    menuKey->addActions({m_actions.at(SHOW_KEYS), m_actions.at(SET_KEY_CLOSE), m_actions.at(SET_KEY_OPEN), m_actions.at(SET_KEY_SAVE), m_actions.at(SET_KEY_QUIT)});

    /* выбор темы */
    menuTheme = new QMenu(ui->menubar);

    m_actions[LIGHT_THEME] = new QAction(menuTheme);
    m_actions[DARK_THEME] = new QAction(menuTheme);
    m_actions[DEFAULT_THEME] = new QAction(menuTheme);

    menuTheme->addActions({m_actions.at(LIGHT_THEME), m_actions.at(DARK_THEME), m_actions.at(DEFAULT_THEME)});

    m_actions[HELP] = new QAction(this);

    ui->menubar->addMenu(menuLeng);
    ui->menubar->addMenu(menuKey);
    ui->menubar->addMenu(menuTheme);
    ui->menubar->addAction(m_actions.at(HELP));

// добавляем tool-bar
    QToolBar* toolbar = new QToolBar(this);

    m_actions[SAVE_FILE] = new QAction(toolbar);
    m_actions[NEW_FILE] =new QAction(toolbar);
    m_actions[PRINT] = new QAction(toolbar);
    m_actions[EXPLOR] = new QAction(toolbar);
    m_actions[COPY_FONT] = new QAction(toolbar);
    m_actions[ALIG_LEFT] = new QAction(toolbar);
    m_actions[ALIG_CENTER] = new QAction(toolbar);
    m_actions[ALIG_RIGHT] = new  QAction(toolbar);
    m_actions[SELECT_ALL_TEXT] = new  QAction(toolbar);
    m_actions[SET_FONT] = new  QAction(toolbar);

    m_actions[DATE] = new QAction(toolbar);
    m_actions[TIME] = new QAction(toolbar);
    m_actions[DATE_TIME] = new QAction(toolbar);


    toolbar->addActions({m_actions.at(SAVE_FILE), m_actions.at(NEW_FILE), m_actions.at(PRINT), m_actions.at(EXPLOR), m_actions.at(COPY_FONT),
                        m_actions.at(ALIG_LEFT), m_actions.at(ALIG_CENTER), m_actions.at(ALIG_RIGHT), m_actions.at(SELECT_ALL_TEXT), m_actions.at(SET_FONT),
                        m_actions.at(DATE_TIME), m_actions.at(DATE), m_actions.at(TIME)});
    toolbar->insertSeparator(m_actions.at(EXPLOR));
    toolbar->insertSeparator(m_actions.at(COPY_FONT));
    toolbar->insertSeparator(m_actions.at(DATE_TIME));

    addToolBar(toolbar);

// устанавливаем коннекты
    connect(m_actions.at(SET_KEY_SAVE), &QAction::triggered, this, &textEditor::onMenuKeyCliced);
    connect(m_actions.at(SET_KEY_OPEN), &QAction::triggered, this, &textEditor::onMenuKeyCliced);
    connect(m_actions.at(SET_KEY_CLOSE), &QAction::triggered, this, &textEditor::onMenuKeyCliced);
    connect(m_actions.at(SET_KEY_QUIT), &QAction::triggered, this, &textEditor::onMenuKeyCliced);
    connect(m_actions.at(SHOW_KEYS), &QAction::triggered, this, &textEditor::onMenuKeyInfo);

    connect(m_actions.at(LANG_RU), &QAction::triggered, this, &textEditor::onMenuLangClicked);
    connect(m_actions.at(LANG_EN), &QAction::triggered, this, &textEditor::onMenuLangClicked);

    connect(m_actions.at(LIGHT_THEME), &QAction::triggered, this, &textEditor::setTheme);
    connect(m_actions.at(DARK_THEME), &QAction::triggered, this, &textEditor::setTheme);
    connect(m_actions.at(DEFAULT_THEME), &QAction::triggered, this, &textEditor::setTheme);

    connect(fileView.get(), SIGNAL(fileSelected(QString)), this, SLOT(filerReturnPath(QString)));

    connect(m_actions.at(HELP), &QAction::triggered, this, &textEditor::on_pushButton_help_clicked);

    connect(m_actions.at(SAVE_FILE), &QAction::triggered, this, &textEditor::on_pushButton_save_clicked);
    connect(m_actions.at(NEW_FILE), &QAction::triggered, this, [this] {
                plaintext = new QTextEdit(this);
                ui->mdiArea->addSubWindow(plaintext);
                plaintext->showMaximized();
            });
    connect(m_actions.at(PRINT), &QAction::triggered, this, &textEditor::on_print_clicked);
    connect(m_actions.at(EXPLOR), &QAction::triggered, this, &textEditor::on_showFiles_clicked);
    connect(m_actions.at(COPY_FONT), &QAction::toggled, this, &textEditor::copy_past_font);

    connect(m_actions.at(ALIG_LEFT), &QAction::triggered, this, &textEditor::setTextAlignment);
    connect(m_actions.at(ALIG_CENTER), &QAction::triggered, this, &textEditor::setTextAlignment);
    connect(m_actions.at(ALIG_RIGHT), &QAction::triggered, this, &textEditor::setTextAlignment);

    connect(m_actions.at(SELECT_ALL_TEXT), &QAction::triggered, this, &textEditor::selectAllText);
    connect(m_actions.at(SET_FONT), &QAction::triggered, this, &textEditor::changeFont);

    connect(m_actions.at(DATE), &QAction::triggered, this, &textEditor::enserDateTime);
    connect(m_actions.at(TIME), &QAction::triggered, this, &textEditor::enserDateTime);
    connect(m_actions.at(DATE_TIME), &QAction::triggered, this, &textEditor::enserDateTime);

// устанавливаем ивент-фильтры
    centralWidget()->installEventFilter(this);
    changeKeyWidjet->installEventFilter(this);

}

textEditor::~textEditor()
{
    m_actions.clear();
    hotKeys.clear();

    delete translator;
}


// меню
void textEditor::on_pushButton_help_clicked()
{
    if(help_widget->isHidden())
        help_widget->show();
    if(help_widget->isTopLevel())
    {
        help_widget->close();
        help_widget->show();
    }
}

void textEditor::onMenuLangClicked()
{
    QObject* obj = sender();

    if(obj->objectName() == Actions::LANG_RU)
    {
        qApp->removeTranslator(translator);
    }
    else if(obj->objectName() == "setEn")
    {
        translator->load(":/QtLanguage_en");
        qApp->installTranslator(translator);
    }

    ui->retranslateUi(this);
    fileView->retranslateUi();
    setText();

}

void textEditor::onMenuKeyCliced()
{
    QObject* obj = sender();
    QString objName = obj->objectName();

    if(objName == "save")
    {
        changeKeyWidjet->setObjectName("save");
        changeKeyWidjet->setWindowTitle(tr("Замена горячей клавиши \"сохраение\""));
    }
    else if(objName == "close")
    {
        changeKeyWidjet->setObjectName("close");
        changeKeyWidjet->setWindowTitle(tr("Замена горячей клавиши \"закрыть документ\""));
    }
    else if(objName == "quit")
    {
        changeKeyWidjet->setObjectName("quit");
        changeKeyWidjet->setWindowTitle(tr("Замена горячей клавиши \"закрыть программу\""));
    }
    else if(objName == "open")
    {
        changeKeyWidjet->setObjectName("open");
        changeKeyWidjet->setWindowTitle(tr("Замена горячей клавиши \"открыть файл\""));
    }

    changeKeyWidjet->show();
}

void textEditor::onMenuKeyInfo()
{
    QMessageBox::information(this, tr("горячие клавиши"), hoKeyList());
}

void textEditor::setTheme()
{
    QString path{ sender()->objectName() };

    if(path == "default")
    {
        qApp->setStyleSheet("");
        return;
    }

    QFile temeFile(path);

    if(temeFile.open(QIODevice::ReadOnly))
    {
        QString temeSettings{temeFile.readAll()};

        qApp->setStyleSheet(temeSettings);

        temeFile.close();
    }
}



// туллбар
void textEditor::on_pushButton_save_clicked()
{
    if(!textEditIsValid())
        return;

    if(plaintext->isReadOnly())
        return;

    QString fileName = QFileDialog::getSaveFileName(this, tr("выберите файл"),
                                                    QDir::homePath(),
                                                    tr("текст (*.txt)"));
    QFile openFile(fileName);

    if(openFile.open(QIODevice::WriteOnly))
    {
        QTextStream writeText(&openFile);
        QString text = plaintext->toPlainText();

        openFile.write(text.toLocal8Bit(), text.length());

        openFile.close();
        plaintext->setWindowTitle(fileName);
    }

}

void textEditor::on_print_clicked()
{
    if(!textEditIsValid())
        return;

    QPrinter printer;
    QPrintDialog dialog(&printer, this);

    if(dialog.exec() == QDialog::Accepted)
       plaintext->print(&printer);

}

void textEditor::on_showFiles_clicked()
{
    if(ui->dockWidget->isHidden())
        ui->dockWidget->show();
    else
        ui->dockWidget->hide();
}

void textEditor::copy_past_font()
{
    if(!textEditIsValid())
        return;

    if(m_actions.at(COPY_FONT)->isChecked())
    {
        currentCharFormat = plaintext->textCursor().charFormat();
    }
    else
    {
        plaintext->textCursor().setCharFormat(currentCharFormat);
    }
}

void textEditor::setTextAlignment()
{
    if(!textEditIsValid())
        return;

    if(sender()->objectName() == "alig_L")
        plaintext->setAlignment(Qt::AlignLeft);
    else if(sender()->objectName() == "alig_C")
        plaintext->setAlignment(Qt::AlignHCenter);
    else if(sender()->objectName() == "alig_R")
        plaintext->setAlignment(Qt::AlignRight);
}

void textEditor::selectAllText()
{
    if(!textEditIsValid())
        return;

    plaintext->selectAll();
}

void textEditor::changeFont()
{
    if(!textEditIsValid())
        return;

    QTextCursor textCursore = plaintext->textCursor();
    QTextCharFormat CharFormat = textCursore.charFormat();
    bool ok{0};

    QFont font = QFontDialog::getFont(&ok, CharFormat.font());

    if(!ok)
        return;

    CharFormat.setFont(font);
    textCursore.setCharFormat(CharFormat);
    plaintext->setTextCursor(textCursore);

}

void textEditor::enserDateTime()
{
    if(!textEditIsValid())
        return;
    if(plaintext->isReadOnly())
        return;
    if(sender()->objectName() == "date")
    {
        plaintext->textCursor().insertText(QDate::currentDate().toString());
    }
    else if(sender()->objectName() == "time")
    {
        plaintext->textCursor().insertText(QTime::currentTime().toString());
    }
    else if(sender()->objectName() == "date_time")
    {
        plaintext->textCursor().insertText(QDateTime::currentDateTime().toString());
    }
}



// кнопки
void textEditor::on_pushButton_quickeSave_clicked()
{
    if(!textEditIsValid())
        return;

    if(plaintext->isReadOnly() | plaintext->windowTitle().isEmpty())
        return;

    QFile openFile(plaintext->windowTitle());

    if(openFile.open(QIODevice::WriteOnly))
    {
        QTextStream writeText(&openFile);
        QString text = plaintext->toPlainText();

        openFile.write(text.toLocal8Bit(), text.length());

        openFile.close();
    }

}

void textEditor::on_pushButton_close_clicked()
{
    if(!textEditIsValid())
        return;

    ui->mdiArea->closeActiveSubWindow();

    delete plaintext;
}

void textEditor::on_pushButton_open_read_only_clicked()
{


    QString fileName = QFileDialog::getOpenFileName(this, tr("выберите файл"),
                                                    QDir::homePath(),
                                                    tr("текст (*.txt)"));
    QFile openFile(fileName);
    if(openFile.open(QIODevice::ReadOnly))
    {
        plaintext = new QTextEdit(this);

        QTextStream readText(&openFile);

        plaintext->setPlainText(readText.readAll());

        openFile.close();

        plaintext->setReadOnly(true);
        plaintext->setWindowTitle(fileName);
        ui->mdiArea->addSubWindow(plaintext);
        plaintext->showMaximized();
    }  
}



// вильтр
bool textEditor::eventFilter(QObject *obj, QEvent *event)
{

    if(event->type() == QEvent::KeyRelease)
    {
        if(obj == changeKeyWidjet.get())
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

             if(keyEvent->modifiers() == Qt::ControlModifier)
             {
                 QString objName = obj->objectName();

                 if(objName == "save")
                 {
                    hotKeys["save"] = keyEvent->key();
                    QMessageBox::information(this, tr("внимание!"), tr("заменя - успешна!"));
                    changeKeyWidjet->hide();
                    return true;
                 }
                 if(objName == "close")
                 {
                     hotKeys["close"] = keyEvent->key();
                     QMessageBox::information(this, tr("внимание!"), tr("заменя - успешна!"));
                     changeKeyWidjet->hide();
                     return true;
                 }
                 if(objName == "quit")
                 {
                     hotKeys["quit"] = keyEvent->key();
                     QMessageBox::information(this, tr("внимание!"), tr("заменя - успешна!"));
                     changeKeyWidjet->hide();
                     return true;
                 }
                 if(objName == "open")
                 {
                     hotKeys["open"] = keyEvent->key();
                     QMessageBox::information(this, tr("внимание!"), tr("заменя - успешна!"));
                     changeKeyWidjet->hide();
                     return true;
                 }
             }
        }

        if(obj == centralWidget())
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

            if(keyEvent->modifiers() == Qt::ControlModifier)
            {

                if(keyEvent->key() == hotKeys.at("save"))
                {
                    textEditor::on_pushButton_save_clicked();
                    return true;
                }
                if(keyEvent->key() == hotKeys.at("close"))

                {
                    on_pushButton_close_clicked();
                    return true;
                }
                if(keyEvent->key() == hotKeys.at("quit"))
                {
                    qApp->exit();
                    return true;
                }
                if(keyEvent->key() == hotKeys.at("print"))
                {
                    on_print_clicked();
                    return true;
                }
            }
        }
    }

    return QObject::eventFilter(obj, event);
}


// функции
void textEditor::setText()
{
    help_widget->setWindowTitle(tr("Справка"));

    ui->pushButton_close->setToolTip(tr("закрыть текущий файл без сохранения изменений"));
    ui->pushButton_open_read_only->setToolTip(tr("открыть файл только для просмотра"));
    ui->pushButton_quickeSave->setToolTip(tr("сохранить изменения в открытом файле"));
    menuLeng->setToolTip(tr("установить язык интерфейса"));
    menuKey->setToolTip(tr("нажмите для замены комбинации клавишь"));
    menuTheme->setToolTip(tr("нажмите для смены темы оформления"));
    m_actions.at(HELP)->setToolTip(tr("открыть текст справки по приложению"));

    menuLeng->setTitle(tr("Язык"));
    menuKey->setTitle(tr("корячие клавиши"));
    menuTheme->setTitle(tr("оформление"));

    m_actions.at(SET_KEY_SAVE)->setText(tr("сохранить как..."));
    m_actions.at(SET_KEY_OPEN)->setText(tr("открыть новый документ"));
    m_actions.at(SET_KEY_QUIT)->setText(tr("закрыть программу"));
    m_actions.at(SET_KEY_CLOSE)->setText(tr("закрыть документ не сохраняя"));
    m_actions.at(SHOW_KEYS)->setText(tr("посмотреть горячие клавиши"));
    m_actions.at(LIGHT_THEME)->setText(tr("светлая тема"));
    m_actions.at(DARK_THEME)->setText(tr("тёмная тема"));
    m_actions.at(DEFAULT_THEME)->setText(tr("стандартная тема"));
    m_actions.at(HELP)->setText(tr("справка"));
    changeKeyWidjet->setPlainText(tr("нажмите CTRL + клавишу для замены"));

    m_actions.at(SAVE_FILE)->setText(tr("сохранить как..."));
    m_actions.at(NEW_FILE)->setText(tr("создать новый файл"));
    m_actions.at(PRINT)->setText(tr("печать документа"));
    m_actions.at(EXPLOR)->setText(tr("открыть/закрыть проводник"));
    m_actions.at(COPY_FONT)->setText(tr("форматирование по образцу"));
    m_actions.at(ALIG_LEFT)->setText(tr("выравнивание по левому краю"));
    m_actions.at(ALIG_CENTER)->setText(tr("выравнивание по центру"));
    m_actions.at(ALIG_RIGHT)->setText(tr("выравнивание по правому краю"));
    m_actions.at(SELECT_ALL_TEXT)->setText(tr("выделить весь текс"));
    m_actions.at(SET_FONT)->setText(tr("выбор шрифта"));
    m_actions.at(DATE)->setText(tr("вставить дату"));
    m_actions.at(TIME)->setText(tr("вставить время"));
    m_actions.at(DATE_TIME)->setText(tr("вставить дату и время"));

}

void textEditor::personalization()
{
    ui->dockWidget->setWidget(fileView.get());
    ui->mdiArea->addSubWindow(plaintext);

    QRect screen = QApplication::desktop()->screenGeometry();

    if(screen.isValid())
    {
        screen.setHeight(screen.height() * 0.75);
        screen.setWidth(screen.width() * 0.75);

        setGeometry(screen);
    }
    else
    {
        resize(800, 600);
    }


    setWindowIcon(QIcon(":/images/icon_cat.png"));

    ui->mdiArea->setBackground(Qt::NoBrush);
    plaintext->showMaximized();

// настраиваем окно справки

    help_widget->setWindowIcon(QIcon(":/images/icon_question.png"));

    QFile fileTXT(":/texts/help.txt");

    if(fileTXT.open(QIODevice::ReadOnly))
    {
        QTextStream readText(&fileTXT);
        help_widget->setPlainText(readText.readAll());

        fileTXT.close();
    }


// настраиваем меню
    m_actions.at(LANG_EN)->setObjectName("setEn");
    m_actions.at(LANG_RU)->setObjectName("setRu");

    m_actions.at(LANG_RU)->setText("Русский");
    m_actions.at(LANG_EN)->setText("English");

    m_actions.at(SET_KEY_SAVE)->setObjectName("save");
    m_actions.at(SET_KEY_OPEN)->setObjectName("open");
    m_actions.at(SET_KEY_QUIT)->setObjectName("quit");
    m_actions.at(SET_KEY_CLOSE)->setObjectName("close");

    m_actions.at(LIGHT_THEME)->setObjectName(":/themes/light_teme.css");
    m_actions.at(DARK_THEME)->setObjectName(":/themes/dark_teme.css");
    m_actions.at(DEFAULT_THEME)->setObjectName("default");

// настраиваем окно подсказки

    changeKeyWidjet->resize(500, 100);
    changeKeyWidjet->setReadOnly(true);
    changeKeyWidjet->setWindowModality(Qt::ApplicationModal);
    changeKeyWidjet->setWindowIcon(style()->standardIcon(QStyle::SP_MessageBoxInformation));


// создаём набор горячих клавишь
    hotKeys["save"] = Qt::Key::Key_S;
    hotKeys["open"] = Qt::Key::Key_O;
    hotKeys["close"] = Qt::Key::Key_N;
    hotKeys["quit"] = Qt::Key::Key_Q;
    hotKeys["print"] = Qt::Key::Key_P;

// устанавливаем иконки для кнопок
    ui->pushButton_close->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    ui->pushButton_quickeSave->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->pushButton_open_read_only->setIcon(style()->standardIcon(QStyle::SP_FileIcon));

// настраиваем бар
    m_actions.at(SAVE_FILE)->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    m_actions.at(NEW_FILE)->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    m_actions.at(PRINT)->setIcon(QPixmap(":/images/Print.ico"));
    m_actions.at(EXPLOR)->setIcon(style()->standardIcon(QStyle::SP_DirIcon));
    m_actions.at(COPY_FONT)->setIcon(style()->standardIcon(QStyle::SP_DialogResetButton));
    m_actions.at(COPY_FONT)->setCheckable(true);

    m_actions.at(ALIG_LEFT)->setIcon(QPixmap(":/images/Text-align-left.ico"));
    m_actions.at(ALIG_CENTER)->setIcon(QPixmap(":/images/Text-align-center.ico"));
    m_actions.at(ALIG_RIGHT)->setIcon(QPixmap(":/images/Text-align-right.ico"));
    m_actions.at(SELECT_ALL_TEXT)->setIcon(QPixmap(":/images/select-AT.ico"));
    m_actions.at(SET_FONT)->setIcon(QPixmap(":/images/Fonts.ico"));

    m_actions.at(ALIG_LEFT)->setObjectName("alig_L");
    m_actions.at(ALIG_CENTER)->setObjectName("alig_C");
    m_actions.at(ALIG_RIGHT)->setObjectName("alig_R");

    m_actions.at(DATE)->setIcon(QPixmap(":/images/date.ico"));
    m_actions.at(TIME)->setIcon(QPixmap(":/images/time.ico"));
    m_actions.at(DATE_TIME)->setIcon(QPixmap(":/images/date_time.ico"));

    m_actions.at(DATE)->setObjectName("date");
    m_actions.at(TIME)->setObjectName("time");
    m_actions.at(DATE_TIME)->setObjectName("date_time");

}

QString textEditor::hoKeyList()
{
    QString text{tr("текущие настройки клавишь:\n")};

    for (const auto& element : hotKeys)
    {
        text += element.first;
        text += tr("  -  клавиша: ");
        text += element.second;
        text += "\n";
    }

    return text;
}

bool textEditor::textEditIsValid()
{
    if(ui->mdiArea->subWindowList().empty())
        return false;

    plaintext = qobject_cast<QTextEdit*>(ui->mdiArea->activeSubWindow()->widget());

    if(!plaintext)
        return false;

    return true;
}



void textEditor::filerReturnPath(const QString& path)
{
    plaintext = new QTextEdit(this);
    ui->mdiArea->addSubWindow(plaintext);
    plaintext->showMaximized();

    QFile openFile(path);
    if(openFile.open(QIODevice::ReadWrite))
    {
        QTextStream readText(&openFile);

        plaintext->setPlainText(readText.readAll());

        openFile.close();

        plaintext->setReadOnly(false);
        plaintext->setWindowTitle(path);
    }

}


