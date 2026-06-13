#ifndef CARDMANAGER_H
#define CARDMANAGER_H

#include "flashcard.h"
#include <vector>
#include <string>

class CardManager {
private:
    std::vector<Flashcard> cards;
    std::string filename;

public:
    CardManager(std::string file = "words.txt");
    
    void markCardCorrect(size_t index);
    void markCardIncorrect(size_t index);

    void loadFromFile();
    void saveToFile();
    
    void addCard(const std::string& unk, const std::string& trans);
    void deleteCard(size_t index);
    void updateCard(size_t index, const std::string& unk, const std::string& trans);
    
    const std::vector<Flashcard>& getCards() const;
    
    // Алгоритм взвешенного случайного выбора (умная тренировка)
    int getRandomCardIndex(); 
};

#endif