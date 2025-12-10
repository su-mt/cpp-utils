#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <stdexcept>


class FileStreamException : public std::exception {
protected:
    std::string message;
    
public:
    explicit FileStreamException(const std::string& msg) : message(msg) {}
    
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class FileOpenException : public FileStreamException {
public:
    explicit FileOpenException(const std::string& filename) : FileStreamException(
        "Ошибка: Не удалось открыть файл '" + filename + "'. "
        "Проверьте, что файл существует и имеет необходимые права доступа.") {}
};

class EmptyFileException : public FileStreamException {
public:
    explicit EmptyFileException(const std::string& filename) : FileStreamException(
        "Ошибка: Файл '" + filename + "' пуст. Пожалуйста, добавьте содержимое в файл.") {}
};

class EmptySearchQueryException : public FileStreamException {
public:
    EmptySearchQueryException() : FileStreamException(
        "Ошибка: Поисковый запрос не может быть пустым!") {}
};


std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}


bool sentenceContainsWord(const std::string& sentence, const std::string& word) {
    std::string lowerSentence = toLower(sentence);
    std::string lowerWord = toLower(word);
    

    size_t pos = 0;
    while ((pos = lowerSentence.find(lowerWord, pos)) != std::string::npos) {

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


std::vector<std::string> extractSentences(const std::string& text) {
    std::vector<std::string> sentences;
    std::string sentence;
    
    for (size_t i = 0; i < text.length(); ++i) {
        sentence += text[i];
        

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
    try {
        std::string filename;
        std::string searchWord;
        
        std::cout << "=== Программа поиска предложений в файле ===" << std::endl;
        std::cout << "Введите имя файла: ";
        std::getline(std::cin, filename);
        
        if (filename.empty()) {
            throw FileOpenException("(пустое имя)");
        }
        

        std::ifstream inputFile(filename);
        
        if (!inputFile.is_open()) {
            throw FileOpenException(filename);
        }
        

        std::stringstream buffer;
        buffer << inputFile.rdbuf();
        std::string fileContent = buffer.str();
        inputFile.close();
        
        if (fileContent.empty()) {
            throw EmptyFileException(filename);
        }
        
        std::cout << "Введите слово для поиска: ";
        std::getline(std::cin, searchWord);
        
        if (searchWord.empty()) {
            throw EmptySearchQueryException();
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
        std::cout << "Всего предложений в файле: " << sentences.size() << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        
        if (foundSentences.empty()) {
            std::cout << "⚠ Предложений с словом \"" << searchWord << "\" не найдено." << std::endl;
        } else {
            std::cout << "✓ Найдено " << foundSentences.size() << " предложение(й):" << std::endl;
            std::cout << std::string(50, '-') << std::endl;
            
            for (size_t i = 0; i < foundSentences.size(); ++i) {
                std::cout << i + 1 << ". " << foundSentences[i] << std::endl;
            }
        }
        
        std::cout << std::string(50, '-') << std::endl;
        
    } catch (const FileStreamException& e) {
        std::cerr << "\n❌ " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Непредвиденная ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
