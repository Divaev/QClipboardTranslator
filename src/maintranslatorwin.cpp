#include "../includes/maintranslatorwin.h"
#include "forms/ui_maintranslatorwin.h"

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

}

MainTranslatorWin::~MainTranslatorWin()
{
    delete ui;
}

void MainTranslatorWin::translateInputWord(const pugi::xml_node& node) {
    ui->outputTextEdit->setStyleSheet("QPlainTextEdit {color: black;}");
    ui->outputTextEdit->setPlainText("word is found");

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



