#ifndef CLIPBOARDCHKR_H
#define CLIPBOARDCHKR_H
#include <QClipboard>
#include <QApplication>
#include <QDebug>
#include <QThread>

#include <thread>
#include <chrono>


class ClipboardThread;

class SignalReceiver: public QObject {
    Q_OBJECT
    QString curr_word;
public:
    SignalReceiver(QObject* parent = nullptr);
public slots:

    void receiveWordFromClipboard(const QString&);

signals:
    void sendWordOut(const QString&);
};

class ClipboardChecker: public QObject {
    Q_OBJECT
    QClipboard* clipboard;
public:
    friend class ClipboardThread;
    ClipboardChecker(QObject *parent = nullptr);
    int testXML() const;
    void testClipboard() const;
signals:
    void wordIsChanged(const QString&);
};

class ClipboardThread: public QThread {
    Q_OBJECT
    ClipboardChecker* clipboardChecker;
    SignalReceiver* signalReceiver;

public:
    ClipboardThread(SignalReceiver* receiver, QObject* parent = nullptr);
    void run() override;
};

#endif // CLIPBOARDCHKR_H
