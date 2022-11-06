#include "selectdictionarydialog.h"
#include "forms/ui_selectdictionarydialog.h"

SelectDictionaryDialog::SelectDictionaryDialog(QSettings *set_ptr, QWidget *parent)
    : settings_ptr(set_ptr)
    , QDialog(parent)
    , ui(new Ui::SelectDictionaryDialog)
    , loadingErrorIcon(QPixmap(":/icons/dict_load_error.png"))
    , loadingSuccessIcon(":/icons/dict_load_ok.png")
    , loadingUnknownIcon(":/icons/dict_load_unknown.png")
    , customDelegate(new CustomDelegate(this))
    , actualDictStatus("", ActualDictStatus::UNKNOWN)
{
    ui->setupUi(this);

    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);             //delete question button on the top of the window

    dictNamesInitial = settings_ptr->value("list_of_dictionaries").value<QStringList>();       //load the dict names and the current dict
    actualDictNameInitial = settings_ptr->value("loading_dict_path").toString();                  //from the settings

    dictNames = dictNamesInitial;
    actualDictName = actualDictNameInitial;

    selectedDictName = actualDictName;

    actualDictStatus = {actualDictName, ActualDictStatus::UNKNOWN};

    refreshDictListWidget();                                                            //refresh dictListWidget after loading from the settings

    addDictDialog = new QFileDialog(this);
    addDictDialog->setWindowModality(Qt::WindowModal);
    addDictDialog->resize(320,240);
    addDictDialog->setViewMode(QFileDialog::List);

    ui->dictListWidget->setItemDelegate(customDelegate);



    QObject::connect(ui->addDictButton, &QPushButton::clicked,
                     this, [this]() {

                                addDictDialog->setFileMode(QFileDialog::ExistingFiles);
                                addDictDialog->setNameFilter("Dictionaries (*.xdxf)");

                                QStringList addDictNames;
                                if (addDictDialog->exec()) {                              //execution of the dict choosing dialog
                                    addDictNames = addDictDialog->selectedFiles();
                                    if (!addDictNames.empty()) {
                                        //setOkButton(true);
                                        setDeleteButton(true);
                                        setSelectButton(true);
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
                                if (dictNames.at(currentRow) == actualDictName)
                                    actualDictName = "";
                                if (dictNames.at(currentRow) == selectedDictName)
                                    selectedDictName = "";

                                dictNames.removeAt(currentRow);                                 //delete this row

                                settings_ptr->setValue("list_of_dictionaries", QVariant::fromValue(dictNames));
                                settings_ptr->setValue("loading_dict_path", actualDictName);

                                refreshDictListWidget();                                        //refresh dictListWidget after removing of a dictionary

                           });

    QObject::connect(ui->selectDictButton, &QPushButton::clicked,
                     this, [this]() {
                                //int selectedRow = dictNames.indexOf(selectedDictName);
                                int selectedRow = ui->dictListWidget->currentRow();
                                selectedDictName = dictNames[selectedRow];
                                chooseSelectedDict();
                                refreshDictListWidget();

                             });


    QObject::connect(ui->dictOkCancelBox, &QDialogButtonBox::accepted,
                     this, [this]() {
                                if (!dictNames.empty()) {
                                    int selectedRow = ui->dictListWidget->currentRow();
                                    selectedDictName = dictNames[selectedRow];
                                    if (actualDictStatus.first != selectedDictName)
                                        chooseSelectedDict();                                     //dictNames QStringList
                                }


                            });
    QObject::connect(ui->dictOkCancelBox, &QDialogButtonBox::rejected,
                     this, [this]() {

                                /*
                                dictNames = settings_ptr->value("list_of_dictionaries").value<QStringList>();       //load the dict names and the current dict
                                actualDictName = settings_ptr->value("loading_dict_path").toString();                //from the settings
                                if (actualDictName == "") {
                                    actualDictName = settings_ptr->value("loading_dict_path").toString();                //from the settings
                                    if (!dictNames.empty())
                                        emit dictHasBeenDeleted(QString("No dictionary is selected"));
                                    else
                                        emit dictHasBeenDeleted(QString("There is no added dictionaries!"));
                                }
                                */
                                QString previousDictName = settings_ptr->value("loading_dict_path").toString();

                                dictNames = dictNamesInitial;
                                actualDictName = actualDictNameInitial;
                                selectedDictName = actualDictName;
                                settings_ptr->setValue("list_of_dictionaries", QVariant::fromValue(dictNames));
                                settings_ptr->setValue("loading_dict_path", actualDictName);

                                refreshDictListWidget();

                                if (!dictNames.empty() && previousDictName != actualDictName) {
                                    emit this->dictHasBeenChosen();
                                }
                                else if(dictNames.empty()) {
                                    emit dictHasBeenDeleted(QString("There is no added dictionaries!"));
                                }

                            });

}

void SelectDictionaryDialog::initMainDictParameters() {
    dictNamesInitial = settings_ptr->value("list_of_dictionaries").value<QStringList>();       //load the dict names and the current dict
    actualDictNameInitial = settings_ptr->value("loading_dict_path").toString();                  //from the settings

    dictNames = dictNamesInitial;
    actualDictName = actualDictNameInitial;
    selectedDictName = actualDictName;

    //selectedDictName = "";
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

    int actualRow = dictNames.indexOf(actualDictName);
    int selectedRow = dictNames.indexOf(selectedDictName);

    QStringList shortDictNames = genShortNames(dictNames);

    ui->dictListWidget->clear();
    ui->dictListWidget->addItems(shortDictNames);

    /*
    QList<QListWidgetItem*> selectedDicts = ui->dictListWidget->findItems(currentDict, Qt::MatchFixedString);

    if(!selectedDicts.empty()) {
        ui->dictListWidget->setCurrentItem(*(selectedDicts.begin()));
        settings_ptr->setValue("loading_dict_path", currentDict);
    }
    */

    for (int row = 0; row < dictNames.count(); ++row) {
        QListWidgetItem* item = ui->dictListWidget->item(row);
        item->setToolTip(dictNames[row]);
    }

    if (actualRow > -1) {
        //ui->dictListWidget->item(actualRow)->setBackgroundColor(Qt::green);
        //ui->dictListWidget->item(actualRow)->setIcon(loadingSuccessIcon);
        printDictStatusIcons();
        setDeleteButton(true);
        setSelectButton(true);

    }
    else if (dictNames.empty()) {
        actualDictName = "";
        //setOkButton(false);
        setDeleteButton(false);
        setSelectButton(false);
        emit dictErrorThrow(QString("There is no added dictionaries!"));
    }

    if (selectedRow != -1)
        ui->dictListWidget->setCurrentRow(selectedRow);
    else if (!dictNames.empty()) {
        ui->dictListWidget->setCurrentRow(0);
        //ui->dictListWidget->item(0)->setIcon(loadingErrorIcon);
    }

    if (!dictNames.empty())
        settings_ptr->setValue("list_of_dictionaries", QVariant::fromValue(dictNames));
}


/*
void SelectDictionaryDialog::setOkButton(const bool& state) {
    ui->dictOkCancelBox->button(QDialogButtonBox::Ok)->setEnabled(state);
}
*/


void SelectDictionaryDialog::setDeleteButton(const bool& state) {
    ui->delDictButton->setEnabled(state);
}

void SelectDictionaryDialog::setSelectButton(const bool &state) {
    ui->selectDictButton->setEnabled(state);
}

void SelectDictionaryDialog::closeEvent(QCloseEvent *event) {
    emit  ui->dictOkCancelBox->rejected();
}
SelectDictionaryDialog::~SelectDictionaryDialog()
{
    delete ui;
}

void SelectDictionaryDialog::applyDictStatus(const ActualDictStatus& status) {
    //int actualRow = dictNames.indexOf(actualDictName);
    //ui->dictListWidget->item(actualRow)->setIcon(loadingErrorIcon);
    //ui->dictListWidget->item(actualRow)->setBackgroundColor(Qt::red);
    if (status == ActualDictStatus::UNKNOWN)
        qWarning() << "UNKNOWN dict status!";
    actualDictStatus = {actualDictName, status};
    printDictStatusIcons();

}

void SelectDictionaryDialog::chooseSelectedDict() {
    if (!dictNames.empty()) {
        int selectedRow = ui->dictListWidget->currentRow();
        actualDictName = dictNames[selectedRow];
    }
    settings_ptr->setValue("list_of_dictionaries", QVariant::fromValue(dictNames));
    settings_ptr->setValue("loading_dict_path", actualDictName);

    if (!dictNames.empty()) {
        emit this->dictHasBeenChosen();
    }
}

void SelectDictionaryDialog::printDictStatusIcons() {

    int actualRow = dictNames.indexOf(actualDictName);
    if (actualRow > -1) {
        switch (actualDictStatus.second) {
        case ActualDictStatus::ERROR:
            ui->dictListWidget->item(actualRow)->setIcon(loadingErrorIcon);
            break;
        case ActualDictStatus::OK:
            ui->dictListWidget->item(actualRow)->setIcon(loadingSuccessIcon);
            break;
        case ActualDictStatus::UNKNOWN:
            ui->dictListWidget->item(actualRow)->setIcon(loadingUnknownIcon);
            break;
        default:
            qDebug() << "Incorrect dictionary state";
        }
    }
}


CustomDelegate::CustomDelegate(QObject* parent): QStyledItemDelegate(parent) {

}

void CustomDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const {
    QStyleOptionViewItem iconsInDictListOption(option);
    iconsInDictListOption.decorationPosition = QStyleOptionViewItem::Right;
    QStyledItemDelegate::paint(painter, iconsInDictListOption, index);
}
