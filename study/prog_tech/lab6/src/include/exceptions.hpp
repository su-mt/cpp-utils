#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>

namespace STUDENT {

class StudentException : public std::exception {
protected:
    std::string message;
    
public:
    explicit StudentException(const std::string& msg) : message(msg) {}
    
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class EmptySurnameException : public StudentException {
public:
    EmptySurnameException() : StudentException(
        "Ошибка: Фамилия и инициалы студента не могут быть пустыми!") {}
};

class InvalidGroupNumberException : public StudentException {
public:
    explicit InvalidGroupNumberException(int group) : StudentException(
        "Ошибка: Неверный номер группы '" + std::to_string(group) + 
        "'. Номер группы должен быть положительным числом!") {}
};

class EmptyMarksException : public StudentException {
public:
    EmptyMarksException() : StudentException(
        "Ошибка: Массив оценок студента не может быть пустым!") {}
};

class InvalidMarkException : public StudentException {
public:
    explicit InvalidMarkException(int mark) : StudentException(
        "Ошибка: Неверная оценка '" + std::to_string(mark) + 
        "'. Оценка должна быть в диапазоне от 1 до 5!") {}
};

class FileException : public StudentException {
public:
    explicit FileException(const std::string& filename) : StudentException(
        "Ошибка: Не удалось открыть файл '" + filename + "'. "
        "Проверьте, что файл существует и имеет необходимые права доступа.") {}
};

class EmptyFileException : public StudentException {
public:
    explicit EmptyFileException(const std::string& filename) : StudentException(
        "Ошибка: Файл '" + filename + "' пуст. Пожалуйста, добавьте содержимое в файл.") {}
};

class EmptySearchQueryException : public StudentException {
public:
    EmptySearchQueryException() : StudentException(
        "Ошибка: Поисковый запрос не может быть пустым!") {}
};

class InputException : public StudentException {
public:
    explicit InputException(const std::string& msg) : StudentException(
        "Ошибка ввода: " + msg) {}
};

} // namespace STUDENT

#endif // EXCEPTIONS_HPP
