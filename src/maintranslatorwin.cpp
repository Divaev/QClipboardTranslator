#include "../includes/maintranslatorwin.h"
#include "forms/ui_maintranslatorwin.h"


MainTranslatorWin::MainTranslatorWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainTranslatorWin)
    , clipboard(QApplication::clipboard())
    , settings("settings.ini", QSettings::IniFormat)
    , wakeUpGlobalShortcut(new QGlobalShortcut)
    , currentWordStatus(WordStatus::TRANSLATED) {

    this->setWindowFlags(Qt::WindowStaysOnTopHint);

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
    wordsFinderThread = new WordsFinderThread(wordsReceiver, settings.value("loading_dict_path").toString(), this);        //define the words finder thread

    QObject::connect(clipboard, &QClipboard::dataChanged,                               //bind the clipboard with the words receiver
                     this, [&](){
                                if (currentWordStatus == WordStatus::TRANSLATED ||
                                        currentWordStatus == WordStatus::ERROR) {              //if the previous translation request is processed
                                        current_word = clipboard->text();                       //read the current word from the clipboard
                                        while (current_word == "") {
                                             current_word = clipboard->text();
                                        }

                                        ui->inputLineEdit->setText(current_word);          //set a word from the clipboard as an input word
                                        emit wordsReceiver->wordIsReceived(current_word);
                                        currentWordStatus = WordStatus::SENT;
                                }
                            });


    QObject::connect(wordsReceiver, &WordsReceiver::sendResultOut,                  //bind words receiver with GUI
                     this, &MainTranslatorWin::printResultTranslation);

    QObject::connect(wordsReceiver, &WordsReceiver::sendErrorNotification,
                     this, &MainTranslatorWin::processError);

    QObject::connect(wordsReceiver, &WordsReceiver::dictionaryIsReady,
                     this, [this]() {
                                ui->inputLineEdit->clear();
                                ui->translateButton->setEnabled(true);
                                ui->inputLineEdit->setEnabled(true);
                                ui->inputLineEdit->setStyleSheet("QPlainTextEdit {color: black;}");
                                ui->inputLineEdit->clear();
                                ui->translateButton->setEnabled(false);
                                ui->outputTextEdit->clear();
                                ui->inputLineEdit->setFocus();
                                emit sendDictStatus(ActualDictStatus::OK);
                           });


    QObject::connect(ui->translateButton, &QPushButton::clicked,                    //bind translate button with the word receiver
                     wordsReceiver, [&](){
                                            current_word = ui->inputLineEdit->text();
                                            emit wordsReceiver->wordIsReceived(current_word);
                                     });


    dictionaryDialog = new SelectDictionaryDialog(&settings, this);

    QObject::connect(dictionaryDialog, &SelectDictionaryDialog::dictHasBeenChosen,
                     this, [this]() {
                                emit sendDictStatus(ActualDictStatus::UNKNOWN);
                                emit wordsReceiver->setDictPath(settings.value("loading_dict_path").toString());
                                ui->outputTextEdit->setStyleSheet("QPlainTextEdit {color: black;}");
                                ui->outputTextEdit->setPlainText("Dictionary is loading..");
                                ui->inputLineEdit->setStyleSheet("QPlainTextEdit {color: red;}");
                                ui->inputLineEdit->setText("Dictionary is loading..");
                                ui->inputLineEdit->setEnabled(false);
                                ui->translateButton->setEnabled(false);
                            });

    QObject::connect(dictionaryDialog, &SelectDictionaryDialog::dictHasBeenDeleted,
                     this, [this](const QString& msg, const bool& block_flag) {
                                emit wordsReceiver->unsetDictPath();
                                processError(msg, block_flag);

                            });

    QObject::connect(dictionaryDialog, &SelectDictionaryDialog::dictErrorThrow,
                     this, &MainTranslatorWin::processError);

    QObject::connect(wordsReceiver, &WordsReceiver::dictErrorThrow,
                     this, [this](const QString& msg, const bool& block_flag) {
                                processError(msg, block_flag);
                                emit sendDictStatus(ActualDictStatus::ERROR);
                            });

    QObject::connect(this, &MainTranslatorWin::sendDictStatus,
                     dictionaryDialog, &SelectDictionaryDialog::applyDictStatus);



    QObject::connect(ui->actionSetDictionary, &QAction::triggered,
                     this, [this]() {
                                dictionaryDialog->show();
                                dictionaryDialog->initMainDictParameters();
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
    currentWordStatus = WordStatus::TRANSLATED;
    ui->inputLineEdit->setFocus();
}

void MainTranslatorWin::processError(const QString& err_message, const bool& block_input) {
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
    currentWordStatus = WordStatus::ERROR;
}




