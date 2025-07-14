import sys
import matplotlib.pyplot as plt

def read_points(filename):
    with open(filename, 'r') as f:
        return [tuple(map(float, line.strip().split())) for line in f if line.strip()]

def draw(points, hull):
    plt.figure(figsize=(8, 8))


    px, py = zip(*points)
    plt.scatter(px, py, color='blue', s=10, label='Points') 


    if len(hull) >= 2:
        hx, hy = zip(*hull)
        plt.plot(hx, hy, color='red', linewidth=1.5, label='Convex Hull')

    plt.title("Convex Hull Visualization (float coords)")
    plt.axis('equal')
    plt.grid(True)
    plt.legend()
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