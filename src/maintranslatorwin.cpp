#include "../includes/maintranslatorwin.h"
#include "forms/ui_maintranslatorwin.h"


MainTranslatorWin::MainTranslatorWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainTranslatorWin)
    , clipboard(QApplication::clipboard())
    , settings("settings.ini", QSettings::IniFormat)
    , wakeUpGlobalShortcut(new QGlobalShortcut) {

    SYSTEMTIME st, lt;

    GetSystemTime(&st);
    GetLocalTime(&lt);

    printf("The system time is: %02d:%02d\n", st.wHour, st.wMinute);
    printf(" The local time is: %02d:%02d\n", lt.wHour, lt.wMinute);
    qDebug() << "The system time: " << QString::number(st.wHour);

    this->setWindowFlags(Qt::WindowStaysOnTopHint);

    settings.setValue("current_dict_path", "dict.xdxf");

    ui->setupUi(this);

    ui->translateButton->setEnabled(false);

    wakeUpGlobalShortcut->setKey(QKeySequence("Alt+Ctrl+M"));              //set the global hot key

    QObject::connect(wakeUpGlobalShortcut, &QGlobalShortcut::activated,
                                            this, [this]() {
                                                qDebug() << "Shortkey is detected!";
                                                this->activateWindow();
                                            });



    QObject::connect(ui->translateButton, &QPushButton::clicked,                        //toggle translate button
                     this, [&]() {ui->translateButton->setEnabled(false);});


    QObject::connect(ui->inputLineEdit, SIGNAL(textChanged(QString)),                          //togge translat button
                     this, SLOT(makeTranslateAvailable()));

    ui->inputLineEdit->installEventFilter(this);                                        //install the event filter to
                                                                                        //incept enter pressing


    wordsReceiver = new WordsReceiver(this);                                            //define the words receiver
                                                                                        //which stays in the main thread
    wordsFinderThread = new WordsFinderThread(wordsReceiver, settings.value("current_dict_path").toString(), this);        //define the words finder thread

    QObject::connect(clipboard, &QClipboard::dataChanged,                               //bind the clipboard with the words receiver
                     this, [&](){
                                current_word = clipboard->text();                       //read the current word from the clipboard
                                ui->inputLineEdit->setText(current_word);          //set a word from the clipboard as an input word
                                emit wordsReceiver->wordIsReceived(current_word);
                            });


    QObject::connect(wordsReceiver, &WordsReceiver::sendResultOut,                  //bind words receiver with GUI
                     this, &MainTranslatorWin::printResultTranslation);

    QObject::connect(wordsReceiver, &WordsReceiver::sendErrorNotification,
                     this, &MainTranslatorWin::proceedError);

    QObject::connect(wordsReceiver, &WordsReceiver::dictionaryIsReady,
                     this, [this]() {
                                ui->inputLineEdit->clear();
                                ui->translateButton->setEnabled(true);
                                ui->inputLineEdit->setEnabled(true);
                                ui->inputLineEdit->setStyleSheet("QPlainTextEdit {color: black;}");
                                ui->inputLineEdit->clear();
                                ui->translateButton->setEnabled(false);
                                ui->outputTextEdit->clear();
                           });


    QObject::connect(ui->translateButton, &QPushButton::clicked,                    //bind translate button with the word receiver
                     wordsReceiver, [&](){
                                            current_word = ui->inputLineEdit->text();
                                            emit wordsReceiver->wordIsReceived(current_word);
                                     });


    dictionaryDialog = new SelectDictionaryDialog(&settings, this);

    QObject::connect(dictionaryDialog, &SelectDictionaryDialog::dictHasBeenChosen,
                     this, [this]() {
                                emit wordsReceiver->setDictPath(settings.value("current_dict_path").toString());
                                qDebug() << "new dictionary has been chosen!";
                                qDebug() << "The new dict is " << settings.value("current_dict_path").toString();
                            });

    QObject::connect(dictionaryDialog, &SelectDictionaryDialog::dictErrorThrow,
                     this, &MainTranslatorWin::proceedError);

    QObject::connect(ui->actionSetDictionary, &QAction::triggered,
                     this, [this]() {
                                dictionaryDialog->show();
                                dictionaryDialog->refreshDictListWidget();
                           });
}

void MainTranslatorWin::closeEvent(QCloseEvent* event) {
    emit wordsReceiver->stopWordsFinderThread();
}

bool MainTranslatorWin::eventFilter(QObject* watched, QEvent* event) {
    if (watched == ui->inputLineEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = dynamic_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Return) {                        //we intercept enter pressing
            emit ui->translateButton->clicked();                        //emit the signale
            return true;                                                //and block this event
        }
    }
    else if (event->type() == QEvent::WindowActivate) {
        ui->inputLineEdit->setFocus();
        return  true;
    }

    return false;                                                       //in the other cases the event goes to textEditor
}


WordsFinderThread* MainTranslatorWin::getWordsFinderThread() {
    return wordsFinderThread;
}

MainTranslatorWin::~MainTranslatorWin()
{
    delete ui;
}


void MainTranslatorWin::makeTranslateAvailable() {
    if(ui->inputLineEdit->text() == "")
        ui->translateButton->setEnabled(false);
    else
        ui->translateButton->setEnabled(true);
}


void MainTranslatorWin::printResultTranslation(const QMap<QString, QString> &result) {
    ui->outputTextEdit->setStyleSheet("QPlainTextEdit {color: black;}");

    ui->outputTextEdit->setPlainText(result["transcription"]);
    ui->outputTextEdit->appendPlainText(result["translation"]);

    QTextCursor textCursor = ui->outputTextEdit->textCursor();
    textCursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    ui->outputTextEdit->setTextCursor(textCursor);
}

void MainTranslatorWin::proceedError(const QString& err_message, const bool& block_input) {
    ui->outputTextEdit->setStyleSheet("QPlainTextEdit {color: red;}");
    ui->outputTextEdit->setPlainText(err_message);
    if (block_input) {
        ui->inputLineEdit->setEnabled(false);
        ui->inputLineEdit->setStyleSheet("QPlainTextEdit {color: red;}");
        ui->inputLineEdit->setText(err_message);
        ui->translateButton->setEnabled(false);

    }

    QTextCursor textCursor = ui->outputTextEdit->textCursor();
    textCursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    ui->outputTextEdit->setTextCursor(textCursor);
}




