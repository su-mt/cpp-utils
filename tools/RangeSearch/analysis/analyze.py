import json
import os
import matplotlib.pyplot as plt
import numpy as np

def load_benchmark_data(filepath):
    with open(filepath, 'r') as f:
        data = json.load(f)
    return data['benchmarks']

def plot_vary_n(benchmarks, output_dir='analysis/plots'):
    # Создаем директорию, если её нет
    os.makedirs(output_dir, exist_ok=True)
    
    # Фильтруем реальные итерации бенчмарка
    data = [b for b in benchmarks if "Search_VaryN" in b['name'] and b.get('run_type') == 'iteration']
    
    if not data:
        print("Нет данных для графика Vary N")
        return

    trees = {}
    for entry in data:
        tree_name = entry['name'].split('_')[0] 
        if tree_name not in trees:
            trees[tree_name] = {'n': [], 'time': []}
        
        trees[tree_name]['n'].append(entry['n_points'])
        trees[tree_name]['time'].append(entry['cpu_time'])

    plt.figure(figsize=(10, 6))
    for tree_name, metrics in trees.items():
        sorted_indices = np.argsort(metrics['n'])
        n_sorted = np.array(metrics['n'])[sorted_indices]
        time_sorted = np.array(metrics['time'])[sorted_indices]
        
        plt.plot(n_sorted, time_sorted, marker='o', label=tree_name, linewidth=2)

    plt.xscale('log', base=2)
    plt.yscale('log', base=10)
    plt.xlabel('Количество точек в дереве (N)')
    plt.ylabel('Время поиска (микросекунды)')
    plt.title('Зависимость времени 3D поиска от количества точек (Range = 10%)')
    plt.grid(True, which="both", ls="--", alpha=0.5)
    plt.legend()
    plt.tight_layout()
    
    # Сохраняем в целевую папку
    save_path = os.path.join(output_dir, 'search_vs_n.png')
    plt.savefig(save_path, dpi=300)
    print(f"График успешно сохранен в: {save_path}")
    plt.close()

def plot_vary_range(benchmarks, output_dir='analysis/plots'):
    # Создаем директорию, если её нет
    os.makedirs(output_dir, exist_ok=True)
    
    # Фильтруем реальные итерации бенчмарка
    data = [b for b in benchmarks if "Search_VaryRange" in b['name'] and b.get('run_type') == 'iteration']
    
    if not data:
        print("Нет данных для графика Vary Range")
        return

    trees = {}
    for entry in data:
        tree_name = entry['name'].split('_')[0]
        if tree_name not in trees:
            trees[tree_name] = {'pct': [], 'time': []}
        
        trees[tree_name]['pct'].append(entry['query_pct'])
        trees[tree_name]['time'].append(entry['cpu_time'])

    plt.figure(figsize=(10, 6))
    for tree_name, metrics in trees.items():
        sorted_indices = np.argsort(metrics['pct'])
        pct_sorted = np.array(metrics['pct'])[sorted_indices]
        time_sorted = np.array(metrics['time'])[sorted_indices]
        
        plt.plot(pct_sorted, time_sorted, marker='s', label=tree_name, linewidth=2)

    plt.xlabel('Размер диапазона поиска (% от объема пространства)')
    plt.ylabel('Время поиска (микросекунды)')
    plt.title('Зависимость времени 3D поиска от размера диапазона (N = 100,000)')
    plt.grid(True, linestyle="--", alpha=0.7)
    plt.legend()
    plt.tight_layout()
    
    # Сохраняем в целевую папку
    save_path = os.path.join(output_dir, 'search_vs_range.png')
    plt.savefig(save_path, dpi=300)
    print(f"График успешно сохранен в: {save_path}")
    plt.close()

if __name__ == "__main__":
    # Предполагается, что скрипт запускается из корня проекта: python analysis/analyze.py
    results = load_benchmark_data("benchmark_results.json")
    plot_vary_n(results)
    plot_vary_range(results)