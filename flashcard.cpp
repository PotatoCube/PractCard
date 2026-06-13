#include "flashcard.h"
#include <algorithm>

Flashcard::Flashcard(std::string unk, std::string trans, int corr, int incorr, int w)
    : unknown(unk), translation(trans), correctCount(corr), incorrectCount(incorr), weight(w) {}

std::string Flashcard::getUnknown() const { return unknown; }
std::string Flashcard::getTranslation() const { return translation; }
int Flashcard::getCorrectCount() const { return correctCount; }
int Flashcard::getIncorrectCount() const { return incorrectCount; }
int Flashcard::getWeight() const { return weight; }

void Flashcard::setUnknown(const std::string& unk) { unknown = unk; }
void Flashcard::setTranslation(const std::string& trans) { translation = trans; }

void Flashcard::markCorrect() {
    correctCount++;
    weight = std::max(10, weight - 20); // Ограничиваем минимальный вес до 10
}

void Flashcard::markIncorrect() {
    incorrectCount++;
    weight += 50; // Увеличиваем вес, чтобы слово выпадало чаще
}