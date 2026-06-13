#include "cardmanager.h"
#include <fstream>
#include <sstream>
#include <random>

CardManager::CardManager(std::string file) : filename(file) {
    loadFromFile();
}

void CardManager::loadFromFile() {
    cards.clear();
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string unk, trans, corrStr, incorrStr, weightStr;
        
        // Чтение данных через разделитель ';'
        if (std::getline(ss, unk, ';') && std::getline(ss, trans, ';') &&
            std::getline(ss, corrStr, ';') && std::getline(ss, incorrStr, ';') &&
            std::getline(ss, weightStr, ';')) {
            cards.emplace_back(unk, trans, std::stoi(corrStr), std::stoi(incorrStr), std::stoi(weightStr));
        }
    }
    file.close();
}

void CardManager::saveToFile() {
    std::ofstream file(filename);
    for (const auto& card : cards) {
        file << card.getUnknown() << ";" 
             << card.getTranslation() << ";"
             << card.getCorrectCount() << ";" 
             << card.getIncorrectCount() << ";"
             << card.getWeight() << "\n";
    }
    file.close();
}

void CardManager::addCard(const std::string& unk, const std::string& trans) {
    cards.emplace_back(unk, trans);
    saveToFile();
}

void CardManager::deleteCard(size_t index) {
    if (index < cards.size()) {
        cards.erase(cards.begin() + index);
        saveToFile();
    }
}

void CardManager::updateCard(size_t index, const std::string& unk, const std::string& trans) {
    if (index < cards.size()) {
        cards[index].setUnknown(unk);
        cards[index].setTranslation(trans);
        saveToFile();
    }
}

const std::vector<Flashcard>& CardManager::getCards() const { return cards; }

int CardManager::getRandomCardIndex() {
    if (cards.empty()) return -1;
    
    int totalWeight = 0;
    for (const auto& card : cards) {
        totalWeight += card.getWeight();
    }
    
    // Рандом на C++
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, totalWeight - 1);
    int randomValue = distr(gen);
    
    int currentSum = 0;
    for (size_t i = 0; i < cards.size(); ++i) {
        currentSum += cards[i].getWeight();
        if (randomValue < currentSum) {
            return i;
        }
    }
    return 0;
}
