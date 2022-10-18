#include "../includes/maintranslatorwin.h"
#include "forms/ui_maintranslatorwin.h"
#include "clipboardchkr.h"

MainTranslatorWin::MainTranslatorWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainTranslatorWin)
{
    ui->setupUi(this);

    ui->translateButton->setEnabled(false);
    QObject::connect(ui->translateButton, SIGNAL(clicked()),
                     this, SLOT(findInputWord()));

    QObject::connect(ui->translateButton, &QPushButton::clicked,
                     this, [&]() {ui->translateButton->setEnabled(false);});

    QObject::connect(ui->inputTextEdit, SIGNAL(textChanged()),
                     this, SLOT(makeTranslateAvailable()));

    QObject::connect(this, &MainTranslatorWin::wordIsNotFound,
                     this, [&](){
                                    ui->outputTextEdit->setStyleSheet("QPlainTextEdit {color: red;}");
                                    ui->outputTextEdit->setPlainText("notfound");
                                    //ui->outputTextEdit->setStyleSheet("QPlainTextEdit {color: black;}");
                                });

    translator.initTheDict("dict.xdxf");                //inite the dictionary

    ClipboardChecker clipChkr;
    clipChkr.testClipboard();

}

MainTranslatorWin::~MainTranslatorWin()
{
    delete ui;
}

void MainTranslatorWin::translateInputWord(const pugi::xml_node& found_node) {
    ui->outputTextEdit->setStyleSheet("QPlainTextEdit {color: black;}");
    ui->outputTextEdit->setPlainText("word is found");
    auto node = found_node.parent();                //Come back to the word's tag
    node = node.next_sibling();                     //The next node will be a transcription or a translation (if there is not transcription)

    QMap<QString, QString> translationResult;       //index - name of parameter, value - transcription/translation

    while(node != nullptr) {
        if(node.type() == pugi::xml_node_type::node_element) {                      //if we encounter transcription tag <tr>
            std::cout << "Transcription tag name: " << node.name() << std::endl;
            std::cout << "Transcription: " << node.child_value() << std::endl;
            translationResult["translation"] = node.child_value();                  //Save the value of the child_node (the transcription itself)
        }
        else if(node.type() == pugi::xml_node_type::node_pcdata) {                  //If we encounter translation
            std::cout << "Translation: " << node.value() << std::endl;
            translationResult["transcription"] = node.value();
        }
        node = node.next_sibling();
    }
    ui->outputTextEdit->setPlainText(translationResult["translation"]);
    ui->outputTextEdit->appendPlainText(translationResult["transcription"]);
}

void MainTranslatorWin::makeTranslateAvailable() {
    if(ui->inputTextEdit->toPlainText() == "")
        ui->translateButton->setEnabled(false);
    else
        ui->translateButton->setEnabled(true);
}

void MainTranslatorWin::findInputWord() {
    //ui->outputTextEdit->setPlainText("input change is detected");
    current_word = ui->inputTextEdit->toPlainText();
    std::cout << "The word is detected in input field: " << current_word.toStdString().c_str() << std::endl;
    pugi::xml_node found_node = translator.findTheSingleWorld(current_word.toStdString().c_str());
    if(found_node == nullptr) {
        emit wordIsNotFound();
    }
    else
        translateInputWord(found_node);
}



