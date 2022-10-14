#include "../includes/maintranslatorwin.h"
#include "forms/ui_maintranslatorwin.h"

MainTranslatorWin::MainTranslatorWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainTranslatorWin)
{
    ui->setupUi(this);
}

MainTranslatorWin::~MainTranslatorWin()
{
    delete ui;
}

