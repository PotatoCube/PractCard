#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <string>

class Flashcard {
private:
    std::string unknown;
    std::string translation;
    int correctCount;
    int incorrectCount;
    int weight; // Вес для алгоритма частоты показа

public:
    Flashcard(std::string unk, std::string trans, int corr = 0, int incorr = 0, int w = 100);

    std::string getUnknown() const;
    std::string getTranslation() const;
    int getCorrectCount() const;
    int getIncorrectCount() const;
    int getWeight() const;

    void setUnknown(const std::string& unk);
    void setTranslation(const std::string& trans);

    void markCorrect();   // Увеличивает верные, уменьшает вес (реже показ)
    void markIncorrect(); // Увеличивает неверные, растит вес (чаще показ)
};

#endif