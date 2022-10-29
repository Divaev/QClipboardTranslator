#include "selectdictionarydialog.h"
#include "forms/ui_selectdictionarydialog.h"

SelectDictionaryDialog::SelectDictionaryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectDictionaryDialog)
{
    ui->setupUi(this);
}

SelectDictionaryDialog::~SelectDictionaryDialog()
{
    delete ui;
}
