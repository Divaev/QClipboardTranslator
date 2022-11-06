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

enum class ActualDictStatus {OK, ERROR, UNKNOWN};

class CustomDelegate;

class SelectDictionaryDialog : public QDialog
{
    Q_OBJECT

public:
    QSettings* settings_ptr;
    explicit SelectDictionaryDialog(QSettings* set_ptr, QWidget *parent = nullptr);
    ~SelectDictionaryDialog();

    void refreshDictListWidget();
    void initMainDictParameters();

private:
    QFileDialog* addDictDialog;

    QStringList dictNames;
    QString actualDictName;
    QString selectedDictName;

    QStringList dictNamesInitial;
    QString actualDictNameInitial;
    //QString selectedDictNameInitial;


    //ActualDictStatus actualDictStatus;

    QPair<QString, ActualDictStatus> actualDictStatus;



    Ui::SelectDictionaryDialog *ui;

    QIcon loadingErrorIcon, loadingSuccessIcon, loadingUnknownIcon;
    CustomDelegate* customDelegate;

    //void setOkButton(const bool&);
    void setDeleteButton(const bool&);
    void setSelectButton(const bool&);

    void closeEvent(QCloseEvent* event) override;

signals:
    void dictHasBeenChosen();
    void dictHasBeenDeleted(const QString&, const bool& = true);
    void dictErrorThrow(const QString&, const bool& = true);

public slots:
    //void markLoadedDictAsBroken();
    void applyDictStatus(const ActualDictStatus& status);

    void printDictStatusIcons();
    void chooseSelectedDict();
};




class CustomDelegate: public QStyledItemDelegate {
    Q_OBJECT
public:
    CustomDelegate(QObject* parent);
private:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};


#endif // SELECTDICTIONARYDIALOG_H
