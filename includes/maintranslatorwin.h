#ifndef MAINTRANSLATORWIN_H
#define MAINTRANSLATORWIN_H

#include <QtCore>
#include <QMainWindow>
#include <QTextCursor>
#include <QClipboard>
#include <QSettings>
#include "words_finder.h"
#include "selectdictionarydialog.h"
#include "qglobalshortcut.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainTranslatorWin; }
QT_END_NAMESPACE



class MainTranslatorWin : public QMainWindow
{
    Q_OBJECT

    QSettings settings;
    QString current_word;

    QClipboard *clipboard;
    WordsReceiver *wordsReceiver;               //stays in GUI thread, receives translations
    WordsFinderThread* wordsFinderThread;       //finding in the dictionary goes to another thread

    void closeEvent(QCloseEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
public:
    MainTranslatorWin(QWidget *parent = nullptr);
    WordsFinderThread* getWordsFinderThread();
    ~MainTranslatorWin();

private:
    Ui::MainTranslatorWin *ui;
    SelectDictionaryDialog *dictionaryDialog;
    QGlobalShortcut *wakeUpGlobalShortcut;

public slots:

    void printResultTranslation(const QMap<QString, QString>&);
    void proceedError(const QString&, const bool&);
    void makeTranslateAvailable();

signals:
    void stopWordsFinderThread();
};
#endif // MAINTRANSLATORWIN_H
