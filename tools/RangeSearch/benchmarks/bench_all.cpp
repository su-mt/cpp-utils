#include <benchmark/benchmark.h>
#include <cstring>
#include <vector>

int main(int argc, char** argv) {
    bool run_vary = false;
    std::vector<char*> new_argv;

    // 1. Ищем кастомный флаг и фильтруем его из списка аргументов
    for (int i = 0; i < argc; ++i) {
        if (std::strcmp(argv[i], "--findvary") == 0) {
            run_vary = true;
        } else {
            new_argv.push_back(argv[i]);
        }
    }

    // 2. Если флага --findvary нет, добавляем фильтр исключения.
    // Регулярка "-.*Vary.*" отсечет BM_Search_VaryN и BM_Search_VaryRange.
    char filter_arg[] = "--benchmark_filter=-.*Vary.*";
    if (!run_vary) {
        new_argv.push_back(filter_arg);
    }

    int new_argc = static_cast<int>(new_argv.size());

    // 3. Стандартная процедура инициализации Google Benchmark
    ::benchmark::Initialize(&new_argc, new_argv.data());
    
    // Если остались нераспознанные флаги (кроме нашего), завершаем с ошибкой
    if (::benchmark::ReportUnrecognizedArguments(new_argc, new_argv.data())) {
        return 1;
    }
    
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();
    
    return 0;
}