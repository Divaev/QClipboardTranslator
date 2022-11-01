#include "words_finder.h"
#include <cstring>

WordsReceiver::WordsReceiver(QObject* parent): QObject(parent) {

}

void WordsReceiver::setCurrentWord(const QString& word) {
    input_word = word;
}

void WordsFinder::findTheSingleWord(const QString& word) {
    QString lower_word = word.toLower();
    pugi::xml_node main_node = current_dictionary.child("xdxf");
    pugi::xml_node found_node = current_dictionary.find_node([lower_word](const pugi::xml_node& curr_node) {
                                                                            return strcmp(curr_node.value(), lower_word.toStdString().c_str()) ? false: true;
                                                                         });

    if(found_node != nullptr) {
        QMap<QString, QString> result = translateTheSingleWorld(found_node);
        emit translationIsReady(result);
    }
    else
        emit wordIsNotFound();
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
    bool res = current_dictionary.load_file(dict_path.toStdString().c_str());
    qDebug() << (res ? QString("Dict is loaded: ") + dict_path :
                       QString("Dict is not initialized"));
    if (res == true)
        //emit dictionaryInitError();
        emit dictionaryHasBeenInitialized();
    else
        emit dictionaryInitError();
}


struct WordsFinder::ErrorMessages WordsFinder::errorMessages;

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

    QObject::connect(wordsFinder, &WordsFinder::wordIsNotFound,
                     wordsReceiver, &WordsReceiver::sendErrorNotification);

    QObject::connect(wordsReceiver, &WordsReceiver::setDictPath,
                     wordsFinder, &WordsFinder::initTheDict);

    QObject::connect(wordsFinder, &WordsFinder::dictionaryHasBeenInitialized,
                     wordsReceiver, &WordsReceiver::dictionaryIsReady);



    QObject::connect(this, &QThread::finished,                              //When the thread stops, the wordsReceiver will be deleted
                     wordsReceiver, &QObject::deleteLater);                 //from heap

    QObject::connect(wordsReceiver, &WordsReceiver::stopWordsFinderThread,
                     this, &QThread::quit);



    exec();                     //we need to start the event loop for translation requests receiving
                                //which comes from GUI thread via queues connection
}

WordsFinder* WordsFinderThread::getWordsFinder() {
    return wordsFinder;
}

WordsFinderThread::~WordsFinderThread() {
    delete wordsFinder;
}

