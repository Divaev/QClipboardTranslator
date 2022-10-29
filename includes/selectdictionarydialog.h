#ifndef SELECTDICTIONARYDIALOG_H
#define SELECTDICTIONARYDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
    class SelectDictionaryDialog;
}
QT_END_NAMESPACE

class SelectDictionaryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectDictionaryDialog(QWidget *parent = nullptr);
    ~SelectDictionaryDialog();

private:
    Ui::SelectDictionaryDialog *ui;
};

#endif // SELECTDICTIONARYDIALOG_H
