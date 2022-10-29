#include "../includes/maintranslatorwin.h"
#include "forms/ui_maintranslatorwin.h"


class QPlainTextEditModif: public QPlainTextEdit {
public:
    QPlainTextEditModif(QObject* parent = nullptr);


};

MainTranslatorWin::MainTranslatorWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainTranslatorWin)
    , clipboard(QApplication::clipboard())
{
    ui->setupUi(this);

    ui->translateButton->setEnabled(false);



    QObject::connect(ui->translateButton, &QPushButton::clicked,                        //toggle translate button
                     this, [&]() {ui->translateButton->setEnabled(false);});

    QObject::connect(ui->inputLineEdit, SIGNAL(textChanged()),                          //togge translat button
                     this, SLOT(makeTranslateAvailable()));

    ui->inputLineEdit->installEventFilter(this);                                        //install the event filter to
                                                                                        //incept enter pressing


    wordsReceiver = new WordsReceiver(this);                                            //define the words receiver
                                                                                        //which stays in the main thread
    wordsFinderThread = new WordsFinderThread(wordsReceiver, "dict.xdxf", this);        //define the words finder thread

    QObject::connect(clipboard, &QClipboard::dataChanged,                               //bind the clipboard with the words receiver
                     this, [&](){
                                current_word = clipboard->text();                       //read the current word from the clipboard
                                ui->inputLineEdit->setText(current_word);          //set a word from the clipboard as an input word
                                emit wordsReceiver->wordIsReceived(current_word);
                            });


    QObject::connect(wordsReceiver, &WordsReceiver::sendResultOut,                  //bind words receiver with GUI
                     this, &MainTranslatorWin::printResultTranslation);

    QObject::connect(wordsReceiver, &WordsReceiver::sendErrorNotification,
                     this, &MainTranslatorWin::printError);


    QObject::connect(ui->translateButton, &QPushButton::clicked,                    //bind translate button with the word receiver
                     wordsReceiver, [&](){
                                            current_word = ui->inputLineEdit->text();
                                            emit wordsReceiver->wordIsReceived(current_word);
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

    return false;                                                       //in the other cases the event goes to textEditor
}


WordsFinderThread* MainTranslatorWin::getWordsFinderThread() {
    return wordsFinderThread;
}

MainTranslatorWin::~MainTranslatorWin()
{
    //delete clipboardThread;
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

void MainTranslatorWin::printError(const QString& err_message) {
    ui->outputTextEdit->setStyleSheet("QPlainTextEdit {color: red;}");
    ui->outputTextEdit->setPlainText(err_message);

    QTextCursor textCursor = ui->outputTextEdit->textCursor();
    textCursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    ui->outputTextEdit->setTextCursor(textCursor);
}




