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

    m_actions["setRu"] = new QAction(this);
    m_actions["setEn"] = new QAction(this);

    menuLeng->addActions({m_actions.at("setRu"), m_actions.at("setEn")});

    /* горячие клавиши */
    menuKey = new QMenu(ui->menubar);

    m_actions["setKeySave"] = new QAction(this);
    m_actions["setKeyOpen"] = new QAction(this);
    m_actions["setKeyQuit"] = new QAction(this);
    m_actions["setKeyClose"] = new QAction(this);
    m_actions["showKeys"] = new QAction(this);

    menuKey->addActions({m_actions.at("showKeys"), m_actions.at("setKeyClose"), m_actions.at("setKeyOpen"), m_actions.at("setKeySave"), m_actions.at("setKeyQuit")});

    /* выбор темы */
    menuTheme = new QMenu(ui->menubar);

    m_actions["lightTheme"] = new QAction(this);
    m_actions["darkTheme"] = new QAction(this);
    m_actions["default"] = new QAction(this);

    menuTheme->addActions({m_actions.at("lightTheme"), m_actions.at("darkTheme"), m_actions.at("default")});

    m_actions["help"] = new QAction(this);

    ui->menubar->addMenu(menuLeng);
    ui->menubar->addMenu(menuKey);
    ui->menubar->addMenu(menuTheme);
    ui->menubar->addAction(m_actions.at("help"));

// добавляем tool-bar
    QToolBar* toolbar = new QToolBar{};

    m_actions["bar_save"] = new QAction(this);
    m_actions["bar_nfile"] =new QAction(this);
    m_actions["bar_print"] = new QAction(this);
    m_actions["bar_expl"] = new QAction(this);
    m_actions["copy_font"] = new QAction(this);
    m_actions["alig_L"] = new QAction(this);
    m_actions["alig_C"] = new QAction(this);
    m_actions["alig_R"] = new  QAction(this);
    m_actions["select_AT"] = new  QAction(this);
    m_actions["set_font"] = new  QAction(this);


    toolbar->addActions({m_actions.at("bar_save"), m_actions.at("bar_nfile"), m_actions.at("bar_print"), m_actions.at("bar_expl"), m_actions.at("copy_font"),
                        m_actions.at("alig_L"), m_actions.at("alig_C"), m_actions.at("alig_R"), m_actions.at("select_AT"), m_actions.at("set_font")});
    toolbar->insertSeparator(m_actions.at("bar_expl"));
    toolbar->insertSeparator(m_actions.at("copy_font"));

    addToolBar(toolbar);

// устанавливаем коннекты
    connect(m_actions.at("setKeySave"), &QAction::triggered, this, &textEditor::onMenuKeyCliced);
    connect(m_actions.at("setKeyOpen"), &QAction::triggered, this, &textEditor::onMenuKeyCliced);
    connect(m_actions.at("setKeyClose"), &QAction::triggered, this, &textEditor::onMenuKeyCliced);
    connect(m_actions.at("setKeyQuit"), &QAction::triggered, this, &textEditor::onMenuKeyCliced);
    connect(m_actions.at("showKeys"), &QAction::triggered, this, &textEditor::onMenuKeyInfo);

    connect(m_actions.at("setRu"), &QAction::triggered, this, &textEditor::onMenuLangClicked);
    connect(m_actions.at("setEn"), &QAction::triggered, this, &textEditor::onMenuLangClicked);

    connect(m_actions.at("lightTheme"), &QAction::triggered, this, &textEditor::setTheme);
    connect(m_actions.at("darkTheme"), &QAction::triggered, this, &textEditor::setTheme);
    connect(m_actions.at("default"), &QAction::triggered, this, &textEditor::setTheme);

    connect(fileView.get(), SIGNAL(fileSelected(QString)), this, SLOT(filerReturnPath(QString)));

    connect(m_actions.at("help"), &QAction::triggered, this, &textEditor::on_pushButton_help_clicked);

    connect(m_actions.at("bar_save"), &QAction::triggered, this, &textEditor::on_pushButton_save_clicked);
    connect(m_actions.at("bar_nfile"), &QAction::triggered, this, [this] {
                plaintext = new QTextEdit(this);
                ui->mdiArea->addSubWindow(plaintext);
                plaintext->showMaximized();
            });
    connect(m_actions.at("bar_print"), &QAction::triggered, this, &textEditor::on_print_clicked);
    connect(m_actions.at("bar_expl"), &QAction::triggered, this, &textEditor::on_showFiles_clicked);
    connect(m_actions.at("copy_font"), &QAction::toggled, this, &textEditor::copy_past_font);

    connect(m_actions.at("alig_L"), &QAction::triggered, this, &textEditor::setTextAlignment);
    connect(m_actions.at("alig_C"), &QAction::triggered, this, &textEditor::setTextAlignment);
    connect(m_actions.at("alig_R"), &QAction::triggered, this, &textEditor::setTextAlignment);

    connect(m_actions.at("select_AT"), &QAction::triggered, this, &textEditor::selectAllText);
    connect(m_actions.at("set_font"), &QAction::triggered, this, &textEditor::changeFont);

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

    if(obj->objectName() == "setRu")
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

    if(m_actions.at("copy_font")->isChecked())
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
    m_actions.at("help")->setToolTip(tr("открыть текст справки по приложению"));

    menuLeng->setTitle(tr("Язык"));
    menuKey->setTitle(tr("корячие клавиши"));
    menuTheme->setTitle(tr("оформление"));

    m_actions.at("setKeySave")->setText(tr("сохранить как..."));
    m_actions.at("setKeyOpen")->setText(tr("открыть новый документ"));
    m_actions.at("setKeyQuit")->setText(tr("закрыть программу"));
    m_actions.at("setKeyClose")->setText(tr("закрыть документ не сохраняя"));
    m_actions.at("showKeys")->setText(tr("посмотреть горячие клавиши"));
    m_actions.at("lightTheme")->setText(tr("светлая тема"));
    m_actions.at("darkTheme")->setText(tr("тёмная тема"));
    m_actions.at("default")->setText(tr("стандартная тема"));
    m_actions.at("help")->setText(tr("справка"));
    changeKeyWidjet->setPlainText(tr("нажмите CTRL + клавишу для замены"));

    m_actions.at("bar_save")->setText(tr("сохранить как..."));
    m_actions.at("bar_nfile")->setText(tr("создать новый файл"));
    m_actions.at("bar_print")->setText(tr("печать документа"));
    m_actions.at("bar_expl")->setText(tr("открыть/закрыть проводник"));
    m_actions.at("copy_font")->setText(tr("форматирование по образцу"));
    m_actions.at("alig_L")->setText(tr("выравнивание по левому краю"));
    m_actions.at("alig_C")->setText(tr("выравнивание по центру"));
    m_actions.at("alig_R")->setText(tr("выравнивание по правому краю"));
    m_actions.at("select_AT")->setText(tr("выделить весь текс"));
    m_actions.at("set_font")->setText(tr("выбор шрифта"));
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
    m_actions.at("setEn")->setObjectName("setEn");
    m_actions.at("setRu")->setObjectName("setRu");

    m_actions.at("setRu")->setText("Русский");
    m_actions.at("setEn")->setText("English");

    m_actions.at("setKeySave")->setObjectName("save");
    m_actions.at("setKeyOpen")->setObjectName("open");
    m_actions.at("setKeyQuit")->setObjectName("quit");
    m_actions.at("setKeyClose")->setObjectName("close");

    m_actions.at("lightTheme")->setObjectName(":/themes/light_teme.css");
    m_actions.at("darkTheme")->setObjectName(":/themes/dark_teme.css");
    m_actions.at("default")->setObjectName("default");

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
    m_actions.at("bar_save")->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    m_actions.at("bar_nfile")->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    m_actions.at("bar_print")->setIcon(QPixmap(":/images/Print.ico"));
    m_actions.at("bar_expl")->setIcon(style()->standardIcon(QStyle::SP_DirIcon));
    m_actions.at("copy_font")->setIcon(style()->standardIcon(QStyle::SP_DialogResetButton));
    m_actions.at("copy_font")->setCheckable(true);

    m_actions.at("alig_L")->setIcon(QPixmap(":/images/Text-align-left.ico"));
    m_actions.at("alig_C")->setIcon(QPixmap(":/images/Text-align-center.ico"));
    m_actions.at("alig_R")->setIcon(QPixmap(":/images/Text-align-right.ico"));
    m_actions.at("select_AT")->setIcon(QPixmap(":/images/select-AT.ico"));
    m_actions.at("set_font")->setIcon(QPixmap(":/images/Fonts.ico"));

    m_actions.at("alig_L")->setObjectName("alig_L");
    m_actions.at("alig_C")->setObjectName("alig_C");
    m_actions.at("alig_R")->setObjectName("alig_R");

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

    fileView->refresh();
}


