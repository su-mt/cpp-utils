import json
import os
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
from scipy.interpolate import PchipInterpolator

# ---------------------------------------------------------------------------
# Production-style настройки matplotlib
# ---------------------------------------------------------------------------
mpl.rcParams.update({
    "figure.dpi": 120,
    "savefig.dpi": 300,
    "font.family": "DejaVu Sans",
    "font.size": 12,
    "axes.titlesize": 15,
    "axes.titleweight": "bold",
    "axes.labelsize": 13,
    "axes.edgecolor": "#333333",
    "axes.linewidth": 1.0,
    "xtick.labelsize": 11,
    "ytick.labelsize": 11,
    "legend.fontsize": 11,
    "legend.frameon": True,
    "legend.framealpha": 0.9,
    "legend.edgecolor": "#cccccc",
    "grid.color": "#c9c9c9",
    "grid.alpha": 0.45,
    "axes.facecolor": "white",
    "figure.facecolor": "white",
})

PALETTE = ["#2563eb", "#dc2626", "#059669", "#d97706", "#7c3aed", "#0891b2"]


def load_benchmark_data(filepath):
    with open(filepath, "r") as f:
        data = json.load(f)
    return data["benchmarks"]


def _aggregate(x_raw, y_raw):
    """Группирует повторные замеры с одинаковым x и считает mean/std.

    Нужно по двум причинам:
    1) PchipInterpolator требует строго уникальные x.
    2) Усреднение по повторам убирает шум измерений -> кривая объективно
       глаже, а не просто визуально сглажена интерполяцией.
    """
    x = np.asarray(x_raw, dtype=float)
    y = np.asarray(y_raw, dtype=float)
    order = np.argsort(x)
    x, y = x[order], y[order]
    x_unique, start_idx = np.unique(x, return_index=True)
    groups = np.split(y, start_idx[1:])
    mean = np.array([g.mean() for g in groups])
    std = np.array([g.std() for g in groups])
    return x_unique, mean, std


def _smooth(x, y, n=300):
    """Монотонная PCHIP-интерполяция: гладкая кривая без осцилляций,
    в отличие от обычного кубического сплайна на разреженных точках."""
    if len(x) < 3:
        return x, y
    pchip = PchipInterpolator(x, y)
    x_dense = np.linspace(x.min(), x.max(), n)
    return x_dense, pchip(x_dense)


def _plot_series(ax, x, mean, std, label, color, log_x=False):
    # Для оси с лог-масштабом интерполируем в log-пространстве,
    # иначе плотная сетка np.linspace будет неравномерной после log-преобразования осей
    x_axis = np.log2(x) if log_x else x

    x_dense, mean_dense = _smooth(x_axis, mean)
    _, upper_dense = _smooth(x_axis, mean + std)
    _, lower_dense = _smooth(x_axis, np.clip(mean - std, a_min=0, a_max=None))

    x_plot = 2 ** x_dense if log_x else x_dense

    ax.fill_between(x_plot, lower_dense, upper_dense, color=color, alpha=0.12, linewidth=0, zorder=1)
    ax.plot(x_plot, mean_dense, color=color, linewidth=2.4, solid_capstyle="round", zorder=3)
    ax.plot(x, mean, marker="o", linestyle="none", markersize=6,
            markerfacecolor=color, markeredgecolor="white", markeredgewidth=1.2,
            zorder=4, label=label)


def plot_vary_n(benchmarks, output_dir="analysis/plots"):
    os.makedirs(output_dir, exist_ok=True)
    data = [b for b in benchmarks if "Search_VaryN" in b["name"] and b.get("run_type") == "iteration"]
    if not data:
        print("Нет данных для графика Vary N")
        return

    trees = {}
    for entry in data:
        tree_name = entry["name"].split("_")[0]
        trees.setdefault(tree_name, {"n": [], "time": []})
        trees[tree_name]["n"].append(entry["n_points"])
        trees[tree_name]["time"].append(entry["cpu_time"])

    fig, ax = plt.subplots(figsize=(10, 6))
    for i, tree_name in enumerate(sorted(trees)):
        n, mean, std = _aggregate(trees[tree_name]["n"], trees[tree_name]["time"])
        _plot_series(ax, n, mean, std, tree_name, PALETTE[i % len(PALETTE)], log_x=True)

    ax.set_xscale("log", base=2)
    ax.set_yscale("log", base=10)
    ax.set_xlabel("Количество точек в дереве (N)")
    ax.set_ylabel("Время поиска (микросекунды)")
    ax.set_title("Зависимость времени 3D поиска от количества точек\n(Range = 10%)")
    ax.grid(True, which="major", linestyle="--", linewidth=0.8)
    ax.grid(True, which="minor", linestyle=":", linewidth=0.5, alpha=0.5)
    ax.legend(title="Структура данных", loc="best")
    fig.tight_layout()

    save_path = os.path.join(output_dir, "search_vs_n.png")
    fig.savefig(save_path, bbox_inches="tight")
    print(f"График успешно сохранён в: {save_path}")
    plt.close(fig)


def plot_vary_range(benchmarks, output_dir="analysis/plots"):
    os.makedirs(output_dir, exist_ok=True)
    data = [b for b in benchmarks if "Search_VaryRange" in b["name"] and b.get("run_type") == "iteration"]
    if not data:
        print("Нет данных для графика Vary Range")
        return

    trees = {}
    for entry in data:
        tree_name = entry["name"].split("_")[0]
        trees.setdefault(tree_name, {"pct": [], "time": []})
        trees[tree_name]["pct"].append(entry["query_pct"])
        trees[tree_name]["time"].append(entry["cpu_time"])

    fig, ax = plt.subplots(figsize=(10, 6))
    for i, tree_name in enumerate(sorted(trees)):
        pct, mean, std = _aggregate(trees[tree_name]["pct"], trees[tree_name]["time"])
        _plot_series(ax, pct, mean, std, tree_name, PALETTE[i % len(PALETTE)])

    ax.set_xlabel("Размер диапазона поиска (% от объёма пространства)")
    ax.set_ylabel("Время поиска (микросекунды)")
    ax.set_title("Зависимость времени 3D поиска от размера диапазона\n(N = 100,000)")
    ax.grid(True, linestyle="--", linewidth=0.8, alpha=0.7)
    ax.legend(title="Структура данных", loc="best")
    fig.tight_layout()

    save_path = os.path.join(output_dir, "search_vs_range.png")
    fig.savefig(save_path, bbox_inches="tight")
    print(f"График успешно сохранён в: {save_path}")
    plt.close(fig)


if __name__ == "__main__":
    # Предполагается, что скрипт запускается из корня проекта: python analysis/analyze.py
    results = load_benchmark_data("benchmark_results.json")
    plot_vary_n(results)
    plot_vary_range(results)