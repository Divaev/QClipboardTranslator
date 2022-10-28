#include "../includes/maintranslatorwin.h"
#include "forms/ui_maintranslatorwin.h"


MainTranslatorWin::MainTranslatorWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainTranslatorWin)
    , clipboard(QApplication::clipboard())
{
    ui->setupUi(this);

    ui->translateButton->setEnabled(false);



    QObject::connect(ui->translateButton, &QPushButton::clicked,                        //toggle translate button
                     this, [&]() {ui->translateButton->setEnabled(false);});

    QObject::connect(ui->inputTextEdit, SIGNAL(textChanged()),                          //togge translat button
                     this, SLOT(makeTranslateAvailable()));



    wordsReceiver = new WordsReceiver(this);                                            //define the words receiver
                                                                                        //which stays in the main thread
    wordsFinderThread = new WordsFinderThread(wordsReceiver, "dict.xdxf", this);        //define the words finder thread

    QObject::connect(clipboard, &QClipboard::dataChanged,                               //bind the clipboard with the words receiver
                     this, [&](){
                                current_word = clipboard->text();                       //read the current word from the clipboard
                                ui->inputTextEdit->setPlainText(current_word);          //set a word from the clipboard as an input word
                                emit wordsReceiver->wordIsReceived(current_word);
                            });


    QObject::connect(wordsReceiver, &WordsReceiver::sendResultOut,                  //bind words receiver with GUI
                     this, &MainTranslatorWin::printResultTranslation);


    QObject::connect(ui->translateButton, &QPushButton::clicked,                    //bind translate button with the word receiver
                     wordsReceiver, [&](){
                                            current_word = ui->inputTextEdit->toPlainText();
                                            emit wordsReceiver->wordIsReceived(current_word);
                                     });

}

void MainTranslatorWin::closeEvent(QCloseEvent* event) {
    emit wordsReceiver->stopWordsFinderThread();
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
    if(ui->inputTextEdit->toPlainText() == "")
        ui->translateButton->setEnabled(false);
    else
        ui->translateButton->setEnabled(true);
}


void MainTranslatorWin::printResultTranslation(const QMap<QString, QString> &result) {
    ui->outputTextEdit->setPlainText(result["transcription"]);
    ui->outputTextEdit->appendPlainText(result["translation"]);

    if(result["translation"] == "") {
        ui->outputTextEdit->setStyleSheet("QPlainTextEdit {color: red;}");
        ui->outputTextEdit->setPlainText("notfound");
    }

    QTextCursor textCursor = ui->outputTextEdit->textCursor();
    textCursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    ui->outputTextEdit->setTextCursor(textCursor);
}




