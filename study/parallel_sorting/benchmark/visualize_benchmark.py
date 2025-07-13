import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Загрузка CSV
df = pd.read_csv("benchmark_results.csv")

# Извлечение группы из имени файла
df["group"] = df["group"].str.extract(r"benchmark_(\w+)\.csv")

# Упрощение названий методов
def simplify(cmd):
    if "stdsort" in cmd:
        return "std::sort" if "--stable" not in cmd else "std::stable_sort"
    if "-m" in cmd:
        return "MergeSort"
    if "-q" in cmd:
        return "QuickSort"
    return cmd

df["method"] = df["command"].apply(simplify)

# 🔴 Удаление строки с MergeSort для big
df = df[~((df["group"] == "big") & (df["method"] == "MergeSort"))]

# Визуализация
sns.set(style="whitegrid")
plt.figure(figsize=(12, 6))

ax = sns.barplot(
    data=df,
    x="group",
    y="mean",
    hue="method",
    palette="Set2"
)

ax.set_title("Сравнение сортировок по среднему времени (сек)", fontsize=14)
ax.set_ylabel("Время, сек")
ax.set_xlabel("Группа входных данных")
plt.legend(title="Метод сортировки")
plt.tight_layout()

# Сохранение и отображение
plt.savefig("benchmark_plot.png", dpi=300)
plt.show()
