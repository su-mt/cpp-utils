#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

// Функция для преобразования строки в нижний регистр
std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

// Функция для проверки, содержит ли предложение заданное слово
bool sentenceContainsWord(const std::string& sentence, const std::string& word) {
    std::string lowerSentence = toLower(sentence);
    std::string lowerWord = toLower(word);
    
    // Ищем слово как целое слово (не как часть другого слова)
    size_t pos = 0;
    while ((pos = lowerSentence.find(lowerWord, pos)) != std::string::npos) {
        // Проверяем, что это целое слово
        bool isStartOfWord = (pos == 0) || !std::isalnum(lowerSentence[pos - 1]);
        bool isEndOfWord = (pos + lowerWord.length() == lowerSentence.length()) || 
                          !std::isalnum(lowerSentence[pos + lowerWord.length()]);
        
        if (isStartOfWord && isEndOfWord) {
            return true;
        }
        pos++;
    }
    return false;
}

// Функция для извлечения предложений из текста
std::vector<std::string> extractSentences(const std::string& text) {
    std::vector<std::string> sentences;
    std::string sentence;
    
    for (size_t i = 0; i < text.length(); ++i) {
        sentence += text[i];
        
        // Предложение заканчивается на '.', '!', '?'
        if (text[i] == '.' || text[i] == '!' || text[i] == '?') {
            // Удаляем пробелы в начале и конце
            size_t start = sentence.find_first_not_of(" \t\n\r");
            size_t end = sentence.find_last_not_of(" \t\n\r");
            
            if (start != std::string::npos) {
                sentences.push_back(sentence.substr(start, end - start + 1));
            }
            sentence.clear();
        }
    }
    
    // Добавляем последнее предложение, если оно не пусто
    if (!sentence.empty()) {
        size_t start = sentence.find_first_not_of(" \t\n\r");
        size_t end = sentence.find_last_not_of(" \t\n\r");
        
        if (start != std::string::npos) {
            sentences.push_back(sentence.substr(start, end - start + 1));
        }
    }
    
    return sentences;
}

int main() {
    std::string filename;
    std::string searchWord;
    
    std::cout << "=== Программа поиска предложений в файле ===" << std::endl;
    std::cout << "Введите имя файла: ";
    std::getline(std::cin, filename);
    
    // Открываем файл для чтения
    std::ifstream inputFile(filename);
    
    if (!inputFile.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл '" << filename << "'" << std::endl;
        return 1;
    }
    
    // Читаем весь текст из файла в строку
    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string fileContent = buffer.str();
    inputFile.close();
    
    if (fileContent.empty()) {
        std::cout << "Файл пуст." << std::endl;
        return 0;
    }
    
    std::cout << "Введите слово для поиска: ";
    std::getline(std::cin, searchWord);
    
    if (searchWord.empty()) {
        std::cout << "Слово для поиска не может быть пустым." << std::endl;
        return 1;
    }
    
    // Извлекаем предложения из текста
    std::vector<std::string> sentences = extractSentences(fileContent);
    
    // Ищем предложения, содержащие заданное слово
    std::vector<std::string> foundSentences;
    for (const auto& sentence : sentences) {
        if (sentenceContainsWord(sentence, searchWord)) {
            foundSentences.push_back(sentence);
        }
    }
    
    // Выводим результаты
    std::cout << "\n=== Результаты поиска ===" << std::endl;
    std::cout << "Ищем слово: \"" << searchWord << "\"" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    
    if (foundSentences.empty()) {
        std::cout << "Предложений с словом \"" << searchWord << "\" не найдено." << std::endl;
    } else {
        std::cout << "Найдено " << foundSentences.size() << " предложение(й):" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        
        for (size_t i = 0; i < foundSentences.size(); ++i) {
            std::cout << i + 1 << ". " << foundSentences[i] << std::endl;
        }
    }
    
    std::cout << std::string(50, '-') << std::endl;
    
    return 0;
}
