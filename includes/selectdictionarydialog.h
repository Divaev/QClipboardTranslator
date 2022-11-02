#ifndef SELECTDICTIONARYDIALOG_H
#define SELECTDICTIONARYDIALOG_H

#include <QDialog>
#include <QWindow>
#include <QSettings>
#include <QFileDialog>
#include <QStringList>
#include <QDebug>
#include <algorithm>
#include <QHelpEvent>
#include <QToolTip>

#include <thread>

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

    void refreshDictListWidget();

private:
    QFileDialog* addDictDialog;
    QStringList dictNames;
    QString currentDict;
    Ui::SelectDictionaryDialog *ui;

    void setOkButton(const bool&);
    void setDeleteButton(const bool&);

    bool event(QEvent* event) override;

signals:
    void dictHasBeenChosen();
    void dictErrorThrow(const QString&, const bool& = true);






};

#endif // SELECTDICTIONARYDIALOG_H
