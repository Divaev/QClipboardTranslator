#include "words_finder.h"
#include <cstring>

WordsReceiver::WordsReceiver(QObject* parent): QObject(parent) {

}

void WordsReceiver::setCurrentWord(const QString& word) {
    input_word = word;
}

void WordsFinder::findTheSingleWord(const QString& word) {
    pugi::xml_node main_node = current_dictionary.child("xdxf");
    pugi::xml_node found_node = current_dictionary.find_node([word](const pugi::xml_node& curr_node) {
                                                                    return strcmp(curr_node.value(), word.toStdString().c_str()) ? false: true;
                                                                });

    if(found_node != nullptr) {
        QMap<QString, QString> result = translateTheSingleWorld(found_node);
        emit translationIsReady(result);
    }
}


QMap<QString, QString> WordsFinder::translateTheSingleWorld(const pugi::xml_node& found_node) const {

    auto node = found_node.parent();
    node = node.next_sibling();

    QMap<QString, QString> translationResult;

    while(node != nullptr) {

        if(node.type() == pugi::xml_node_type::node_element) {
            translationResult["transcription"] = node.child_value();
        }
        else if(node.type() == pugi::xml_node_type::node_pcdata) {
            translationResult["translation"] = node.value();
        }
        node = node.next_sibling();
    }

    return translationResult;
}

void WordsFinder::initTheDict(const QString& dict_path) {
    current_dictionary.load_file(dict_path.toStdString().c_str());
}


WordsFinder::FinderError::FinderError(const char* err): errorMsg(err) {

}

void WordsFinder::FinderError::typeError() const {
    qDebug() << QString(errorMsg);
}

const char *WordsFinder::FinderError::getError() const {
    return errorMsg;
}

WordsFinder::~WordsFinder() {
    //current_dictionary.reset();
}

WordsFinderThread::WordsFinderThread(WordsReceiver *receiver,
                                     const QString& _dict_path,
                                     QObject* parent): wordsReceiver(receiver),
                                                       dict_path(_dict_path),
                                                       QThread(parent) {
}

void WordsFinderThread::run() {

    wordsFinder = new WordsFinder;
    wordsFinder->initTheDict(dict_path);

    QObject::connect(wordsReceiver, &WordsReceiver::wordIsReceived,
                     wordsFinder, &WordsFinder::findTheSingleWord);

    QObject::connect(wordsFinder, &WordsFinder::translationIsReady,
                     wordsReceiver, &WordsReceiver::sendResultOut);

    QObject::connect(wordsReceiver, &WordsReceiver::setDictPath,
                     wordsFinder, &WordsFinder::initTheDict);



    QObject::connect(this, &QThread::finished,                              //When the thread stops, the wordsReceiver will be deleted
                     wordsReceiver, &QObject::deleteLater);                 //from heap

    QObject::connect(wordsReceiver, &WordsReceiver::stopWordsFinderThread,
                     this, &QThread::quit);



    exec();                     //we need to start the event loop for translation requests receiving
                                //which comes from GUI thread vie queues connection
}

WordsFinder* WordsFinderThread::getWordsFinder() {
    return wordsFinder;
}

WordsFinderThread::~WordsFinderThread() {
    delete wordsFinder;
}

