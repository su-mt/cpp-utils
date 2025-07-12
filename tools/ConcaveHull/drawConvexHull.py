import sys
import matplotlib.pyplot as plt

def read_points(filename):
    with open(filename, 'r') as f:
        return [tuple(map(float, line.strip().split())) for line in f if line.strip()]

def draw(points, hull):
    plt.figure(figsize=(10, 10), dpi=120)

    # Все точки — синие маленькие кружки с номерами
    px, py = zip(*points)
    plt.scatter(px, py, color='blue', s=15, label='Points', zorder=2)
    # Для больших наборов точек не подписываем индексы
    if len(points) <= 30:
        for idx, (x, y) in enumerate(points):
            plt.text(x + 0.03, y + 0.03, str(idx), fontsize=8, color='navy', alpha=0.7, zorder=3)

    # Выпуклая/вогнутая оболочка — красная ломаная с маркерами
    if len(hull) >= 2:
        hx, hy = zip(*hull)
        plt.plot(hx, hy, color='red', linewidth=2.2, marker='o', markersize=6, label='Hull', zorder=4)
        # Соединить последнюю и первую точку, если не совпадают
        if (hx[0], hy[0]) != (hx[-1], hy[-1]):
            plt.plot([hx[-1], hx[0]], [hy[-1], hy[0]], color='red', linewidth=2.2, linestyle='--', alpha=0.5, zorder=4)

    plt.title("Concave/Convex Hull Visualization (float coords)", fontsize=14)
    plt.xlabel('X', fontsize=12)
    plt.ylabel('Y', fontsize=12)
    plt.axis('equal')
    plt.minorticks_on()
    plt.grid(which='major', linestyle='-', linewidth=0.5, alpha=0.7)
    plt.grid(which='minor', linestyle=':', linewidth=0.3, alpha=0.4)
    plt.legend(fontsize=12)
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python drawConvexHull.py all_points.txt hull_points.txt")
        sys.exit(1)

    all_points_file = sys.argv[1]
    hull_points_file = sys.argv[2]

    try:
        all_points = read_points(all_points_file)
        hull_points = read_points(hull_points_file)
        draw(all_points, hull_points)
    except Exception as e:
        print(f"Ошибка: {e}")
        sys.exit(1)