#include "selectdictionarydialog.h"
#include "forms/ui_selectdictionarydialog.h"

SelectDictionaryDialog::SelectDictionaryDialog(QSettings *set_ptr, QWidget *parent)
    : settings_ptr(set_ptr)
    , QDialog(parent)
    , ui(new Ui::SelectDictionaryDialog)
    , loadingErrorIcon(QPixmap(":/icons/dict_load_error.png"))
    , customDelegate(new CustomDelegate(this))
{
    ui->setupUi(this);

    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    dictNames = settings_ptr->value("list_of_dictionaries").value<QStringList>();       //load the dict names and the current dict
    loadedDictName = settings_ptr->value("loaded_dict_path").toString();                  //from the settings

    selectedDictName = "";

    refreshDictListWidget();                                                            //refresh dictListWidget after loading from the settings

    addDictDialog = new QFileDialog(this);
    addDictDialog->setWindowModality(Qt::WindowModal);
    addDictDialog->resize(320,240);
    addDictDialog->setViewMode(QFileDialog::List);

    QObject::connect(ui->addDictButton, &QPushButton::clicked,
                     this, [this]() {

                                addDictDialog->setFileMode(QFileDialog::ExistingFiles);
                                addDictDialog->setNameFilter("Dictionaries (*.xdxf)");

                                QStringList addDictNames;
                                if (addDictDialog->exec()) {                              //execution of the dict choosing dialog
                                    addDictNames = addDictDialog->selectedFiles();
                                    if (!addDictNames.empty()) {
                                        setOkButton(true);
                                        setDeleteButton(true);
                                        //----------Temporary code--------------
                                        if (addDictNames.count() == 1 &&
                                                !dictNames.contains(addDictNames[0])) {
                                            //currentDict = addDictNames[0];
                                            selectedDictName = addDictNames[0];
                                        }
                                        //---------------------------------------

                                        dictNames.append(addDictNames);                       //add chosen dialogs to the existing pool
                                        std::sort(dictNames.begin(), dictNames.end());
                                        auto lastName = std::unique(dictNames.begin(), dictNames.end());
                                        dictNames.erase(lastName, dictNames.end());
                                    }
                                }

                                refreshDictListWidget();                                //refresh dictListWidget after adding new dictionaries
                            });

    QObject::connect(ui->delDictButton, &QPushButton::clicked,
                     this, [this]() {
                                int currentRow = ui->dictListWidget->currentRow();              //receive the number of the selected row
                                if (dictNames.at(currentRow) == loadedDictName)
                                    loadedDictName = "";
                                if (dictNames.at(currentRow) == selectedDictName)
                                    selectedDictName = "";

                                dictNames.removeAt(currentRow);                                 //delete this row

                                settings_ptr->setValue("list_of_dictionaries", QVariant::fromValue(dictNames));
                                settings_ptr->setValue("loaded_dict_path", loadedDictName);

                                refreshDictListWidget();                                        //refresh dictListWidget after removing of a dictionary

                           });


    QObject::connect(ui->dictOkCancelBox, &QDialogButtonBox::accepted,
                     this, [this]() {


                                //int loadedRow = ui->dictListWidget->currentRow();              //selected row in the widget is the current logic row
                                //loadedDictName = dictNames[loadedRow];                           //assing currentDict as the dict that has currentRow index in the
                                                                                            //dictNames QStringList
                                if (!dictNames.empty()) {
                                    int selectedRow = ui->dictListWidget->currentRow();
                                    loadedDictName = dictNames[selectedRow];
                                }
                                settings_ptr->setValue("list_of_dictionaries", QVariant::fromValue(dictNames));
                                settings_ptr->setValue("loaded_dict_path", loadedDictName);

                                if (!dictNames.empty()) {
                                    emit this->dictHasBeenChosen();
                                }

                             });
    QObject::connect(ui->dictOkCancelBox, &QDialogButtonBox::rejected,
                     this, [this]() {
                                dictNames = settings_ptr->value("list_of_dictionaries").value<QStringList>();       //load the dict names and the current dict
                                loadedDictName = settings_ptr->value("loaded_dict_path").toString();                //from the settings
                                if (loadedDictName == "") {
                                    loadedDictName = settings_ptr->value("loaded_dict_path").toString();                //from the settings
                                    if (!dictNames.empty())
                                        emit dictHasBeenDeleted(QString("No dictionary is selected"));
                                    else
                                        emit dictHasBeenDeleted(QString("There is no added dictionaries!"));
                                }
                            });

}

void SelectDictionaryDialog::refreshDictListWidget() {

    //-----short names making lambda-------------------
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
    //-------------------------------------------------

    dictNames.sort();

    int loadedRow = dictNames.indexOf(loadedDictName);
    int selectedRow = dictNames.indexOf(selectedDictName);

    QStringList shortDictNames = genShortNames(dictNames);

    ui->dictListWidget->clear();
    ui->dictListWidget->addItems(shortDictNames);

    /*
    QList<QListWidgetItem*> selectedDicts = ui->dictListWidget->findItems(currentDict, Qt::MatchFixedString);

    if(!selectedDicts.empty()) {
        ui->dictListWidget->setCurrentItem(*(selectedDicts.begin()));
        settings_ptr->setValue("loaded_dict_path", currentDict);
    }
    */

    for (int row = 0; row < dictNames.count(); ++row) {
        QListWidgetItem* item = ui->dictListWidget->item(row);
        item->setToolTip(dictNames[row]);
    }

    if (loadedRow > -1) {
        ui->dictListWidget->item(loadedRow)->setBackgroundColor(Qt::green);
    }
    else if (!dictNames.empty()) {
        //ui->dictListWidget->setCurrentRow(0);
    }
    else {
        loadedDictName = "";
        setOkButton(false);
        setDeleteButton(false);
        emit dictErrorThrow(QString("There is no added dictionaries!"));
    }

    if (selectedRow != -1)
        ui->dictListWidget->setCurrentRow(selectedRow);
    else if (!dictNames.empty()) {
        ui->dictListWidget->setCurrentRow(0);
        ui->dictListWidget->item(0)->setIcon(loadingErrorIcon);

        ui->dictListWidget->setItemDelegate(customDelegate);

    }



    if (!dictNames.empty())
        settings_ptr->setValue("list_of_dictionaries", QVariant::fromValue(dictNames));
}


void SelectDictionaryDialog::setOkButton(const bool& state) {
    ui->dictOkCancelBox->button(QDialogButtonBox::Ok)->setEnabled(state);
}


void SelectDictionaryDialog::setDeleteButton(const bool& state) {
    ui->delDictButton->setEnabled(state);
}

void SelectDictionaryDialog::closeEvent(QCloseEvent *event) {
    emit  ui->dictOkCancelBox->rejected();
}
SelectDictionaryDialog::~SelectDictionaryDialog()
{
    delete ui;
}


CustomDelegate::CustomDelegate(QObject* parent): QStyledItemDelegate(parent) {

}

void CustomDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const {
    QStyleOptionViewItem iconsInDictListOption(option);
    iconsInDictListOption.decorationPosition = QStyleOptionViewItem::Right;
    QStyledItemDelegate::paint(painter, iconsInDictListOption, index);
}
