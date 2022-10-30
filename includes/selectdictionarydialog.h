#ifndef SELECTDICTIONARYDIALOG_H
#define SELECTDICTIONARYDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QFileDialog>
#include <QStringList>
#include <QDebug>
#include <algorithm>

QT_BEGIN_NAMESPACE
namespace Ui {
    class SelectDictionaryDialog;
}
QT_END_NAMESPACE

class SelectDictionaryDialog : public QDialog
{
    Q_OBJECT

public:
    QSettings* settings_ptr;
    explicit SelectDictionaryDialog(QSettings* set_ptr, QWidget *parent = nullptr);
    ~SelectDictionaryDialog();

private:
    QFileDialog* addDictDialog;
    QStringList dictNames;
    QString currentDict;
    Ui::SelectDictionaryDialog *ui;

    void refreshDictListWidget();

signals:
    void dictHasBeenChosen();


};

#endif // SELECTDICTIONARYDIALOG_H
