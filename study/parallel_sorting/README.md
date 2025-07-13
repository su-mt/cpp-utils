# Parallel External Merge Sort

## Описание

Данная программа реализует внешний сортировщик (External Merge Sort) для очень больших файлов. Алгоритм делит исходный файл на чанки по 100 МБ, сортирует их параллельно с помощью пула потоков, а затем сливает в итоговый отсортированный файл.

> ⚠️ На малых объёмах данных (до сотен тысяч строк) программа работает неэффективно — накладные расходы на файловые операции и многопоточность превышают выигрыш от сортировки.

---

## Использование

1. **Генерация тестовых данных:**

   Для генерации файла с 1 000 000 случайных строк используйте Python-скрипт:

   ```bash
   python3 generate_strings.py
   ```
   
   Будет создан файл `file.txt`.

2. **Сборка проекта:**

   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. **Запуск сортировки:**

   ```bash
   ./parallel_sorting file.txt
   ```
   
   (Параметры запуска могут отличаться, см. исходный код `main.cpp`)

---

## Пример Python-кода для генерации строк

```python
import random
import string

def generate_random_strings(filename: str, count: int = 10**6, min_len: int = 5, max_len: int = 15):
    with open(filename, 'w') as f:
        for _ in range(count):
            length = random.randint(min_len, max_len)
            rand_str = ''.join(random.choices(string.ascii_letters + string.digits, k=length))
            f.write(rand_str + '\n')

if __name__ == "__main__":
    generate_random_strings("file.txt")
```

---

## Примечания
- Размер чанка (100 МБ) можно изменить в исходном коде.
- Для очень больших файлов убедитесь, что на диске достаточно места для временных файлов.
- Для малых файлов используйте обычные сортировки — внешний merge sort эффективен только на гигабайтах данных.
