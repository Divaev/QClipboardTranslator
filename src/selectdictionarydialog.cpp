#include "selectdictionarydialog.h"
#include "forms/ui_selectdictionarydialog.h"

SelectDictionaryDialog::SelectDictionaryDialog(QSettings *set_ptr, QWidget *parent)
    : settings_ptr(set_ptr)
    , QDialog(parent)
    , ui(new Ui::SelectDictionaryDialog)
{
    ui->setupUi(this);

    refreshDictListWidget();

    addDictDialog = new QFileDialog(this);

    QObject::connect(ui->addDictButton, &QPushButton::clicked,
                     this, [this]() {

                                addDictDialog->setViewMode(QFileDialog::Detail);
                                addDictDialog->setFileMode(QFileDialog::ExistingFiles);
                                addDictDialog->setNameFilter("Dictionaries (*.xdxf)");

                                QStringList addDictNames;
                                if (addDictDialog->exec()) {
                                    addDictNames = addDictDialog->selectedFiles();
                                    dictNames.append(addDictNames);
                                }

                                refreshDictListWidget();
                            });


    QObject::connect(ui->dictOkCancelBox, &QDialogButtonBox::accepted,
                     this, [this]() {
                                int currentRow = ui->dictListWidget->currentRow();
                                currentDict = dictNames[currentRow];
                                settings_ptr->setValue("current_dict_path", currentDict);
                                emit this->dictHasBeenChosen();
                            });

}

void SelectDictionaryDialog::refreshDictListWidget() {
    auto genShortNames = [](const QStringList& fullPaths) {
        QStringList shortNames;
        std::transform(fullPaths.begin(),  fullPaths.end(),
                       std::back_inserter(shortNames),
                       [](const QString& fullPath) {
                            QString shortName = fullPath.mid(fullPath.lastIndexOf('/') + 1);
                            shortName = shortName.left(shortName.lastIndexOf('.'));
                            return shortName;
                        });
        return shortNames;
    };

    dictNames.sort();
    QStringList shortDictNames = genShortNames(dictNames);
    shortDictNames.append("dict1");
    ui->dictListWidget->clear();
    ui->dictListWidget->addItems(shortDictNames);

    currentDict = "dict1";

    QList<QListWidgetItem*> selectedDicts = ui->dictListWidget->findItems(currentDict, Qt::MatchFixedString);

    ui->dictListWidget->setCurrentItem(*(selectedDicts.begin()));

}


SelectDictionaryDialog::~SelectDictionaryDialog()
{
    delete ui;
}
