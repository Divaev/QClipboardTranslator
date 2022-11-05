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
#include <QIcon>
#include <QStyledItemDelegate>

#include <thread>

QT_BEGIN_NAMESPACE
namespace Ui {
    class SelectDictionaryDialog;
}
QT_END_NAMESPACE

class CustomDelegate;

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
    QString loadedDictName;
    QString selectedDictName;
    Ui::SelectDictionaryDialog *ui;

    QIcon loadingErrorIcon;
    CustomDelegate* customDelegate;

    void setOkButton(const bool&);
    void setDeleteButton(const bool&);

    void closeEvent(QCloseEvent* event) override;

signals:
    void dictHasBeenChosen();
    void dictHasBeenDeleted(const QString&, const bool& = true);
    void dictErrorThrow(const QString&, const bool& = true);
};




class CustomDelegate: public QStyledItemDelegate {
    Q_OBJECT
public:
    CustomDelegate(QObject* parent);
private:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};


#endif // SELECTDICTIONARYDIALOG_H
